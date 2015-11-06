import MGKeyLogger
import MGEmotiveLogger


VERBOSITY = False
DEBUG = False
GLOBAL_PARAMS = {}

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
    emotiveStateData, emotiveRawData = MGEmotiveLogger.StopLog()

    nnInput, meta, params = formatData(keyData, emotiveRawData, emotiveStateData)
    saveData(nnInput, meta, params)
    return (input, meta, nn_params)

#format data so that emotive data and keyboard input is synced
def formatData(keyData, emotiveRawData):
    synched = []
    meta = {"img":[]}
    k = 0
    for i in range(0, len(keyData)):
        kTimeStart  = keyData[i]['timestamp']
        kTimeEnd = keyData[i+1]['timestamp']
    for k in range(k, len(emotiveRawData)):
        eTime = emotiveData[k]['timestamp']
        if eTime >= kTimeStart and eTime < kTimeEnd
            synched = {}
        else
            break

    return (nil, nil, params)
    pass

#save data collected incase we need to redo
def saveData(nnInput, meta, params):
    pass

#save neuralnetwork to data.json
def SaveNetwork(nnTrainingData, nnTestData, currentNetwork):
    pass
