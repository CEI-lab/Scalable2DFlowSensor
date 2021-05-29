# Created 02/26/21
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
ser = serial.Serial('COM13', 115200)
time.sleep(2)

#constants
trials = 10
thermistorToCalibrate = 3
samplesPerSecond = 10
numSamples = 600

#set up files
t = time.localtime()
filename = str(t.tm_mon) + str(t.tm_mday) + str(t.tm_year) + '_calibration_data_' + str(thermistorToCalibrate) + '_' + str(t.tm_hour) + '.' + str(t.tm_min) + '.' + str(t.tm_sec)  + '.csv'
# filename2 = str(t.tm_mon) + str(t.tm_mday) + str(t.tm_year) + '_calibration_summary_' + str(thermistorToCalibrate) + '_' + str(t.tm_hour) + '.' + str(t.tm_min) + '.' + str(t.tm_sec)  + '.csv'

#initialize the summary file
# summary = pd.DataFrame(None)
# summary.to_csv(path_or_buf=filename2, index=False)

#initialize data file
#dataframe has dimensions (250/increment) x 600 samples with column labels of f
df = pd.DataFrame(None)
df.to_csv(path_or_buf=filename, index=False)
#temporary list to hold values
sensorValues = []
#first column label
col = '/0'
#loop counter
global loop
loop = int(col[1:])


def readArduino():
    string = ''
    ser.reset_input_buffer()
    while(ser.in_waiting < 1):
        c = 0
    
    while(len(string) < 3 or string[0] == 0):
        b = ser.readline()
        string_n = b.decode()
        string = string_n.rstrip()
        # print(int(time.time() - start_time) ,repr(string), sep=": ")

    # mag, angle = string.split(",")
    addData(df, sensorValues, string)

def addData(df, sensorValues, string):
    #if the first character is a "/" then start a new column
    if(string.startswith('/')):
        newColumn(df, sensorValues, string)
        sensorValues.clear()

    else:
        sensorValues.append(int(string))
    print(string)

def newColumn(df, sensorValues, string):
    # print(sensorValues)
    #pull column out of string
    col = string
    #and save the value of that string in another variable loop
    global loop
    loop = int(col[1:])
    print("loop = " + str(loop))
    # print("trials = " + str(trials+20))
    #format list to match dataframe
    if(len(sensorValues)>numSamples):
        sensorValues = sensorValues[:numSamples]
    elif(len(sensorValues)<numSamples):
        sensorValues = sensorValues + ((numSamples-len(sensorValues))*[0])
    #add sensorValue to df with label in string
    df[("/" + str(50-(loop-10)))] = sensorValues
    #save data to file
    df.to_csv(path_or_buf=filename, index=False) 
    #wipe sensorValues
    # sensorValues = []

print("starting")


while(loop < 10):  #by 23 the fan turns off so anything below 20 is off
    readArduino()
    

print("finished read")


#printout file names for ease of access
print(filename)
# print(filename2)

#close serial line
ser.close()