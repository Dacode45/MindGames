import MGKeyLogger
import MGEmotiveLogger


VERBOSITY = False
DEBUG = False
GLOBAL_PARAMS

def CollectData(params, verb, deb):
    VERBOSITY = verb
    DEBUG = deb
    GLOBAL_PARAMS = params

    MGKeyLogger.setup(params, VERBOSITY, DEBUG)
    MGEmotiveLogger.setup(params, VERBOSITY, DEBUG)

    MGKeyLogger.StartLog()
    MGEmotiveLogger.StartLog()
    if not interval:
        raw_input('Enter Any Key to stop collectiong Data')
    else:
        time.sleep(params['interval'])
    keyData = MGKeyLogger.StopLog()
    emotiveData = MGEmotiveLogger.StopLog()

    nnInput, meta, params = formatData(keyData, emotiveData)
    saveData(nnInput, meta, params)
    return (input, meta, nn_params)

#format data so that emotive data and keyboard input is synced
def formatData():
    return (nil, nil, params)
    pass

#save data collected incase we need to redo
def saveData(nnInput, meta, params):
    pass

#save neuralnetwork to data.json
def SaveNetwork(nnTrainingData, nnTestData, currentNetwork):
    pass