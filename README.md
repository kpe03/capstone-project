

# Building Ventilation Software

## Project Description 
An automatic control system for a building's ventilation, where a software base platform will analyze several environmental factors (e.g. temperature, humidity, carbon monoxide levels) and decide whether to send a signal to turn the system on or off.

Some environmental conditions will have higher priority than others so our software will aim to maintain comfort as best as possible.

As of now we are uncertain of all the hardware that we will be working with but we for sure know that we will have Arduino sensors to track indoor and outdoor environments.

We have also begun discussing whether we would like to create a UI that would allow users to set their preferences. It could potentially be used to set the priority levels for each condition.

## Requirements To Run Code
This code requires several different applications and software:
### Arduino IDE 2.3.4
#### Libraries
- Adafruit Unified Sensor by Adafruit
- DHT sensor library by Adafruit
- Mhz19 by Eduard Malokhvi

### PLX-DAQ
  - https://www.parallax.com/package/plx-daq/
  - For writing to a .csv
  - Windows ONLY, use the python script `read_serial.py` as an alternative

## To Run Code
### Arduino
- Install the Arduino IDE
- Connect the Arduino
- Upload the `Sound_Light_Temp_Humid.ino`
- Open 'Serial Monitor', PLX DAQ, or the python script `read_serial.py` to output the results

### GUI
- Install PySide6 using: pip install pyside6
- Run the python code 'initalGUI.py'
- Configure Pathname to the file generated from PLX-DAQ

## Technologies 
### Languages
C++
- Version: C++ 23
- Arduino language based on C++
  
Java
- Version: Java 23
- Swing or JavaFX
  
Python
- Version: Python 3.12.0
- PySide 6

### Environment
Arduino IDE
- Version: 2.3.4

### Hardware
Arduino
- Model: unknown
- Temperature and Humidity sensor
- TBD

## Team Roles
Product Owner: Mason
Quality Assurance: David 
Scrum Master 1: Kaitlyn
Scrum Master 2: Landon
Scrum Master 3: Kazi
Scrum Master 4: Gerry

## Group Goals / Future Plans
- We are planning to see what technology is readily available so we can find a starting point for the software.
- Further discussions regarding UI implementation.
- We want to figure out how to optimize comfort first and then a plan to optimize energy if feasible.
