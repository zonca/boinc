<?php
require_once("docutil.php");
page_head("The BOINC manager");
echo "
<p>
The <b>BOINC manager</b> program is available for Windows, Mac OS X
and Linux.  It controls the use of your computer's disk, network, and
processor resources, and is normally started at boot time.
<br>
<br>On Windows, the BOINC Manager is represented by an icon in the system tray.
<br>On Mac OS X, it is represented by icons in both the menubar and the Dock.
<!-- ** ROM ** is there an equivalent on Linux to the system tray icon? -->

<p>
On Windows:
    <ul>
    <li>Double-click on the icon to open the BOINC manager window.
    <li>Right-click on the icon to access its menu (shown below).
    </ul>
<br>
<a name=icon/>
<img src=mgrsystraymenu.png>
<br>
On Mac OS X:
    <ul>
    <li>Click on the icon in the menubar or Dock and hold the
    button down until the menu appears.
    </ul>
<p>
The icon menu choices are:
<ul>
<li> <b>Open BOINC Manager</b>: opens the current BOINC Manager.
<li> <b>Snooze</b>: stop work (computation and file transfer) for
one hour or until you cancel it.
See also <a href=#activity>Activity menu</a>.
<li> <b>About BOINC Manager</b>:  displays useful information about the
BOINC Manager.
<li> <b>Exit</b>:  exit the BOINC manager and all running BOINC applications.
No further work will take place until you run the BOINC manager again.
<br>(On Mac OS X, this menu item is called <b>Quit</b>.)
</ul>
<br>
<img src=mgrsystrayballoon.png>
<p>
Hovering over the BOINC icon will display a status balloon which contains
the project it is currently working on, how far along it is, and which
computer it is connected to (Windows only).
<h1>BOINC Manager Tabs</h1>
<h2>Projects</h2>
<p>
Shows the projects in which this computer is participating.
<br><br>
<img src=mgrprojects.png>
<p>

<h3>Status messages</h3>
<ul>
<li>Suspended by user:
    The project is currently suspended.
<li> Communication deferred ...:
    The client will wait the specified amount of time before attempting
    to contact the project server again.
    If no communication is required at that point, none will be attempted.
<li>Won't get new tasks:
    The project will not fill the cache for this project
    when it runs out of tasks.
</ul>

<h3>Commands</h3>
<p>
Click on a project name to enable the following additional buttons:

<ul>
<li> <b>Allow new tasks</b>:
    Allow the project to download additional work, if needed.
<li> <b>Detach</b>:
    Your computer will stop working for the project.
<li> <b>No new tasks</b>:
    Do not download any additional work for this project.
<li> <b>Reset project</b>:
    Stop the project's current work, if any,
    and start from scratch.
    Use this if BOINC has become stuck for some reason.
    Any unreported results and tasks in progress will be discarded.
<li> <b>Resume</b>:
    Resumes processing of a previously suspended project.
<li> <b>Suspend</b>:
    Suspends any further processing of this project.
<li> <b>Update</b>:
    Connect to the project;
    report all completed results,
    get more work if necessary,
    and get your latest <a href=prefs.php>preferences</a>.
</ul>
<h3>Web sites</h3>

<p>Project administrators can add <a href=gui_urls.php>buttons</a>
   to the manager to quickly navigate the project website.

<h2>Tasks</h2>
<p>Shows the tasks currently on your computer.
<br><br>
<img src=mgrwork.png>
<br>

<p>
Each task is either:
<ul>
<li>Aborted by user:
    Result has been aborted.
<li>Downloading:
    Input files are being downloaded.
<li>Preempted:
    Result has been suspended by the client-side scheduler and will be
    resumed the next time the project comes up in the processing rotation.
<li>Ready to report:
    Waiting to notify the scheduling server.
<li>Ready to run:
    An estimate of the total CPU time is shown.
<li>Running:
    Elapsed CPU time and estimated percent done is shown.
<li>Suspended by user:
    Task has been suspended by the user.
    Use the 'Resume' button on this tab to resume the task.
<li>Uploading:
    Output files are being uploaded.
<li>Activities Suspended:
    BOINC is suspended, either by the Activity->Suspend menu item or 
    benchmarks are running
<li>Project suspended by user:
    Project has been suspended by user.  Use the 'Resume' button on the
    Projects tab to resume the project.
</ul>

</ul>

<p>
Click on a result name to enable the following additional buttons:
<ul>
<li> <b>Abort</b>:
    Abort processing for a result. NOTE: This will prevent you from receiving
    credit for any work already completed.
<li> <b>Resume</b>:
    Resumes processing of a previously suspended task.  
    Note, this applies to tasks with status 'Suspended by user'.
<li> <b>Show graphics</b>:
    Open a window showing application graphics.
<li> <b>Suspend</b>:
    Suspends any further processing of this task.
</ul>

<h2>Transfers</h2>
<p>
Shows file transfers (uploads and downloads).
    These may be ready to start, in progress, and completed.
<br><br>
<img src=mgrtransfers.png>

<ul>
<li>Aborted by user:
    Result has been aborted and will be reported to the project server
    as a computational error.
<li>Downloading:
    Your computer is receiving data from the project server.
<li>Retry in ...:
    The client will wait the specified amount of time before attempting
    to contact the project server again.
<li>Uploading:
    Your computer is sending data to the project server.
</ul>

<p>Click on a file name to enable the following additional buttons:
<ul>
<li> <b>Retry Now</b>:
    Retry the file transfer now.
<li> <b>Abort Transfer</b>:
    Abort the file transfer. NOTE: This will prevent you from receiving credit
    for any work already completed.
</ul>

<h2>Messages</h2>
<p>
Shows status and error messages.
    You can <a href=client_msgs.php>control what messages are shown</a>.
    Messages are also written to a file 'stdoutdae.txt'.
<br>
<img src=mgrmessages.png>

<p>
Click on one or more messages to enable the following additional buttons:
<ul>
<li> <b>Copy all messages</b>:
    Copies all the messages to the clipboard.
<li> <b>Copy selected messages</b>:
    Copies the highlighted messages to the clipboard.
    NOTE: To highlight a message
    hold down the CTRL key (Windows) or Command key (Mac)
    and then click on the messages you want to store in the clipboard.
    When done click on the 'Copy selected messages' button to copy them
    to the clipboard.
</ul>

<h2>Statistics</h2>
<p>
Shows some simple charts and graphs about the user and host progress
<br><br>
<img src=mgrstatistics.png>
<p>
NOTE: This feature requires three connections to each project scheduler on three
   different days before it starts to work properly.
<h3>Command view</h3>
Click on any of the buttons to change to a different chart:
<ul>
<li> <b>Show user total</b>:
    Shows the user's credit totals for each project.
<li> <b>Show user average</b>:
    Shows the user's credit averages for each project.
<li> <b>Show host total</b>:
    Shows this computer's credit totals for each project.
<li> <b>Show host average</b>:
    Shows this computer's credit averages for each project.
</ul>

<h3>Project</h3>
Switch between projects when displaying one project at a time:
<ul>
<li> <b>Previous project</b>:
    Click this button to select the previous project.
<li> <b>Next project</b>:
    Click this button to select the next project
</ul>
<h3>Mode view</h3>
Click on any of the buttons to change the view:
<ul>
<li> <b>All projects</b>:
    Show all projects, one chart per project
<li> <b>One project</b>:
    Shows one project at a time.
    The chart and a list of projects will be shown,
    <br>switch between
    them using 'previous project' and 'next project' buttons.
<li> <b> All projects(sum)</b>
    Shows all projects in one chart.
</ul>

<h2>Disk</h2>
<p>This shows how much disk space is currently being used by each project.
<br><br>
<img src=mgrdisk.png>

<h1>BOINC Manager Menus</h1>

The BOINC manager has the following menus:
<ul>
<li> <b>File</b>
    <ul>
    <li> <b>Exit</b>:  exit the BOINC manager and all running BOINC applications.
        No further work will take place until you run the BOINC manager again.
        <br>(On Mac OS X, this is under the BOINC menu as <b>Quit BOINC</b>.)
    </ul>

<br>
<li> <b>Tools</b>
    <ul>
    <li> <b>Attach to project</b>:
        enroll this computer in a project.
        You will be asked to enter the project's URL and
        your email address and password.
<!-- ** ROM ** Please add a link to a page with details on using the Wizard. -->
    <li> <b>Account Manager</b>: attach to one or more new projects using an
        account manager web site.
        See <a href=/acct_mgrs.php>Account managers</a>
    </ul>

<br>
<a name=activity/>
<li> <b>Activity</b>
    <ul>
    <li> <b>Run always</b>: do work, regardless of preferences.
    <li> <b>Run based on preferences</b>: do work
        when your <a href=prefs.php>preferences</a> allow it.
    <li> <b>Suspend</b>: stop work (computation and file transfer).
    <li> <b>Network activity always available</b>: always allow BOINC to
        contact the project servers when it needs to.
    <li> <b>Network activity based on preferences</b>: allow BOINC to contact
        the project servers only when your <a href=prefs.php>preferences</a>
        allow it.
    <li> <b>Network activity suspended</b>: setting this keeps BOINC
        from attempting to contact any of the project servers.  It is useful
        for those on dial-up connections who do not want to be bothered with
        BOINC prompting to connect or disconnect for a time.
    </ul>
<br>
Notes:
<ul>
<li> Selecting an option that requires contacting a project
will temporarily enable network activity regardless of the network setting.
This includes updating a project, retrying file transfers,
retrying communications and attaching to projects.
Network activity will remain enabled for five minutes.
<li> Selecting <b>Snooze</b> from the <a href=#icon>icon menu</a>
overrides the 'run....' setting and suspends activity for one hour.
Unselecting <b>Snooze</b>, or Selecting the <b>Activity</b> menu items
<b>Run always</b> or <b>Run based on preferences</b>
cancels <b>Snooze</b>.
</ul>

<br>
<li> <b>Advanced</b>
    <ul>
    <li> <b>Options</b>: opens a dialog allowing you to select your preferred
        language, how often you wish to be reminded of the need to connect to
        the project servers (for dial-up users), etc.
        <br>If you connect to the web through an HTTP or SOCKS proxy,
        use this dialog to enter its address and port.
        <br>Windows only: use this dialog to tell BOINC your method of connecting
        to the Internet.
<!--  ** ROM ** Please add a link to a page with details on using this dialog. -->
    <li><b>Select Computer</b>: Allows you to control BOINC on a different
        computer
<!--  ** ROM ** is this correct?  Please add more details as appropriate -->

    <li><b>Run Benchmarks</b>:
        run benchmark functions, which measure the speed of your processor.
        BOINC does this automatically,
        but you can repeat it whenever you want.
        The results are shown in the Messages tab.
    <li><b>Retry Communications</b>: retry any deferred communications.
    </ul>

<br>
<li> <b>Help</b>
    <ul>
    <li> <b>BOINC Manager</b>:
        open this web page with instructions for using the BOINC manager.
        The F1 function key also does this.
    <li> <b>BOINC website</b>: open the main BOINC web page.
    <li> <b>About BOINC Manager</b>: show BOINC manager version number (on Mac OS X,
        this command is under the BOINC menu.)
    </ul>
</ul>

<p>
Menu names and other text in the BOINC manager can be displayed in
<a href=language.php>languages other than English</a>.
<p>
To select the <b>BOINC screensaver</b>:
    <ul>
    <li> <b>Windows</b>: use the Display Properties dialog.
    <li> <b>Mac OS X</b>: select System Preferences under the Apple menu and
        click on \"Screen Saver\".
    </ul>
 <p>
 For dialup configuration in Windows, see the <a href=dialup.php>
 Dial-up Connections</a> page.

";
page_tail();
?>
