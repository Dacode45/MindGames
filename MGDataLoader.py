import logging

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
    keyData, imgData = MGKeyLogger.StopLog()
    emotiveStateData, emotiveRawData = MGEmotiveLogger.StopLog()

    nnInput, meta, params = formatData(keyData, imgData, emotiveRawData, emotiveStateData)
    saveData(nnInput, meta, params)
    return (nnInput, meta, nn_params)

#format key data
#todo get ascii mappings for bittrip
#scan codes retrieved from here: https://msdn.microsoft.com/en-us/library/aa299374%28VS.60%29.aspx
#order is scancode, ascii
MAPPINGS = {'BIT_TRIP':{
    'w':[17, 119, 87, 23],
    'a':[30, 97, 65, 1],
    's':[31, 115, 83, 19],
    'space':[57, 32]}
}
def formatKeyData(rawKeyData):
    mapping = MAPPINGS[GLOBAL_PARAMS['game']]
    mappingSize = len(mapping)

    for i in range(0, rawKeyData):
        gameMapping = []
        rawKeyData[i]['game'] = []
        for k, v in kData.iteritems():
            if rawKeyData[i]["key"] in v:
                rawKeyData[i]['game'].append(1)
            else
                rawKeyData[i]['game'].append(0)
        if DEBUG:
            logging.debug("[%s] Convert key:%d to prob vector:%s"%(__name__, rawKeyData[i]["key"], rawKeyData[i]["game"]))

#synch data for format data funciton
def synchData(rawKeyData, imgData, emotiveRawData, emotiveStateData):
    formatKeyData(rawKeyData)

    synched = []

    i = 0 #key index
    j = 0 #emotive raw index
    k = 0 #emotive state index
    l = 0 #img index
    maxLen = max(len(keyData), len(emotiveRawData), len(emotiveStateData))
    for currentIndex in range(0, maxLen):
        kTime = keyData[i]['timestamp']
        iTime = imgData[l]['timestamp']
        erTime = emotiveRawData[j]['timestamp']
        esTime = emotiveStateData[k]['timestamp']
        maxTime = max(kTime, erTime, esTime)

        synched.append({'raw':emotiveRawData[j], 'es':emotiveStateData[k], 'key':keyData[i], 'img':imgData[l]})
        if kTime <= maxTime:
            i++
        if erTime <= maxTime:
            j++
        if esTime <= maxTime:
            k++
        if iTime <= maxTime:
            l++

    if DEBUG:
        logging.debug('[%s] Synched Data: %s'%(__name__, synched))

    return synched
#How many samples per iteration to feed into neural network
#Currently decided to have as many that fit in .3 seconds
SAMPLE_PER_SET = 39
#Percent of data that is training
PERCENT_TRAINING = .8

channels = ["AF3", "F7", "F3", "FC5", "T7", "P7", "O1", "O2", "P8", "T8", "FC6", "F4", "F8", "AF4"]
#format data so that emotive data and keyboard input is synced
def formatData(keyData, imgData, emotiveRawData, emotiveStateData):
    #assume synched by samples
    synched = synchData(keyData, imgData, emotiveRawData, emotiveStateData)
    data = []
    for i in range (0, len(synched), SAMPLE_PER_SET):
        data.append(Synched[i:i+SAMPLE_PER_SET])
    #Currently prefer raw data to emotive state data
    nnData = {"testing":([],[]), "training":([],[])}
    meta = {}
    params = {}
    trainingCutOff = PERCENT_TRAINING * len(data)
    #trX, teX, trY, teY, meta = loadDataset()
    #header for channels
    for i in range(0, len(data)):
        x = []
        y = []
        sample = data[i]
        for j in range(0, len(sample))
            for k in range(0, channels)
                x.append(sample[j]['raw'][channels[k]])
        y.append(sample[j]['key']['game'])
        if i < trainingCutOff:
            nnData["training"][0].append(x)
            nnData["training"][1].append(y)
        else:
            nnData["testing"][0].append(x)
            nnData["testing"][1].append(y)

    return (nnData, nil, params)
    pass

#save data collected incase we need to redo
def saveData(nnInput, meta, params):
    
    pass

#save neuralnetwork to data.json
def SaveNetwork(nnTrainingData, nnTestData, currentNetwork):
    pass
