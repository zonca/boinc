<?php
// This file is part of BOINC.
// http://boinc.berkeley.edu
// Copyright (C) 2011 University of California
//
// BOINC is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// BOINC is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with BOINC.  If not, see <http://www.gnu.org/licenses/>.

// remote job submission API

//// Implementation stuff follows

function req_to_xml($req, $op) {
    $x = "<$op>
    <authenticator>$req->authenticator</authenticator>
    <batch>
    <app_name>$req->app_name</app_name>
";
    foreach ($req->jobs as $job) {
        $x .= "    <job>
        <rsc_fpops_est>$job->rsc_fpops_est</rsc_fpops_est>
        <command_line>$job->command_line</command_line>
";
        foreach ($job->input_files as $file) {
            $x .= "        <input_file>
            <source>$file->source</source>
            <name>$file->name</name>
        </input_file>
";
        }
        $x .= "    </job>
";
    }
    $x .= "    </batch>
</$op>
";
    return $x;
}

function validate_request($req) {
    if (!is_object($req)) return "req is not an object";
    if (!array_key_exists('project', $req)) return "missing req->project";
    if (!array_key_exists('authenticator', $req)) return "missing req->authenticator";
    if (!array_key_exists('app_name', $req)) return "missing req->app_name";
    if (!array_key_exists('jobs', $req)) return "missing req->jobs";
    if (!is_array($req->jobs)) return "req->jobs is not an array";
    foreach ($req->jobs as $job) {
        // other checks
    }
    return null;
}

function do_http_op($project, $xml) {
    $ch = curl_init("$project/submit.php");
    curl_setopt($ch, CURLOPT_POST, 1);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_POSTFIELDS, "request=$xml");
    $reply = curl_exec($ch);
    if (!$reply) return array(null, "HTTP error");
    $r = simplexml_load_string($reply);
    if (!$r) return array(null, "Can't parse reply XML");
    return array($r, null);
}

function do_batch_op($req, $op) {
    $retval = validate_request($req);
    if ($retval) return array(null, $retval);
    $xml = req_to_xml($req, $op);
    return do_http_op($req->project, $xml);
}

//// Interface functions follow

function boinc_estimate_batch($req) {
    list($reply, $errmsg) = do_batch_op($req, "estimate_batch");
    if ($errmsg) return array(0, $errmsg);
    $name = $reply->getName();
    if ($name == 'estimate') {
        return array((string)$reply->seconds, null);
    } else if ($name = 'error') {
        return array(null, (string)$reply->message);
    } else {
        return array(null, "Bad reply message");
    }
}

function boinc_submit_batch($req) {
    list($reply, $errmsg) = do_batch_op($req, "submit_batch");
    if ($errmsg) return array(0, $errmsg);
    $name = $reply->getName();
    if ($name == 'batch_id') {
        return array((int)$reply, null);
    }  else if ($name == 'error') {
        return array(null, (string)$reply->message);
    } else {
        return array(null, "Bad reply message");
    }
}

function boinc_query_batches($req) {
    $req_xml = "<query_batches>
    <authenticator>$req->authenticator</authenticator>
</query_batches>
";
    list($reply, $errmsg) = do_http_op($req->project, $req_xml);
    if ($errmsg) return array(null, $errmsg);
    $batches = array();
    foreach ($reply->batch as $batch) {
        $b = null;
        $b->id = (int)($batch->id);
        $b->completed = (int)($batch->completed);
        $b->njobs = (int)($batch->njobs);
        $b->create_time = (double)($batch->create_time);
        if (!$batch->completed) {
            $b->fraction_done = (double) $batch->fraction_done;
        } else {
            $b->completed_time = (double)($batch->completed_time);
        }
        $batches[] = $b;
    }
    return array($batches, null);
}

function boinc_query_batch() {
    $req_xml = "<query_batch>
    <authenticator>$req->authenticator</authenticator>
    <batch_id>$req->batch_id</batch_id>
</query_batch>
";
    list($reply, $errmsg) = do_http_op($req->project, $req_xml);
    if ($errmsg) return array(null, $errmsg);
    $jobs = array();
    foreach ($reply->job as $job) {
        $j = null;
        $j->id = (int)($job->id);
        $jobs[] = $j;
    }
    $r = null;
    $r->jobs = $jobs;
    return array($r, null);
}

function boinc_abort_batch() {
}

//// example usage follows

if (0) {
    $req->project = "http://foo.edu/test/";
    $req->authenticator = "xxx";
    $req->app_name = "uppercase";
    $req->jobs = array();

    $f->source = "http://foo.edu/index.php";
    $f->name = "in";
    $job->input_files = array($f);

    for ($i=10; $i<20; $i++) {
        $job->rsc_fpops_est = $i*1e9;
        $job->command_line = "--t $i";
        $req->jobs[] = $job;
    }

    if (0) {
        list($e, $errmsg) = boinc_estimate_batch($req);
        if ($errmsg) {
            echo "Error from server: $errmsg\n";
        } else {
            echo "Batch completion estimate: $e seconds\n";
        }
    } else {
        list($id, $errmsg) = boinc_submit_batch($req);
        if ($errmsg) {
            echo "Error from server: $errmsg\n";
        } else {
            echo "Batch ID: $id\n";
        }
    }
}

if (0) {
    $req->project = "http://foo.edu/test/";
    $req->authenticator = "xxx";
    list($batches, $errmsg) = boinc_query_batches($req);
    if ($errmsg) {
        echo "Error: $errmsg\n";
    } else {
        print_r($batches);
    }
}

?>