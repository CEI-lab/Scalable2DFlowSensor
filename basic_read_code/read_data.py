# Created 05/30/21
# By Jack A. Defay
#
# Code for reading the magnitude and angle data from the sensor as raw analog outputs.
#
# This code is published under the MIT Open Source Liscense

import numpy as np
import pandas as pd
import serial
import time


#setup time
start_time = time.time()

#set save file name
t = time.localtime()
filename = str(t.tm_mon) + str(t.tm_mday) + str(t.tm_year) + '_experiment_' + str(t.tm_hour) + '.' + str(t.tm_min) + '.' + str(t.tm_sec)  + '.csv'

#setup dataframe
df = pd.DataFrame([[start_time,1,2,3,4,5,6,7]], columns=['time', 'mag', 'North', 'East', 'South', 'West'])
df.to_csv(path_or_buf=filename, mode='a', index=False)
df = pd.DataFrame(None)

#setup the serial line
ser = serial.Serial('COM9', 115200)
time.sleep(2)

def readArduino(df):
    string = ''
    ser.reset_input_buffer()
    while(ser.in_waiting < 1):
        c=0
    
    while(len(string) < 13):
        b = ser.readline()
        string_n = b.decode()
        string = string_n.rstrip()
        # print(int(time.time() - start_time) ,repr(string), sep=": ")

    stringArray = []
    stringArray = string.split(", ")
    magnitudeArray = []
    for cell in stringArray:
        # print(cell)
        magnitudeArray.append(int(cell))

    return addData(df, [magnitudeArray])

def addData(df, magnitudeArray):
    magnitudeArray[0].insert(0, (time.time() - start_time))
    if(not len(magnitudeArray[0]) == 6):
        magnitudeArray = [[0,0,0,0,0,0]]
    frame = pd.DataFrame(magnitudeArray, columns=['time', 'mag', 'North', 'East', 'South', 'West'])
    print(frame)
    return pd.concat([df, frame], axis=0)

while(True):
    try:
        df = readArduino(df)
        if(df.shape[0]%100 == 0):
            df.to_csv(path_or_buf=filename, mode='a', index=False, header=False)
            df = pd.DataFrame(None)
    except KeyboardInterrupt:
        df.to_csv(path_or_buf=filename, mode='a', index=False, header=False)
        print(df)
        print(filename)
        raise