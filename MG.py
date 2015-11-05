#entry point
import MGDataLoader
import MGNeuralNet
import MGVisualizer


import os
import argparse
import datetime
import time

import json



def GetDataList():
    data = {}
    with open('data/datalist.json') as data_list:
        data = json.load(data_list)
    return data


def SaveDataList(datalist):
    with open('data/') as data_list:
        json.dump(datalist, data_list)


#GLOBAL VARIABLES
VERBOSITY = False
DEBUG = False

def run(user, interval):
    if not user:
        raise(ValueException("Must run with a user"))

    now = datetime.datetime.now()
    datalist = GetDataList()
    #Check if user is in datalist. If not create a user id for him
    if user in datalist['users']:
        userID = int(datalist['users'][user])
    else
        userID = len(datalist['users'])
        datalist['users'][user] = userID

    #Create experiment directory
    expNum = len(datalist['experiments'])
    expDir = 'data%sexp%d' % (os.pathsep, expNu)
    imgDir = expDir + '%simg' % os.pathsep

    if not os.path.exist(expDir):
        os.makedirs(expDir, imgDir)

        #make meta file
        meta = {
        'timestamp':now,
        'user':user
        }
        with open(expDir+''+os.pathsep+'meta.json') as meta_file:
            json.dump(meta, meta_file)
        #empty for now
        info = {}
        with open(imgDir+''+os.pathsep+'data.json') as data_file:
            json.dump(info, data_file)

    params = {'imgDir':imgDir, 'expNum':expNum, 'expDir':expDir, 'interval':interval, 'user':user, 'userID':userID}
    nnInput, nnMeta, nnParams = MGDataLoader.CollectData(params, VERBOSITY, DEBUG)

    nnTrainingData, currentNetwork = MGNeuralNet.Train(MGNeuralNet.Basic(), nnInput.training, nnMeta, nnParams, VERBOSITY, DEBUG)
    nnTestData = MGNeuralNet.Test(currentNetwork, nnInput.testing, nnMeta, nnParams, VERBOSITY, DEBUG)
    MGDataLoader.SaveNetwork(nnTrainingData, nnTestData, currentNetwork)

    MGVisualizer.Analyze(nnTestData, VERBOSITY, DEBUG)

    SaveDataList(datalist)
#load data from data.json and pass to Visualizer
def inspect(exp):
    pass


#main loop
if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='WashU BCI MindGames Data Processor')
    parser.add_argument('--verbosity', '-v', help="increases output verbosity", action="store_true")
    parser.add_argument('--debug', '-d', help="display debug information", action = "store_true")
    parser.add_argument('--run', 'r', help="collect data and run it through the neural network", action="store_true")
    parser.add_argument('--user', '-d', help="user of the emotive headset")
    parser.add_argument('--runtime', help="how long to collect data (seconds)", type=int)
    parser.add_argument('--inspect', 'i', help="See Visualizer data for a given experiment")

    args = parser.parse_args()
    if args.verbose:
        print("verbosity turned on")
        VERBOSITY = True
    if args.debug:
        print("debug turned on")
        DEBUG = True

    if args.run:
        run(args.user, args.runtime)
    elif not args.inspect:
        inspect(args.inspect)
