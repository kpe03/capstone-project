

# Building Ventilation Software

## Project Description 
An automatic control system for a building's ventilation, where a software base platform will analyze several environmental factors (e.g. temperature, humidity, carbon monoxide levels) and decide whether to send a signal to turn the system on or off.

Some environmental conditions will have higher priority than others so our software will aim to maintain comfort as best as possible.

As of now we are uncertain of all the hardware that we will be working with but we for sure know that we will have Arduino sensors to track indoor and outdoor environments.

We have also begun discussing whether we would like to create a UI that would allow users to set their preferences. It could potentially be used to set the priority levels for each condition.

## Contributions and Practices
If contributing:
- Follow the coding style and conventions
- Write variable, method, and class names to be descriptive
- Write comments

## Branching and Merging
- For new features create a branch such as:
`git checkout -b feature-name`

## Requirements To Run Code
This code requires several different applications and software:

### Arduino IDE 2.3.4
#### Libraries
- Adafruit Unified Sensor by Adafruit
- DHT sensor library by Adafruit
- Mhz19 by Eduard Malokhvi

### PLX-DAQ
  - Download from: https://www.parallax.com/package/plx-daq/
  - This is a software for writing data from Arduino output to an excel file
  - Note: Windows ONLY 

## To Run Code
### Arduino
- Install the Arduino IDE
- Connect the Arduino via USB
- Upload the `Sound_Light_Temp_Humid.ino` file to the Arduino
- Use PLX-DAQ to write the data to an excel file 
- You can also view the outputs of the Arduino through `Serial Monitor` on the Arduino IDE, or by running the `read_serial.py` code

### GUI
- Install PySide6 using: `pip install pyside6`
- Configure Pathname to the file generated from PLX-DAQ (update variable `file_path` on line 26)
- To run the code use the command `python initalGUI.py`

## Technologies 
### Languages
C++
- Version: C++ 23
- Arduino language based on C++
  
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
- Light sensor
- Sound sensor
- CO2 sensor
- Smoke sensor

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