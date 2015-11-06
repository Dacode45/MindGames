import sys,os
import threading
import logging

from msvcrt import getch
import wx

import datetime
import time

VERBOSITY = False
DEBUG = False
GLOBAL_PARAMS = {}

SCREEN = nil
SCREEN_SIZE = []

def Setup(params, verb, deb):
    GLOBAL_PARAMS = params
    VERBOSITY = verb
    DEBUG = deb

    wx.App()
    SCREEN = wx.screenDC()
    SCREEN_SIZE = screen.GetSize()

STOP_COLLECTION = False

KEY_DATA = []
IMG_DATA = []
#special keys
def Collector():
    while not STOP_COLLECTION:
        key = ord(getch())
        timestamp = datetime.datetime.now()
        #need to get keyboard input
        keyData = {'key':key, 'timestamp':time}
        if VERBOSITY:
            logging.info('[%s] key:%s, timestamp:%s'%(__name__, key, timestamp))
        KEY_DATA.append({'key':key, 'timestamp':timestamp})

        #get screenshot
        bmp = wx.EmptyBitmap(size[0], size[1])
        mem = wx.MemoryDC(bmp)
        mem.Blit(0, 0, size[0], size[1], 0, 0)
        del mem
        imgFile = GLOBAL_PARAMS.imgDir + os.pathsep + "%d.png"%(len(IMG_DATA))
        bmp.SaveFile(imgFile, wx.BITMAP_TYPE_PNG)
        if DEBUG:
            saveTime = datetime.datetime.now()
            logging.debug('[%s] Saving File Took %s'%(__name__, timestamp - saveTime))
        IMG_DATA.append({'img':imgFile, 'timestamp':timestamp})

def StartLog():
    t = threading.Thread(target=Collector)
    t.start()

def StopLog():
    STOP_COLLECTION = True
    time.sleep(.1)
    return (KEY_DATA, IMG_DATA)
