/****************************************************************************
**
** Copyright 2014 by Emotiv. All rights reserved
** Example 7 - Multi Dongle Connection
** this example demonstrates how to connect to two headsets at the same time
** It captures event when you plug or unplug dongle .
** Every time you plug or unplug dongle, there is a notice that dongle ID
** is added or removed
****************************************************************************/

#include <iostream>
#include <list>

#include "edk.h"
#include "edkErrorCode.h"
#include "EmoStateDLL.h"

using namespace std;

#ifdef _WIN32
#include <conio.h>
#endif

#ifdef __linux__
int _kbhit(void);
#endif

int main(int argc,char** argv[])
{
	EmoEngineEventHandle hEvent = EE_EmoEngineEventCreate();
	EmoStateHandle eState = EE_EmoStateCreate();
	unsigned int userID = -1;
	list<int> listUser;

	if( EE_EngineConnect() == EDK_OK )
	{
		while(!_kbhit()) 
		{
			int state = EE_EngineGetNextEvent(hEvent);
			if( state == EDK_OK )
			{
				EE_Event_t eventType = EE_EmoEngineEventGetType(hEvent);				
				EE_EmoEngineEventGetUserId(hEvent, &userID);
				if(userID==-1)
					continue;			

				if(eventType == EE_EmoStateUpdated  )
				{								
                    // Copies an EmoState returned with a EE_EmoStateUpdate event
                    // to memory referenced by an EmoStateHandle.
					if(EE_EmoEngineEventGetEmoState(hEvent,eState)==EDK_OK)
					{
						if(EE_GetUserProfile(userID,hEvent)==EDK_OK)
						{
							//Affective score, short term excitement						
							cout <<"userID: " << userID  <<endl;
                            cout <<"    affectiv excitement score: "
                                 << ES_AffectivGetExcitementShortTermScore (eState)
                                 << endl;
                            cout <<"    expressiv smile extent : "
                                 << ES_ExpressivGetSmileExtent(eState)
                                 <<endl;
						}						
					}					
					
				}
				// userremoved event
				else if( eventType == EE_UserRemoved )
				{
					cout <<"user ID: "<<userID<<" have removed" << endl;	
					listUser.remove(userID);
				}
				// useradded event 
				else if(eventType == EE_UserAdded)
				{
					listUser.push_back(userID);
					cout <<"user ID: "<<userID<<" have added" << endl;
				}		
				userID=-1;
			}			
		}
	}

	EE_EngineDisconnect();
	EE_EmoStateFree(eState);
	EE_EmoEngineEventFree(hEvent);	
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
