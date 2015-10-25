/****************************************************************************
**
** Copyright 2014 by Emotiv. All rights reserved
** Example 10 - Affectiv Demo
** This example receives the Affective scores then calculates the scaled scores
** and writes them to the data file( .csv file ...ie)
****************************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <stdexcept>
#include <cstdlib>

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

void CaculateScale(double& rawScore, double& maxScale, double& minScale,
                   double& scaledScore);
void logAffectivScore(std::ostream& os, unsigned int userID,
                      EmoStateHandle eState, bool withHeader = false);

#ifdef __linux__
int _kbhit(void);
#endif

int main(int argc, char** argv) {

	EmoEngineEventHandle eEvent			= EE_EmoEngineEventCreate();
	EmoStateHandle eState				= EE_EmoStateCreate();
	unsigned int userID					= 0;
	const unsigned short composerPort	= 1726;
	int option = 0;
	int state  = 0;
	std::string input;

	try {

		if (argc != 2) {
            throw std::runtime_error("Please supply the log file name.\n"
                                     "Usage: AffectivDemo [log_file_name].");
		}

        std::cout << "==================================================================="
                  << std::endl;
        std::cout << "Example to show how to log the EmoState from EmoEngine/EmoComposer."
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


        std::cout << "Start receiving Affectiv Score! "
                  << "Press any key to stop logging...\n"
                  << std::endl;

		std::ofstream ofs(argv[1]);
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

                    printf("%10.3fs : New Affectiv score from user %d ...\r",
                           timestamp, userID);

					logAffectivScore(ofs, userID, eState, writeHeader);
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

		ofs.close();
	}
    catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		std::cout << "Press any key to exit..." << std::endl;
		getchar();
	}

	EE_EngineDisconnect();
	EE_EmoStateFree(eState);
	EE_EmoEngineEventFree(eEvent);

	return 0;
}

void logAffectivScore(std::ostream& os, unsigned int userID,
                      EmoStateHandle eState, bool withHeader) {

	// Create the top header
	if (withHeader) {
		os << "Time,";
		os << "UserID,";
		os << "Frustration raw score,";
		os << "Frustration min score,";
		os << "Frustration max score,";
		os << "Frustration scaled score,";
		os << "Engagement boredom raw score,";
		os << "Engagement boredom min score,";
		os << "Engagement boredom max score,";
		os << "Engagement boredom scaled score,";
		os << "Meditation raw score,";
		os << "Meditation min score,";
		os << "Meditation max score,";
		os << "Meditation scaled score,";
		os << "Excitement raw score,";
		os << "Excitement min score,";
		os << "Excitement max score,";
		os << "Excitement scaled score,";
		os << "Valence raw score,";
		os << "Valence min score,";
		os << "Valence max score,";
		os << "Valence scaled score,";
		os << std::endl;
	}
	// Log the time stamp and user ID
	os << ES_GetTimeFromStart(eState) << ",";
	os << userID << ",";
	// Affectiv results
	double rawScore=0;
	double minScale=0;
	double maxScale=0;	
	double scaledScore=0;
	ES_AffectivGetFrustrationModelParams(eState,&rawScore,&minScale,&maxScale);
	os << rawScore << ",";
	os << minScale << ",";
	os << maxScale << ",";
	if (minScale==maxScale)
	{
		os << "undefined" << ",";
	}
	else{
		CaculateScale(rawScore,maxScale, minScale,scaledScore);
		os << scaledScore << ",";
	}		
    ES_AffectivGetEngagementBoredomModelParams(eState,&rawScore,&minScale,
                                               &maxScale);
	os << rawScore << ",";
	os << minScale << ",";
	os << maxScale << ",";
	if (minScale==maxScale)
	{
		os << "undefined" << ",";
	}
	else{
		CaculateScale(rawScore,maxScale, minScale,scaledScore);
		os << scaledScore << ",";
	}		
	ES_AffectivGetMeditationModelParams(eState,&rawScore,&minScale,&maxScale);
	os << rawScore << ",";
	os << minScale << ",";
	os << maxScale << ",";
	if (minScale==maxScale)
	{
		os << "undefined" << ",";
	}
	else{
		CaculateScale(rawScore,maxScale, minScale,scaledScore);
		os << scaledScore << ",";
	}		
    ES_AffectivGetExcitementShortTermModelParams(eState,&rawScore,&minScale,
                                                 &maxScale);
	os << rawScore << ",";
	os << minScale << ",";
	os << maxScale << ",";
	if (minScale==maxScale)
	{
		os << "undefined" << ",";
	}
	else{
		CaculateScale(rawScore,maxScale, minScale,scaledScore);
		os << scaledScore << ",";
	}		
	ES_AffectivGetValenceModelParams(eState,&rawScore,&minScale,&maxScale);
	os << rawScore << ",";
	os << minScale << ",";
	os << maxScale << ",";
	if (minScale==maxScale)
	{
		os << "undefined" << ",";
	}
	else{
		CaculateScale(rawScore,maxScale, minScale,scaledScore);
		os << scaledScore << ",";
	}
	os << std::endl;
}

void CaculateScale (double& rawScore, double& maxScale, double& minScale,
                    double& scaledScore){
	
	if (rawScore<minScale)
	{
		scaledScore =0;
    }
    else if (rawScore>maxScale)
	{
		scaledScore = 1;
	}
	else{
		scaledScore = (rawScore-minScale)/(maxScale-minScale);
	}
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
