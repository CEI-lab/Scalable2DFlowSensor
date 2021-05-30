# Scalable2DFlowSensor

Repository for paper, A Customizable, Low-Cost Alternative for Distributed 2D Flow Sensing in Swarms, published in SWARMS2021 conference. 

This is an implementation of a 2D thermal flow sensor that uses a PCB with central heating element and 5 surrounding thermistors to sense both magnitude and direction of flow along a surface. See paper for details https://cei.ece.cornell.edu/publications/

This work is published for the purpose of helping other groups design their own thermal flow sensors. Details for use:

The calibration code folder contains the python and arduino code used in the calibration of the sensor. The sensor was designed to be read from any microcontroller with an analog read, we chose to use an arduino. The python code is then run from a computer connected over usb to the device to read data from the sensor. The purpose of this calibration is to find the proper offset values for accurate reading of direction and magnitude.

The field_test_code folder contains the python and arduino files used to read data from the sensor in the field. These files are quite simple because a key goal of the sensor was a simple interface. Even the arduino is overkill for this application. The python file here periodically saves the read data to a csv file with a unique generated file name. The test is concluded by interrupting the script with a keyboard interrupt which is caught and saves the remaining data to the file before terminating.

The other three folders flowSensorFlexible, flowSensorV2, and magArraySensor contain the PCB design files for the 3 versions of the sensor discussed. These files were created in Altium and can be used as is to order your own PCBs or can serve as inspiration to design your own geometries. These designs are quite simple, so recreating them in another program or to different specificiation should be straigh forward.

Published under the MIT Open Source Liscense.

Maintained by Jack A. Defay (user: jackdefay).