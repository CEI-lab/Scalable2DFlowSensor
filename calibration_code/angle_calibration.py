# Created 03/18/21
# By Jack A. Defay
#
# This code is published under the MIT Open Source Liscense



import serial
import time
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

start_time = time.time()

#set up the serial line
ser = serial.Serial('COM12', 115200)
time.sleep(2)

#constants
increment = 17
samplesPerSecond = 10
numSamples = 1200

#set up files
t = time.localtime()
filename = str(t.tm_mon) + str(t.tm_mday) + str(t.tm_year) + '_angle_calibration_data_' + '_30deg_' + str(t.tm_hour) + '.' + str(t.tm_min) + '.' + str(t.tm_sec)  + '.csv'

#initialize data file
#dataframe has dimensions (250/increment) x 600 samples with column labels of f
df = pd.DataFrame(None)
df.to_csv(path_or_buf=filename, index=False)
df = pd.DataFrame([["Time", "Steps", "Vertical", "Horizontal"]])


def readArduino(df):
    string = ''
    ser.reset_input_buffer()
    while(ser.in_waiting < 1):
        c = 0
    
    while(len(string) < 7):
        b = ser.readline()
        string_n = b.decode()
        string = string_n.rstrip()

    try:
        s, v, h = string.split(",")
    except:
        s=-100
        v=-100
        h=-100
    return addData(df, s, v, h)

def addData(df, steps=0, vert=0, horiz=0):
    sampleTime = time.time() - start_time
    frame = pd.DataFrame([[sampleTime, steps, vert, horiz]])
    print(frame)
    return pd.concat([df, frame], axis=0)


while(True):
    try:
        df = readArduino(df)
        if(df.shape[0]%100 == 0):
            df.to_csv(path_or_buf=filename, index=False, header=False)
            # df = pd.DataFrame(None)
    except KeyboardInterrupt:
        df.to_csv(path_or_buf=filename, index=False, header=False)
        print(df)
        print(filename)
        raise


#printout file names for ease of access
print(filename)

#close serial line
ser.close()