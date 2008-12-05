// Berkeley Open Infrastructure for Network Computing
// http://boinc.berkeley.edu
// Copyright (C) 2005 University of California
//
// This is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation;
// either version 2.1 of the License, or (at your option) any later version.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//
// To view the GNU Lesser General Public License visit
// http://www.gnu.org/copyleft/lesser.html
// or write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "stdafx.h"
#include "boinccas.h"
#include "CACreateBOINCGroups.h"
#include "lsaprivs.h"


#define CUSTOMACTION_NAME               _T("CACreateBOINCGroups")
#define CUSTOMACTION_PROGRESSTITLE      _T("Validating user groups used by BOINC for secure sandboxes")


/////////////////////////////////////////////////////////////////////
// 
// Function:    
//
// Description: 
//
/////////////////////////////////////////////////////////////////////
CACreateBOINCGroups::CACreateBOINCGroups(MSIHANDLE hMSIHandle) :
    BOINCCABase(hMSIHandle, CUSTOMACTION_NAME, CUSTOMACTION_PROGRESSTITLE)
{}


/////////////////////////////////////////////////////////////////////
// 
// Function:    
//
// Description: 
//
/////////////////////////////////////////////////////////////////////
CACreateBOINCGroups::~CACreateBOINCGroups()
{
    BOINCCABase::~BOINCCABase();
}


/////////////////////////////////////////////////////////////////////
// 
// Function:    
//
// Description: 
//
/////////////////////////////////////////////////////////////////////
UINT CACreateBOINCGroups::OnExecution()
{
    NET_API_STATUS   nasReturnValue;
    DWORD            dwParameterError;
    UINT             uiReturnValue = -1;
    BOOL             bBOINCAdminsCreated = FALSE;
    BOOL             bBOINCUsersCreated = FALSE;
    BOOL             bBOINCProjectsCreated = FALSE;
    tstring          strUserSID;
    tstring          strBOINCMasterAccountUsername;
    tstring          strBOINCProjectAccountUsername;
    tstring          strEnableProtectedApplicationExecution;
    tstring          strEnableUseByAllUsers;


    uiReturnValue = GetProperty( _T("UserSID"), strUserSID );
    if ( uiReturnValue ) return uiReturnValue;

    uiReturnValue = GetProperty( _T("BOINC_MASTER_USERNAME"), strBOINCMasterAccountUsername );
    if ( uiReturnValue ) return uiReturnValue;

    uiReturnValue = GetProperty( _T("BOINC_PROJECT_USERNAME"), strBOINCProjectAccountUsername );
    if ( uiReturnValue ) return uiReturnValue;

    uiReturnValue = GetProperty( _T("ENABLEPROTECTEDAPPLICATIONEXECUTION2"), strEnableProtectedApplicationExecution );
    if ( uiReturnValue ) return uiReturnValue;

    uiReturnValue = GetProperty( _T("ENABLEUSEBYALLUSERS"), strEnableUseByAllUsers );
    if ( uiReturnValue ) return uiReturnValue;


    // Create the 'boinc_admins' group if needed
    //
    LOCALGROUP_INFO_1 lgrpiAdmins;

    lgrpiAdmins.lgrpi1_name = _T("boinc_admins");
    lgrpiAdmins.lgrpi1_comment = _T("Accounts in this group can control the BOINC client.");

    nasReturnValue = NetLocalGroupAdd(
        NULL,
        1,
        (LPBYTE)&lgrpiAdmins,
        &dwParameterError
    );

    if ((NERR_Success != nasReturnValue) && (ERROR_ALIAS_EXISTS != nasReturnValue)) {
        LogMessage(
            INSTALLMESSAGE_ERROR,
            NULL, 
            NULL,
            NULL,
            nasReturnValue,
            _T("Failed to create the 'boinc_admins' group.")
        );
        return ERROR_INSTALL_FAILURE;
    }

    if (NERR_Success == nasReturnValue) {
        bBOINCAdminsCreated = TRUE;
    }

    // If we just created the 'boinc_admins' local group then we need to populate
    //   it with the default accounts.
    PSID                         pAdminSID = NULL;
    PSID                         pInstallingUserSID = NULL;
    PSID                         pBOINCMasterSID = NULL;
    LOCALGROUP_MEMBERS_INFO_0    lgrmiAdmins;
    SID_IDENTIFIER_AUTHORITY     SIDAuthNT = SECURITY_NT_AUTHORITY;


    // Create a SID for the BUILTIN\Administrators group.
    if(!AllocateAndInitializeSid(
                     &SIDAuthNT, 2,
                     SECURITY_BUILTIN_DOMAIN_RID,
                     DOMAIN_ALIAS_RID_ADMINS,
                     0, 0, 0, 0, 0, 0,
                     &pAdminSID)) 
    {
        LogMessage(
            INSTALLMESSAGE_ERROR,
            NULL, 
            NULL,
            NULL,
            GetLastError(),
            _T("AllocateAndInitializeSid Error for BUILTIN\\Administrators")
        );
        return ERROR_INSTALL_FAILURE;
    }

    lgrmiAdmins.lgrmi0_sid = pAdminSID;

    nasReturnValue = NetLocalGroupAddMembers(
        NULL,
        _T("boinc_admins"),
        0,
        (LPBYTE)&lgrmiAdmins,
        1
    );

    if ((NERR_Success != nasReturnValue) && (ERROR_MEMBER_IN_ALIAS != nasReturnValue)) {
        LogMessage(
            INSTALLMESSAGE_ERROR,
            NULL, 
            NULL,
            NULL,
            nasReturnValue,
            _T("Failed to add user to the 'boinc_admins' group (Administrator).")
        );
        return ERROR_INSTALL_FAILURE;
    }

    if(pAdminSID != NULL) FreeSid(pAdminSID);


    // Create a SID for the current logged in user.
    if(!ConvertStringSidToSid(strUserSID.c_str(), &pInstallingUserSID)) 
    {
        LogMessage(
            INSTALLMESSAGE_ERROR,
            NULL, 
            NULL,
            NULL,
            GetLastError(),
            _T("ConvertStringSidToSid Error for installing user")
        );
        return ERROR_INSTALL_FAILURE;
    }

    lgrmiAdmins.lgrmi0_sid = pInstallingUserSID;

    nasReturnValue = NetLocalGroupAddMembers(
        NULL,
        _T("boinc_admins"),
        0,
        (LPBYTE)&lgrmiAdmins,
        1
    );

    if ((NERR_Success != nasReturnValue) && (ERROR_MEMBER_IN_ALIAS != nasReturnValue)) {
        LogMessage(
            INSTALLMESSAGE_ERROR,
            NULL, 
            NULL,
            NULL,
            nasReturnValue,
            _T("Failed to add user to the 'boinc_admins' group (Installing User).")
        );
        return ERROR_INSTALL_FAILURE;
    }

    if(pInstallingUserSID != NULL) HeapFree(GetProcessHeap(), 0, pInstallingUserSID);


    // Create a SID for the 'boinc_master' user account.
    if(!GetAccountSid(NULL, strBOINCMasterAccountUsername.c_str(), &pBOINCMasterSID))
    {
        LogMessage(
            INSTALLMESSAGE_ERROR,
            NULL, 
            NULL,
            NULL,
            GetLastError(),
            _T("GetAccountSid Error for 'boinc_master' user account")
        );
        return ERROR_INSTALL_FAILURE;
    }

    lgrmiAdmins.lgrmi0_sid = pBOINCMasterSID;

    nasReturnValue = NetLocalGroupAddMembers(
        NULL,
        _T("boinc_admins"),
        0,
        (LPBYTE)&lgrmiAdmins,
        1
    );

    if ((NERR_Success != nasReturnValue) && (ERROR_MEMBER_IN_ALIAS != nasReturnValue)) {
        LogMessage(
            INSTALLMESSAGE_ERROR,
            NULL, 
            NULL,
            NULL,
            nasReturnValue,
            _T("Failed to add user to the 'boinc_admins' group (BOINC Master).")
        );
        return ERROR_INSTALL_FAILURE;
    }

    if(pBOINCMasterSID != NULL) HeapFree(GetProcessHeap(), 0, pBOINCMasterSID);


    // Create the 'boinc_users' group if needed
    //
    LOCALGROUP_INFO_1 lgrpiUsers;

    lgrpiUsers.lgrpi1_name = _T("boinc_users");
    lgrpiUsers.lgrpi1_comment = _T("Accounts in this group can monitor the BOINC client.");

    nasReturnValue = NetLocalGroupAdd(
        NULL,
        1,
        (LPBYTE)&lgrpiUsers,
        &dwParameterError
    );

    if ((NERR_Success != nasReturnValue) && (ERROR_ALIAS_EXISTS != nasReturnValue)) {
        LogMessage(
            INSTALLMESSAGE_ERROR,
            NULL, 
            NULL,
            NULL,
            nasReturnValue,
            _T("Failed to create the 'boinc_users' group.")
        );
        return ERROR_INSTALL_FAILURE;
    }

    if (NERR_Success == nasReturnValue) {
        bBOINCUsersCreated = TRUE;
    }

    // If we just created the 'boinc_users' local group or the user has enabled all
    //   users to manage BOINC then we need to make sure that the 'Everyone' group
    //   is added to the 'boinc_users' group.
    if (_T("1") == strEnableUseByAllUsers) {

        PSID                         pEveryoneSID = NULL;
        LOCALGROUP_MEMBERS_INFO_0    lgrmiUsers;
        SID_IDENTIFIER_AUTHORITY     SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;

        // Create a well-known SID for the Everyone group.
        if(!AllocateAndInitializeSid(
                         &SIDAuthWorld, 1,
                         SECURITY_WORLD_RID,
                         0, 0, 0, 0, 0, 0, 0,
                         &pEveryoneSID))
        {
            LogMessage(
                INSTALLMESSAGE_ERROR,
                NULL, 
                NULL,
                NULL,
                GetLastError(),
                _T("AllocateAndInitializeSid Error for Everyone group")
            );
        }


        lgrmiUsers.lgrmi0_sid = pEveryoneSID;

        nasReturnValue = NetLocalGroupAddMembers(
            NULL,
            _T("boinc_users"),
            0,
            (LPBYTE)&lgrmiUsers,
            1
        );

        if ((NERR_Success != nasReturnValue) && (ERROR_MEMBER_IN_ALIAS != nasReturnValue)) {
            LogMessage(
                INSTALLMESSAGE_ERROR,
                NULL, 
                NULL,
                NULL,
                nasReturnValue,
                _T("Failed to add user to the 'boinc_users' group (Everyone).")
            );
            return ERROR_INSTALL_FAILURE;
        }

        if(pEveryoneSID != NULL) FreeSid(pEveryoneSID);
    }


    // Create the 'boinc_project' group if needed
    //
    LOCALGROUP_INFO_1 lgrpiProjects;

    lgrpiProjects.lgrpi1_name = _T("boinc_projects");
    lgrpiProjects.lgrpi1_comment = _T("Accounts in this group are used to execute boinc applications.");

    nasReturnValue = NetLocalGroupAdd(
        NULL,
        1,
        (LPBYTE)&lgrpiProjects,
        &dwParameterError
    );

    if ((NERR_Success != nasReturnValue) && (ERROR_ALIAS_EXISTS != nasReturnValue)) {
        LogMessage(
            INSTALLMESSAGE_ERROR,
            NULL, 
            NULL,
            NULL,
            nasReturnValue,
            _T("Failed to create the 'boinc_projects' group.")
        );
        return ERROR_INSTALL_FAILURE;
    }

    if (NERR_Success == nasReturnValue) {
        bBOINCProjectsCreated = TRUE;
    }

    // If we just created the 'boinc_projects' local group or the user has enabled 
    //   protected application execution then we need to add the 'boinc_project'
    //   account to the local group.
    if (_T("1") == strEnableProtectedApplicationExecution) {

        PSID                         pBOINCProjectSID = NULL;
        LOCALGROUP_MEMBERS_INFO_0    lgrmiProjects;
        SID_IDENTIFIER_AUTHORITY     SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;

        // Create a SID for the 'boinc_project' user account.
        if(!GetAccountSid(NULL, strBOINCProjectAccountUsername.c_str(), &pBOINCProjectSID))
        {
            LogMessage(
                INSTALLMESSAGE_ERROR,
                NULL, 
                NULL,
                NULL,
                GetLastError(),
                _T("GetAccountSid Error for 'boinc_master' user account")
            );
            return ERROR_INSTALL_FAILURE;
        }

        lgrmiProjects.lgrmi0_sid = pBOINCProjectSID;

        nasReturnValue = NetLocalGroupAddMembers(
            NULL,
            _T("boinc_projects"),
            0,
            (LPBYTE)&lgrmiProjects,
            1
        );

        if ((NERR_Success != nasReturnValue) && (ERROR_MEMBER_IN_ALIAS != nasReturnValue)) {
            LogMessage(
                INSTALLMESSAGE_ERROR,
                NULL, 
                NULL,
                NULL,
                nasReturnValue,
                _T("Failed to add user to the 'boinc_projects' group (boinc_project).")
            );
            return ERROR_INSTALL_FAILURE;
        }

        if(pBOINCProjectSID != NULL) FreeSid(pBOINCProjectSID);
    }


    SetProperty( _T("BOINC_ADMINS_GROUPNAME"), _T("boinc_admins") );
    SetProperty( _T("BOINC_USERS_GROUPNAME"), _T("boinc_users") );
    SetProperty( _T("BOINC_PROJECTS_GROUPNAME"), _T("boinc_projects") );

    if (bBOINCAdminsCreated || bBOINCUsersCreated || bBOINCProjectsCreated) {
        RebootWhenFinished();
    }

    return ERROR_SUCCESS;
}


/////////////////////////////////////////////////////////////////////
// 
// Function:    CreateBOINCGroups
//
// Description: This custom action creates the three user groups that'll
//              be used to enfore the account based sandboxing scheme
//              on Windows.
//
/////////////////////////////////////////////////////////////////////
UINT __stdcall CreateBOINCGroups(MSIHANDLE hInstall)
{
    UINT uiReturnValue = 0;

    CACreateBOINCGroups* pCA = new CACreateBOINCGroups(hInstall);
    uiReturnValue = pCA->Execute();
    delete pCA;

    return uiReturnValue;
}

const char *BOINC_RCSID_00ed9786df="$Id: CACreateBOINCGroups.cpp 11804 2007-01-08 18:42:48Z rwalton $";
