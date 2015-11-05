
# python version > = 2.5
"""
Taken from the Emotiv-provided emoStateLoger.py, but cleaned up and extended to
work on Mac OSX.
"""
import sys
import os
import time
import ctypes

from ctypes import *

if sys.platform.startswith('win32'):
    libEDK = cdll.LoadLibrary("edk.dll")
elif sys.platform.startswith('linux'):
    srcDir = os.getcwd()
    libPath = srcDir + "/libedk.so.1.0.0"
    libEDK = CDLL(libPath)
else:
    srcDir = os.getcwd()
    libPath = srcDir + "/libedk.1.0.0.dylib"
    libEDK = CDLL(libPath)

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

print ""
print "Example to show how to log EEG Data from EmoEngine/EmoComposer."
print ""
print "Press '1' to start and connect to the EmoEngine                    "
print "Press '2' to connect to the EmoComposer                            "
print ">> ",


#------------------------------------------------------------------------------

option = int(raw_input())
if option == 1:
    if libEDK.EE_EngineConnect("Emotiv Systems-5") != 0:
        print "Emotiv Engine start up failed."
elif option == 2:
    composerPort = c_uint(1726)
    if libEDK.EE_EngineRemoteConnect("127.0.0.1", composerPort) != 0:
        print "Cannot connect to EmoComposer on"
else:
    print "option = ?"

print "Start receiving EEG Data! Press any key to stop logging...\n"
f = open('ES.csv', 'w')
print >> f, header

userID = c_uint(9999)
eEvent = EE_EmoEngineEventCreate()
eState = EE_EmoStateCreate()
while 1:
    time.sleep(0.1)
    state = EE_EngineGetNextEvent(eEvent)
    if state == EDK_OK:
        eventType = EE_EmoEngineEventGetType(eEvent)
        EE_EmoEngineEventGetUserId(eEvent, pointer(userID))
        if eventType == EE_EmoStateUpdated:
            EE_EmoEngineEventGetEmoState(eEvent, eState)
            timestamp = ES_GetTimeFromStart(eState)
            print "%10.3f New EmoState from user %d ...\r" % \
                (timestamp, userID.value)
            logEmoState(userID,  eState)
    elif state != 0x0600:
        print "Internal error in Emotiv Engine!"

#------------------------------------------------------------------------------
libEDK.EE_EngineDisconnect()
libEDK.EE_EmoStateFree(eState)
libEDK.EE_EmoEngineEventFree(eEvent)
f.close()
