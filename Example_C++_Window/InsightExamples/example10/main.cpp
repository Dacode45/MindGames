/****************************************************************************
**
** Copyright 2014 by Emotiv. All rights reserved
** Example 10 - PerformanceMetric Demo
** This example receives the PerformanceMetric scores
** then calculates the scaled scores
** and writes them to the data file( .csv file ...ie)
****************************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cstdlib>
#include <stdexcept>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#endif
#ifdef __linux__
    #include <unistd.h>
#endif

#include "IEmoStateDLL.h"
#include "Iedk.h"
#include "IedkErrorCode.h"

void CaculateScale(double& rawScore, double& maxScale,
                   double& minScale, double& scaledScore);
void logPerformanceMetricScore(std::ostream& os, unsigned int userID,
                               EmoStateHandle eState, bool withHeader = false);

#ifdef __linux__
int _kbhit(void);
#endif

int main(int argc, char** argv) {

	EmoEngineEventHandle eEvent			= IEE_EmoEngineEventCreate();
	EmoStateHandle eState				= IEE_EmoStateCreate();
	unsigned int userID					= 0;
	const unsigned short composerPort	= 1726;
	int option = 0;
	int state  = 0;
	std::string input;

	try {

		if (argc != 2) {
            throw std::runtime_error("Please supply the log file name.\n"
                                     "Usage: PerformanceMetricDemo [log_file_name].");
		}

        std::cout << "==================================================================="
                  << std::endl;
        std::cout << "Example to show how to log the EmoState from EmoInsightDriver/"
                     "EmoInsightComposer."
                  << std::endl;
        std::cout << "==================================================================="
                  << std::endl;
        std::cout << "Press '1' to start and connect to the EmoInsightDriver "
                  << std::endl;
        std::cout << "Press '2' to connect to the EmoInsightComposer  "
                  << std::endl;
		std::cout << ">> ";

		std::getline(std::cin, input, '\n');
		option = atoi(input.c_str());

		switch (option) {
        case 1:
            {
                if (IEE_EngineConnect() != EDK_OK) {
                    throw std::runtime_error(
                                "Emotiv Insight Driver start up failed.");
                }
                break;
            }
        case 2:
            {
                std::cout << "Target IP of EmoInsightComposer? [127.0.0.1] ";
                std::getline(std::cin, input, '\n');

                if (input.empty()) {
                    input = std::string("127.0.0.1");
                }

                if (IEE_EngineRemoteConnect(input.c_str(), composerPort) != EDK_OK) {
                    std::string errMsg = "Cannot connect to EmoInsightComposer on [" +
                                                                            input + "]";
                    throw std::runtime_error(errMsg.c_str());
                }
                break;
            }
        default:
            throw std::runtime_error("Invalid option...");
            break;
		}


        std::cout << "Start receiving PerformanceMetric Score! "
                  << "Press any key to stop logging..."
                  << std::endl;

		std::ofstream ofs(argv[1]);
		bool writeHeader = true;

		while (!_kbhit()) {

			state = IEE_EngineGetNextEvent(eEvent);

			// New event needs to be handled
			if (state == EDK_OK) {

				IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
				IEE_EmoEngineEventGetUserId(eEvent, &userID);

				// Log the EmoState if it has been updated
				if (eventType == IEE_EmoStateUpdated) {

					IEE_EmoEngineEventGetEmoState(eEvent, eState);
					const float timestamp = IS_GetTimeFromStart(eState);

                    printf("%10.3fs : New PerformanceMetric score from user %d ...\r",
                           timestamp, userID);

					logPerformanceMetricScore(ofs, userID, eState, writeHeader);
					writeHeader = false;
				}
			}
			else if (state != EDK_NO_EVENT) {
				std::cout << "Internal error in Emotiv Engine!" << std::endl;
				break;
			}

#ifdef _WIN32
            Sleep(1);
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

	IEE_EngineDisconnect();
	IEE_EmoStateFree(eState);
	IEE_EmoEngineEventFree(eEvent);

	return 0;
}


void logPerformanceMetricScore(std::ostream& os, unsigned int userID,
                               EmoStateHandle eState, bool withHeader) {

	// Create the top header
	if (withHeader) {
		os << "Time,";
		os << "UserID,";
		os << "Stress raw score,";
		os << "Stress min score,";
		os << "Stress max score,";
		os << "Stress scaled score,";
		os << "Engagement boredom raw score,";
		os << "Engagement boredom min score,";
		os << "Engagement boredom max score,";
		os << "Engagement boredom scaled score,";
		os << "Relaxation raw score,";
		os << "Relaxation min score,";
		os << "Relaxation max score,";
		os << "Relaxation scaled score,";
		os << "Excitement raw score,";
		os << "Excitement min score,";
		os << "Excitement max score,";
		os << "Excitement scaled score,";
		os << "Interest raw score,";
		os << "Interest min score,";
		os << "Interest max score,";
		os << "Interest scaled score,";
		os << std::endl;
	}
	// Log the time stamp and user ID
	os << IS_GetTimeFromStart(eState) << ",";
	os << userID << ",";
	// PerformanceMetric results
	double rawScore=0;
	double minScale=0;
	double maxScale=0;	
	double scaledScore=0;
    IS_PerformanceMetricGetStressModelParams(eState,&rawScore,&minScale,
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
    IS_PerformanceMetricGetEngagementBoredomModelParams(eState,&rawScore,
                                                        &minScale,&maxScale);
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
    IS_PerformanceMetricGetRelaxationModelParams(eState,&rawScore,
                                                 &minScale,&maxScale);
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
    IS_PerformanceMetricGetInstantaneousExcitementModelParams(eState,
                                                              &rawScore,&minScale,
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
    IS_PerformanceMetricGetInterestModelParams(eState,&rawScore,
                                               &minScale,&maxScale);
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
void CaculateScale (double& rawScore, double& maxScale,
                    double& minScale, double& scaledScore){
	
	if (rawScore<minScale)
	{
		scaledScore =0;
	}else if (rawScore>maxScale)
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
