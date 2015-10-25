/****************************************************************************
**
** Copyright 2014 by Emotiv. All rights reserved
** Example 16 - ProfileUpload
** Example lists all profiles of a user , and demonstrates how to upload and delete a profile.
** 
****************************************************************************/

#include <iostream>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#endif

#ifdef __linux__
    #include <unistd.h>
#endif

#include "elsClient.h"

using namespace std;

void printProfileDetails();

int main()
{
    ELS_Connect();
	char* clientID = "";	//
	char* clientSecret = "";	//clientID and clientSecret of your app
	ELS_SetClientSecret(clientID, clientSecret);
    int userID;
    cout<<"Log in "<<ELS_Login("jqk", "jqk", &userID)<<"\r\n";
    cout<<"user ID: "<< userID<< "\r\n";
    printProfileDetails();
    ELS_UploadProfileFile("profile 1", "profile.emu",TRAINING);
    printProfileDetails();

    ELS_GetProfileId("profile 1");
    int pID = ELS_GetProfileId("profile 1");
    if (pID>=0)
    {
        ELS_DownloadFileProfile(pID, "profile1.emu");
        ELS_DeleteProfileFile(pID);
    }
    printProfileDetails();

    return 0;
}

void printProfileDetails()
{
    int nProfile = ELS_GetAllProfileName();

    cout<<"number of profiles: "<< nProfile<<"\n";

    for (int i = 0; i<nProfile; i++)
    {
        cout<< ELS_ProfileIDAtIndex(i)<<", ";
        cout<< ELS_ProfileNameAtIndex(i)<<",";
        cout<<((ELS_ProfileTypeAtIndex(i) == profileFileType::TRAINING)?
                                            "TRAINING":"EMOKEY") <<",";
        cout<< ELS_ProfileLastModifiedAtIndex(i)<<",\r\n";
    }
}
