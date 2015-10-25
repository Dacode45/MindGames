/****************************************************************************
**
** Copyright 2014 by Emotiv. All rights reserved
** Example 5 - EEGLogger
** This example demonstrates how to extract live EEG data using the EmoEngineTM
** in C++. Data is read from the headset and sent to an output file for
** later analysis
**
****************************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <stdexcept>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif
#ifdef __linux__
#include <unistd.h>
#endif
#include <map>

#include "EmoStateDLL.h"
#include "edk.h"
#include "edkErrorCode.h"


EE_DataChannel_t targetChannelList[] = {
		ED_COUNTER,
		ED_AF3, ED_F7, ED_F3, ED_FC5, ED_T7, 
		ED_P7, ED_O1, ED_O2, ED_P8, ED_T8, 
		ED_FC6, ED_F4, ED_F8, ED_AF4, ED_GYROX, ED_GYROY, ED_TIMESTAMP, 
		ED_FUNC_ID, ED_FUNC_VALUE, ED_MARKER, ED_SYNC_SIGNAL
	};

const char header[] = "COUNTER,AF3,F7,F3, FC5, T7, P7, O1, O2,P8" 
                      ", T8, FC6, F4,F8, AF4,GYROX, GYROY, TIMESTAMP, "   
                      "FUNC_ID, FUNC_VALUE, MARKER, SYNC_SIGNAL,";

#ifdef __linux__
int _kbhit(void);
#endif

int main(int argc, char** argv) {

	EmoEngineEventHandle eEvent			= EE_EmoEngineEventCreate();
	EmoStateHandle eState				= EE_EmoStateCreate();
	unsigned int userID					= 0;
	const unsigned short composerPort	= 1726;
	float secs							= 1;
	unsigned int datarate				= 0;
	bool readytocollect					= false;
	int option							= 0;
	int state							= 0;


	std::string input;

	try {

		if (argc != 2) {
            throw std::runtime_error("Please supply the log file name.\n"
                                     "Usage: EEGLogger [log_file_name].");
		}

        std::cout << "==================================================================="
                  << std::endl;
        std::cout << "Example to show how to log EEG Data from EmoEngine/EmoComposer."
                  << std::endl;
        std::cout << "==================================================================="
                  << std::endl;
        std::cout << "Press '1' to start and connect to the EmoEngine                    "
                  << std::endl;
        std::cout << "Press '2' to connect to the EmoComposer                            "
                  << std::endl;
		std::cout << ">> ";

		std::getline(std::cin, input, '\n');
		option = atoi(input.c_str());

		switch (option) {
			case 1:
			{
				if (EE_EngineConnect() != EDK_OK) {
                    throw std::runtime_error("Emotiv Engine start up failed.");
				}
				break;
			}
			case 2:
			{
				std::cout << "Target IP of EmoComposer? [127.0.0.1] ";
				std::getline(std::cin, input, '\n');

				if (input.empty()) {
					input = std::string("127.0.0.1");
				}

				if (EE_EngineRemoteConnect(input.c_str(), composerPort) != EDK_OK) {
                    std::string errMsg = "Cannot connect to EmoComposer on [" +
                                         input + "]";
                    throw std::runtime_error(errMsg.c_str());
				}
				break;
			}
			default:
                throw std::runtime_error("Invalid option...");
				break;
		}
		
		
        std::cout << "Start receiving EEG Data! Press any key to stop logging...\n"
                  << std::endl;
    	std::ofstream ofs(argv[1],std::ios::trunc);
		ofs << header << std::endl;
		
		DataHandle hData = EE_DataCreate();
		EE_DataSetBufferSizeInSec(secs);

		std::cout << "Buffer size in secs:" << secs << std::endl;
		
		while (!_kbhit()) {

			state = EE_EngineGetNextEvent(eEvent);
			if (state == EDK_OK) {

				EE_Event_t eventType = EE_EmoEngineEventGetType(eEvent);
				EE_EmoEngineEventGetUserId(eEvent, &userID);

				// Log the EmoState if it has been updated
				if (eventType == EE_UserAdded) {
					std::cout << "User added";
					EE_DataAcquisitionEnable(userID,true);
					readytocollect = true;
				}
			}

			if (readytocollect) {
						
                EE_DataUpdateHandle(0, hData);

                unsigned int nSamplesTaken=0;
                EE_DataGetNumberOfSample(hData,&nSamplesTaken);

                std::cout << "Updated " << nSamplesTaken << std::endl;

                if (nSamplesTaken != 0) {

                    double* data = new double[nSamplesTaken];
                    for (int sampleIdx=0 ; sampleIdx<(int)nSamplesTaken ;
                         ++ sampleIdx) {
                        for (int i = 0 ; i<sizeof(targetChannelList)/
                                           sizeof(EE_DataChannel_t) ; i++) {
                            EE_DataGet(hData, targetChannelList[i], data,
                                       nSamplesTaken);
                            ofs << data[sampleIdx] << ",";
                        }
                        ofs << std::endl;
                    }
                    delete[] data;
                }
			}
#ifdef _WIN32
			Sleep(100);
#endif
#ifdef __linux__
            sleep(1);
#endif
		}

		ofs.close();
		EE_DataFree(hData);		
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		std::cout << "Press any key to exit..." << std::endl;
		getchar();
	}

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
