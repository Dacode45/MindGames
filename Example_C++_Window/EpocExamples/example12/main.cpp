/****************************************************************************
**
** Copyright 2014 by Emotiv. All rights reserved
** Example 12 - EdfSavingAndLoading
** This example demonstrates saving and playback(loading) data
** with an edf file
****************************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif
#ifdef __linux__
#include <unistd.h>
#endif
#include "EmoStateDLL.h"
#include "edk.h"
#include "edkErrorCode.h"

void logEmoState(std::ostream& os, unsigned int userID,
                 EmoStateHandle eState, bool withHeader = false);

#ifdef __linux__
int _kbhit(void);
#endif

std::string getCurrentTime();

int main(int argc, char** argv) {

	EmoEngineEventHandle eEvent			= EE_EmoEngineEventCreate();
	EmoStateHandle eState				= EE_EmoStateCreate();
	unsigned int userID					= 0;
	const unsigned short composerPort	= 1726;
	int option = 0;
	int state  = 0;
	std::string input;
	std::string edfFileName;
	std::string strTime = getCurrentTime();

	try {
		while (true)
		{
			std::cout.clear();
            std::cout << "==================================================================="
                      << std::endl;
            std::cout << "Example to show how to save eeg data to edf file"
                      << std::endl;
			std::cout << "and how to load eeg data from edf file," << std::endl;
            std::cout << "then get emostates from this file without using the headset"
                      << std::endl;
            std::cout << "==================================================================="
                      << std::endl;
            std::cout << "Press '1' to record EEG data into EDF file		                 "
                      << std::endl;
            std::cout << "Press '2' to load data from EDF file		                         "
                      << std::endl;
            std::cout << "Press '3' to exit													 "
                      << std::endl;
			std::cout << ">> ";

			std::cin.clear();
			fflush(stdin);
			std::getline(std::cin, input, '\n');
			option = atoi(input.c_str());

			switch (option) {
            case 1:
            {
#ifdef _WIN32
                std::cout << "\nEnter full path of the edf file:"
                             " \(for example: C:\\edf\\testdata.edf\)\n";
#endif
#ifdef __linux__
                std::cout << "\nEnter full path of the edf file:"
                             " \(for example: /home/user/my_edf.edf)\n";
#endif
                std::getline(std::cin, input, '\n');
                edfFileName = std::string(input);

                if (EE_EngineConnect() != EDK_OK) {
                    throw std::runtime_error("Emotiv Engine start up failed.");
                }
                EE_EdfStartSaving(userID, edfFileName.c_str(),
                                  "edfTest", "0", "01.01.2013", "00:00:00");
                break;
            }
            case 2:
            {
#ifdef _WIN32
                std::cout << "\nEnter full path of the edf file:"
                             " \(for example: C:\\edf\\testdata.edf\)\n";
#endif
#ifdef __linux__
                std::cout << "\nEnter full path of the edf file:"
                             " \(for example: /home/user/my_edf.edf)\n";
#endif
                std::getline(std::cin, input, '\n');
                edfFileName = std::string(input);

                if (EE_EngineLocalConnect(edfFileName.c_str()) != EDK_OK) {
                    throw std::runtime_error("Emotiv Engine with EDF file "
                                             "start up failed.");
                }

                std::cout << "\nEmoStates is logged into file EmoStates.csv\n";
                EE_EdfStart();

                break;
            }
            case 3:
            {
                break;
            }
            default:
                throw std::runtime_error("Invalid option...");
                break;
			}
			if (option ==3) break;
		
            std::cout << "Start receiving EmoState! Press any key to stop logging...\n"
                      << std::endl;
			
			std::ofstream ofs("EmoStates.csv");
			bool writeHeader = true;
		
			while (!_kbhit()) {

				state = EE_EngineGetNextEvent(eEvent);

				// New event needs to be handled
				if (state == EDK_OK) {

					EE_Event_t eventType = EE_EmoEngineEventGetType(eEvent);
					EE_EmoEngineEventGetUserId(eEvent, &userID);

					// Log the EmoState if it has been updated
					if (eventType == EE_EmoStateUpdated) {

						EE_EmoEngineEventGetEmoState(eEvent, eState);
						const float timestamp = ES_GetTimeFromStart(eState);

                        printf("%10.3fs : New EmoState from user %d ...\r",
                               timestamp, userID);
					
						logEmoState(ofs, userID, eState, writeHeader);
						writeHeader = false;
					}
				}
				else if (state != EDK_NO_EVENT) {
					std::cout << "Internal error in Emotiv Engine!" << std::endl;
					break;
				}

#ifdef _WIN32
            Sleep(100);
#endif
#ifdef __linux__
            sleep(1);
#endif
			}
			
			if (option == 1) EE_EdfStopSavingAll();
			ofs.close();
			EE_EngineDisconnect();
		}
	}
    catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		std::cout << "Press any key to exit..." << std::endl;
		getchar();
		EE_EngineDisconnect();
	}
	EE_EmoStateFree(eState);
	EE_EmoEngineEventFree(eEvent);
	return 0;
}


void logEmoState(std::ostream& os, unsigned int userID,
                 EmoStateHandle eState, bool withHeader) {

	// Create the top header
	if (withHeader) {
		os << "Time,";
		os << "UserID,";
		os << "Wireless Signal Status,";
		os << "Blink,";
		os << "Wink Left,";
		os << "Wink Right,";
		os << "Look Left,";
		os << "Look Right,";
		os << "Eyebrow,";
		os << "Furrow,";
		os << "Smile,";
		os << "Clench,";
		os << "Smirk Left,";
		os << "Smirk Right,";
		os << "Laugh,";
		os << "Short Term Excitement,";
		os << "Long Term Excitement,";
		os << "Engagement/Boredom,";
		os << "Cognitiv Action,";
		os << "Cognitiv Power,";
		os << std::endl;
	}

	// Log the time stamp and user ID
	os << ES_GetTimeFromStart(eState) << ",";
	os << userID << ",";
	os << static_cast<int>(ES_GetWirelessSignalStatus(eState)) << ",";

	// Expressiv Suite results
	os << ES_ExpressivIsBlink(eState) << ",";
	os << ES_ExpressivIsLeftWink(eState) << ",";
	os << ES_ExpressivIsRightWink(eState) << ",";

	os << ES_ExpressivIsLookingLeft(eState) << ",";
	os << ES_ExpressivIsLookingRight(eState) << ",";

	std::map<EE_ExpressivAlgo_t, float> expressivStates;

	EE_ExpressivAlgo_t upperFaceAction = ES_ExpressivGetUpperFaceAction(eState);
	float			   upperFacePower  = ES_ExpressivGetUpperFaceActionPower(eState);

	EE_ExpressivAlgo_t lowerFaceAction = ES_ExpressivGetLowerFaceAction(eState);
	float			   lowerFacePower  = ES_ExpressivGetLowerFaceActionPower(eState);

	expressivStates[ upperFaceAction ] = upperFacePower;
	expressivStates[ lowerFaceAction ] = lowerFacePower;
	
	os << expressivStates[ EXP_EYEBROW     ] << ","; // eyebrow
	os << expressivStates[ EXP_FURROW      ] << ","; // furrow
	os << expressivStates[ EXP_SMILE       ] << ","; // smile
	os << expressivStates[ EXP_CLENCH      ] << ","; // clench
	os << expressivStates[ EXP_SMIRK_LEFT  ] << ","; // smirk left
	os << expressivStates[ EXP_SMIRK_RIGHT ] << ","; // smirk right
	os << expressivStates[ EXP_LAUGH       ] << ","; // laugh

	// Affectiv Suite results
	os << ES_AffectivGetExcitementShortTermScore(eState) << ",";
	os << ES_AffectivGetExcitementLongTermScore(eState) << ",";

	os << ES_AffectivGetEngagementBoredomScore(eState) << ",";

	// Cognitiv Suite results
	os << static_cast<int>(ES_CognitivGetCurrentAction(eState)) << ",";
	os << ES_CognitivGetCurrentActionPower(eState);

	os << std::endl;
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

std::string getCurrentTime()
{
    time_t t = time(0);
    struct tm* now = localtime(&t);
    std::string timeString = asctime(now);
    timeString = timeString.erase(timeString.length()-1);
    return timeString;
}
