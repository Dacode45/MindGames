import sys,os
import time
import ctypes
from ctypes import *
import threading
import logging

import MGUtil

import datetime

VERBOSITY = False
DEBUG = False
GLOBAL_PARAMS = {}

class

try:
    if sys.platform.startswith('win32'):
        libEDK = cdll.LoadLibrary("edk.dll")
    if sys.platform.startswith('linux'):
        srcDir = os.getcwd()
        libPath = srcDir = "/libedk.so.1.0.0"
        libEDK = CDLL(libPath)
except:
    raise MGHeadSetError('Cannot load edk dll/lib')

#------------------------------------------------------------------------------
EE_EmoEngineEventCreate = libEDK.EE_EmoEngineEventCreate
EE_EmoEngineEventCreate.restype = c_void_p

EE_EmoEngineEventGetEmoState = libEDK.EE_EmoEngineEventGetEmoState
EE_EmoEngineEventGetEmoState.argtypes = [c_void_p, c_void_p]
EE_EmoEngineEventGetEmoState.restype = None

EE_EmoEngineEventGetUserId = libEDK.EE_EmoEngineEventGetUserId
EE_EmoEngineEventGetUserId.argtypes = [c_void_p, POINTER(c_uint)]
EE_EmoEngineEventGetUserId.restype = None

EE_EmoEngineEventGetType = libEDK.EE_EmoEngineEventGetType
EE_EmoEngineEventGetType.argtypes = [c_void_p]
EE_EmoEngineEventGetType.restype = c_int

EE_EngineGetNextEvent = libEDK.EE_EngineGetNextEvent
EE_EngineGetNextEvent.argtypes = [c_void_p]
EE_EngineGetNextEvent.restype = c_int

ES_GetTimeFromStart = libEDK.ES_GetTimeFromStart
ES_GetTimeFromStart.argtypes = [ctypes.c_void_p]
ES_GetTimeFromStart.restype = c_float

EE_EmoStateCreate = libEDK.EE_EmoStateCreate
EE_EmoStateCreate.restype = c_void_p

ES_GetWirelessSignalStatus = libEDK.ES_GetWirelessSignalStatus
ES_GetWirelessSignalStatus.restype = c_int
ES_GetWirelessSignalStatus.argtypes = [c_void_p]

ES_ExpressivIsBlink = libEDK.ES_ExpressivIsBlink
ES_ExpressivIsBlink.restype = c_int
ES_ExpressivIsBlink.argtypes = [c_void_p]

ES_ExpressivIsLeftWink = libEDK.ES_ExpressivIsLeftWink
ES_ExpressivIsLeftWink.restype = c_int
ES_ExpressivIsLeftWink.argtypes = [c_void_p]

ES_ExpressivIsRightWink = libEDK.ES_ExpressivIsRightWink
ES_ExpressivIsRightWink.restype = c_int
ES_ExpressivIsRightWink.argtypes = [c_void_p]

ES_ExpressivIsLookingLeft = libEDK.ES_ExpressivIsLookingLeft
ES_ExpressivIsLookingLeft.restype = c_int
ES_ExpressivIsLookingLeft.argtypes = [c_void_p]

ES_ExpressivIsLookingRight = libEDK.ES_ExpressivIsLookingRight
ES_ExpressivIsLookingRight.restype = c_int
ES_ExpressivIsLookingRight.argtypes = [c_void_p]

ES_ExpressivGetUpperFaceAction = libEDK.ES_ExpressivGetUpperFaceAction
ES_ExpressivGetUpperFaceAction.restype = c_int
ES_ExpressivGetUpperFaceAction.argtypes = [c_void_p]

ES_ExpressivGetUpperFaceActionPower = libEDK.ES_ExpressivGetUpperFaceActionPower
ES_ExpressivGetUpperFaceActionPower.restype = c_float
ES_ExpressivGetUpperFaceActionPower.argtypes = [c_void_p]

ES_ExpressivGetLowerFaceAction = libEDK.ES_ExpressivGetLowerFaceAction
ES_ExpressivGetLowerFaceAction.restype = c_int
ES_ExpressivGetLowerFaceAction.argtypes = [c_void_p]

ES_ExpressivGetLowerFaceActionPower = libEDK.ES_ExpressivGetLowerFaceActionPower
ES_ExpressivGetLowerFaceActionPower.restype = c_float
ES_ExpressivGetLowerFaceActionPower.argtypes = [c_void_p]

ES_AffectivGetExcitementShortTermScore = libEDK.ES_AffectivGetExcitementShortTermScore
ES_AffectivGetExcitementShortTermScore.restype = c_float
ES_AffectivGetExcitementShortTermScore.argtypes = [c_void_p]

ES_AffectivGetExcitementLongTermScore = libEDK.ES_AffectivGetExcitementLongTermScore
ES_AffectivGetExcitementLongTermScore.restype = c_float
ES_AffectivGetExcitementLongTermScore.argtypes = [c_void_p]


ES_AffectivGetEngagementBoredomScore = libEDK.ES_AffectivGetEngagementBoredomScore
ES_AffectivGetEngagementBoredomScore.restype = c_float
ES_AffectivGetEngagementBoredomScore.argtypes = [c_void_p]

ES_CognitivGetCurrentAction = libEDK.ES_CognitivGetCurrentAction
ES_CognitivGetCurrentAction.restype = c_int
ES_CognitivGetCurrentAction.argtypes = [c_void_p]

ES_CognitivGetCurrentActionPower = libEDK.ES_CognitivGetCurrentActionPower
ES_CognitivGetCurrentActionPower.restype = c_float
ES_CognitivGetCurrentActionPower.argtypes = [c_void_p]

EE_EmoStateUpdated = 64  # libEDK.EE_Event_enum.EE_EmoStateUpdated
EDK_OK = 0

#-----------------------------------------------------------------------------------------------------------------------------------------------------------------

def logEmoState(userID, eState):
    print >>f, ES_GetTimeFromStart(eState), ", ",
    print >>f, userID.value, ", ",
    print >>f, ES_GetWirelessSignalStatus(eState), ", ",
    print >>f, ES_ExpressivIsBlink(eState), ", ",
    print >>f, ES_ExpressivIsLeftWink(eState), ", ",
    print >>f, ES_ExpressivIsRightWink(eState), ", ",

    print >>f, ES_ExpressivIsLookingLeft(eState), ", ",
    print >>f, ES_ExpressivIsLookingRight(eState), ", ",

    expressivStates = {}
    expressivStates[EXP_EYEBROW] = 0
    expressivStates[EXP_FURROW] = 0
    expressivStates[EXP_SMILE] = 0
    expressivStates[EXP_CLENCH] = 0
    expressivStates[EXP_SMIRK_LEFT] = 0
    expressivStates[EXP_SMIRK_RIGHT] = 0
    expressivStates[EXP_LAUGH] = 0
    upperFaceAction = ES_ExpressivGetUpperFaceAction(eState)
    upperFacePower = ES_ExpressivGetUpperFaceActionPower(eState)
    lowerFaceAction = ES_ExpressivGetLowerFaceAction(eState)
    lowerFacePower = ES_ExpressivGetLowerFaceActionPower(eState)
    expressivStates[upperFaceAction] = upperFacePower
    expressivStates[lowerFaceAction] = lowerFacePower
    print >>f, expressivStates[EXP_EYEBROW], ", ",
    print >>f, expressivStates[EXP_FURROW], ", ",
    print >>f, expressivStates[EXP_SMILE], ", ",
    print >>f, expressivStates[EXP_CLENCH], ", ",
    print >>f, expressivStates[EXP_SMIRK_LEFT], ", ",
    print >>f, expressivStates[EXP_SMIRK_RIGHT], ", ",
    print >>f, expressivStates[EXP_LAUGH], ", ",

    # Affectiv Suite results
    print >>f, ES_AffectivGetExcitementShortTermScore(eState), ", ",
    print >>f, ES_AffectivGetExcitementLongTermScore(eState), ", ",

    print >>f, ES_AffectivGetEngagementBoredomScore(eState), ", ",

    # Cognitiv Suite results
    print >>f, ES_CognitivGetCurrentAction(eState), ", ",
    print >>f, ES_CognitivGetCurrentActionPower(eState)

#-------------------------------------------------------------------------------

EXP_EYEBROW = 0x0020      # eyebrow
EXP_FURROW = 0x0040       # furrow
EXP_SMILE = 0x0080        # smile
EXP_CLENCH = 0x0100       # clench
EXP_SMIRK_LEFT = 0x0400   # smirk left
EXP_SMIRK_RIGHT = 0x0800  # smirk right
EXP_LAUGH = 0x0200        # laugh

#------------------------------------------------------------------------------
header = ['Time', 'UserID', 'Wireless Signal Status', 'Blink', 'Wink Left',
          'Wink Right', 'Look Left', 'Look Right', 'Eyebrow', 'Furrow',
          'Smile', 'Clench', 'Smirk Left', 'Smirk Right', 'Laugh',
          'Short Term Excitement', 'Long Term Excitement',
          'Engagement/Boredom', 'Cognitiv Action', 'Cognitiv Power']
#------------------------------------------------------------------------------
USER_ID = 0

def Setup(params, verb, deb):
    GLOBAL_PARAMS = params
    VERBOSITY = verb
    DEBUG = deb

    if libEDK.EE_EngineConnect("Emotiv Systems-5") != 0:
        raise MGHeadSetError("Emotive Engine start up failed.")
    USER_ID = c_uint(params['userID'])

STOP_COLLECTION = False

EMO_DATA = []

def Collector():
    eEvent = EE_EmoEngineEventCreate()
    eState = EE_EmoStateCreate()
    while not STOP_COLLECTION:
        time.sleep(.1)
        state = EE_EngineGetNextEvent(eEvent)
        EE_EmoEngineEventGetUserId(eEvent, pointer(userID))
        if eventType == EE_EmoStateUpdated:
            EE_EmoEngineEventGetEmoState(eEvent, eState)
            timestamp = ES_GetTimeFromStart(eState)
            if VERBOSITY or DEBUG:
                logging.Info("%10.3f New EmoState from user %d ...\r" % (timestamp, USER_ID)
            if DEBUG:
                logEmoState(USER_ID, eState)
            EMO_DATA.append({'userID':USER_ID, 'eState':eState, 'timestamp':timestamp})
        elif state != 0x600:
            print "Internal error in Emotive Engine"
#special keys


def StartLog():
    t = threading.Thread(target=Collector)
    t.start()

def StopLog():
    STOP_COLLECTION = True
    libEDK.EE_EngineDisconnect()
    libEDK.EE_EmoStateFree(eState)
    libEDK.EE_EmoEngineEventFree(eEvent)
    return (KEY_DATA, IMG_DATA)
