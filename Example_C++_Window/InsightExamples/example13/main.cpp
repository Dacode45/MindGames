/****************************************************************************
**
** Copyright 2014 by Emotiv. All rights reserved
** Example 13 - CloudGetReportOnline
** Example deonstrates how to get report online from the cloud server
**
****************************************************************************/

#include <iostream>
#include <stdlib.h>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#endif

#ifdef __linux__
    #include <unistd.h>
#endif

#include "Iedk.h"
#include "IEmoStateDLL.h"
#include "elsClient.h"

#ifdef __linux__
int _kbhit(void);
#endif

void getOnlineReport(int);

int main()
{
	EmoEngineEventHandle eEvent			= IEE_EmoEngineEventCreate();
	unsigned int engineUserID			= 0;
	bool ready							= false;
	int state							= 0;
	int err = 0;

    while( err = IEE_EngineConnect() )
    {
#ifdef _WIN32
        Sleep(15);
#endif
#ifdef __linux__
        sleep(1);
#endif
	}

	while (!_kbhit())
	{
		state = IEE_EngineGetNextEvent(eEvent);
        if (state == EDK_OK)
        {

			IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
			IEE_EmoEngineEventGetUserId(eEvent, &engineUserID);

            if (eventType == IEE_UserAdded)
            {
				std::cout << "\nEngine User added\n";
				ready = true;
			}
		}
		if (ready)
		{
			getOnlineReport(engineUserID);
			break;
		}
		else 
		{
            std::cout << "\n please connect to the headset...";
		}
#ifdef _WIN32
        Sleep(1000);
#endif
#ifdef __linux__
        sleep(1);
#endif
	}

	IEE_EngineDisconnect();

    return 0;
}

#ifdef __linux__
int _kbhit(void)
{
    struct timeval tv;
    fd_set read_fd;

    tv.tv_sec=0;
    tv.tv_usec=0;

    FD_ZERO(&read_fd);
    FD_SET(0,&read_fd);

    if(select(1, &read_fd,NULL, NULL, &tv) == -1)
    return 0;

    if(FD_ISSET(0,&read_fd))
        return 1;

    return 0;
}
#endif

void getOnlineReport(int engineUserID)
{
    int userID       = 0,
        experimentID = 0,
        protocolID   = 0,
        engagement   = 0,
        excitement   = 0,
        stress       = 0,
        relax        = 0,
        interest     = 0;
    bool getReport   = true;

    ELS_Connect();
	char* clientID = "";	//
	char* clientSecret = "";	//clientID and clientSecret of your app
	ELS_SetClientSecret(clientID, clientSecret);
    std::cout << "sign in " << ELS_Login("jqk", "jqk", &userID) << "\r\n";
    std::cout << "userID " << userID << "\r\n";
    std::cout << "create protocol " << ELS_CreateProtocol("example", &protocolID)
         << "\r\n";
    std::cout << "protocolID " << protocolID << "\r\n";
    std::cout << "create exp "
              << ELS_CreateExperiment("example", "des", &experimentID)
              << "\r\n";
    std::cout << "exp " << experimentID << "\r\n";
    std::cout << "create Headset "<< ELS_CreateHeadset(engineUserID) <<"\r\n";
    std::cout << "create session " << ELS_CreateRecordingSession() << "\r\n";

    std::cout << "start record " << ELS_StartRecordData() << "\r\n";
    std::cout << "create marker " << ELS_Marker_EyeOpenStart() << "\r\n";

#ifdef _WIN32
            Sleep(300);
#endif
#ifdef __linux__
            sleep(3);
#endif
    std::cout << "create marker " << ELS_Marker_EyeOpenEnd() << "\r\n";
#ifdef _WIN32
            Sleep(500);
#endif
#ifdef __linux__
            sleep(5);
#endif
    std::cout << "create marker " << ELS_Marker_EyeClosedStart() << "\r\n";
#ifdef _WIN32
            Sleep(300);
#endif
#ifdef __linux__
            sleep(3);
#endif
    std::cout << "create marker " << ELS_Marker_EyeClosedEnd() << "\r\n";
#ifdef _WIN32
            Sleep(300);
#endif
#ifdef __linux__
            sleep(3);
#endif
    std::cout << "create marker " << ELS_Marker_RecordingStart() << "\r\n";
#ifdef _WIN32
            Sleep(5000);
#endif
#ifdef __linux__
            sleep(5);
#endif

    std::cout << "\n stop " ;
    if (ELS_StopRecordData())
    {
        std::cout<< " success\n";

        while(getReport)
        {
            ELS_GetReportOnline(&engagement,&excitement,&stress,&relax,&interest);
            if(engagement !=0)
            {
                std::cout << "\nReport is " << engagement << " " << excitement
                    << " " << stress << " " << relax << " " << interest;
                std::cout << std::endl;
                getReport = false;
            }
            else
            {
                std::cout << "\n Waiting for report...";
            }
#ifdef _WIN32
            Sleep(5000);
#endif
#ifdef __linux__
            sleep(5);
#endif
        }
    }
    else
    {
        std::cout<<" Fail to record data!";
    }

    ELS_Disconnect();
}
