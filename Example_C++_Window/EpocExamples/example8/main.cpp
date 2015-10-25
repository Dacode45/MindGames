/****************************************************************************
**
** Copyright 2014 by Emotiv. All rights reserved
** Example 8 - Multi Dongle EEGLogger
** This sample is to get synchronized eeg data from two headsets.
** The data is only written to files as two headsets are in the good condition
** ( without noise, full of battery, ... )
****************************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif

#include "EmoStateDLL.h"
#include "edk.h"
#include "edkErrorCode.h"

using namespace std;

// Set the channels
EE_DataChannel_t targetChannelList[] = {
		ED_COUNTER,
		ED_AF3, ED_F7, ED_F3, ED_FC5, ED_T7, 
		ED_P7, ED_O1, ED_O2, ED_P8, ED_T8, 
		ED_FC6, ED_F4, ED_F8, ED_AF4, ED_GYROX, ED_GYROY, ED_TIMESTAMP, 
		ED_FUNC_ID, ED_FUNC_VALUE, ED_MARKER, ED_SYNC_SIGNAL
	};

// This will go at the top of the output files
const char header[] = "COUNTER,AF3,F7,F3, FC5, T7, P7, O1, O2,P8" 
                      ", T8, FC6, F4,F8, AF4,GYROX, GYROY, TIMESTAMP, "   
                      "FUNC_ID, FUNC_VALUE, MARKER, SYNC_SIGNAL,";

bool IsHeadset1On  = false;
bool IsHeadset2On  = false;
bool onetime       = true;
int  write         =  0;
double* data1[100]     ;
double* data2[100]     ;

unsigned int numberOfSample1 = 0;
unsigned int numberOfSample2 = 0;

#ifdef __linux__
int _kbhit(void);
#endif

int main(int argc,char** argv[])
{
	// Create some structures to hold the data
	EmoEngineEventHandle eEvent = EE_EmoEngineEventCreate();
	EmoStateHandle eState = EE_EmoStateCreate();

    std::ofstream ofs1("data1.csv",std::ios::trunc);
	ofs1 << header << std::endl;
    std::ofstream ofs2("data2.csv",std::ios::trunc);
	ofs2 << header << std::endl;

	// Initialize the users
	// NOTE: Only expecting two for now
	unsigned int userID = -1;
	int userList[2] = {-1,-1};
	int totalSamples[2] = {0,0};		
	int fileNumbers[2] = {1,1};
	int numUsers = 0;
	std::ofstream ofs[2];

	// Initialize some parameter
	const unsigned short composerPort	= 1726;
	float secs							= 1;
	unsigned int datarate				= 0;
	bool readytocollect					= false;
	int option							= 0;
	int state							= 0;

	// Make sure we're connect
	if( EE_EngineConnect() == EDK_OK )
	{

		// Create the data holder
		DataHandle eData = EE_DataCreate();
		EE_DataSetBufferSizeInSec(secs);

		// Let them know about it
		std::cout << "Buffer size in secs:" << secs << std::endl;

		// How many samples per file?
		int samples_per_file = 384;		// 3 seconds

		// Presumably this will fail when we no longer
		//	receive data...
		while(!_kbhit()) 
		{
			// Grab the next event.
			// We seem to mainly care about user adds and removes
			int state = EE_EngineGetNextEvent(eEvent); 
			if( state == EDK_OK ) 
			{
				// Grab some info about the event
				EE_Event_t eventType = EE_EmoEngineEventGetType(eEvent); // same				
				EE_EmoEngineEventGetUserId(eEvent, &userID); // same

				// Do nothing if no user...
				if(userID==-1) {
					continue;	
				}
				

				// Add the user to the list, if necessary				
				if (eventType == EE_UserAdded)	
				{
					std::cout << "User added: " << userID << endl;
					EE_DataAcquisitionEnable(userID,true);
					userList[numUsers++] = userID;

					// Check
					if (numUsers > 2)
					{
                        throw std::runtime_error("Too many users!");
					}					
				} 
				else if (eventType == EE_UserRemoved)
				{
					cout << "User removed: " << userID << endl;
					if (userList[0] == userID)
					{
						userList[0] = userList[1];
						userList[1] = -1;
						numUsers--;
					}
					else if (userList[1] == userID)
					{
						userList[1] = -1;
						numUsers--;
					} 
				}
				
				// Might be ready to get going.
				if (numUsers == 2) {
					readytocollect = true;
				} else {
					readytocollect = false;
				}
			}	

			//EE_DataUpdateHandle(userID, eData);

			// If we've got both, then start collecting
			if (readytocollect && (state==EDK_OK)) 
			{		
				int check = EE_DataUpdateHandle(userID, eData);
				unsigned int nSamplesTaken=0;
				EE_DataGetNumberOfSample(eData,&nSamplesTaken);

				if( userID == 0 )
				{
					if( nSamplesTaken != 0) 
					{
						IsHeadset1On = true;

                        if( onetime) {
                            write = userID;
                            onetime = false;
                        }

                        for (int c = 0 ; c < sizeof(targetChannelList)/
                                             sizeof(EE_DataChannel_t) ; c++)
						{							
							data1[c] = new double[nSamplesTaken];
                            EE_DataGet(eData, targetChannelList[c], data1[c],
                                       nSamplesTaken);
							numberOfSample1 = nSamplesTaken;
						}
					}                    
                    else
                        IsHeadset1On = false;
				}

				if( userID == 1  )
				{  
					if(nSamplesTaken != 0) 
					{
						IsHeadset2On = true;
                        if( onetime) {
                            write = userID;
                            onetime = false;
                        }

                        for (int c = 0 ; c < sizeof(targetChannelList)/
                                             sizeof(EE_DataChannel_t) ; c++)
						{
							data2[c] = new double[nSamplesTaken];
                            EE_DataGet(eData, targetChannelList[c], data2[c],
                                       nSamplesTaken);
							numberOfSample2 = nSamplesTaken;
						}
					}
					else 
						IsHeadset2On = false;  									
				}
								
				if( IsHeadset1On && IsHeadset2On) 
				{ 
					cout <<"Update " << 0 <<" : " << numberOfSample1 << endl;
					for (int c = 0 ; c < numberOfSample1  ; c++)
					{
                        for (int i = 0 ; i<sizeof(targetChannelList)/
                                           sizeof(EE_DataChannel_t) ; i++)
						{
							ofs1 << data1[i][c] <<",";
						}

						ofs1 << std::endl;
					}

					cout <<"Update " << 1 <<" : " << numberOfSample2 << endl;
					for (int c = 0 ; c < numberOfSample2  ; c++)
					{
                        for (int i = 0 ; i<sizeof(targetChannelList)/
                                           sizeof(EE_DataChannel_t) ; i++)
						{ 
							ofs2 << data2[i][c] << ",";
						}

						ofs2 << std::endl;
					}

                    // Don't overload
					IsHeadset1On = false;
					IsHeadset2On = false;
				}
			}
		}
	}
	ofs1.close();
	ofs2.close();

	// Clean things up
	EE_EngineDisconnect();
	EE_EmoStateFree(eState);
	EE_EmoEngineEventFree(eEvent);	

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
