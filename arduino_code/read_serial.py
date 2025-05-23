# This python code is an alternative to using PLX DAQ
# this code prints: time, temperature, humidity, co2 and smoke levels to a csv 'arduino_data.csv'

# change the port if necessary for the arduino
import serial
import csv
import time
from datetime import datetime
import os


# Set the port accordingly
def read_arduino_serial(port='COM9', baud_rate=9600, csv_filename='arduino_data.csv'):
    try:
        # Establish the connection
        # baud rate is the speed of data transmission
        with serial.Serial(port, baud_rate, timeout=1) as ser:
            print(f"Connected to {port} at {baud_rate} baud.")
            print(f"Writing data to {csv_filename}")
            
            # Open CSV file for writing
            with open(csv_filename, 'w', newline='') as csv_file:
                csv_writer = csv.writer(csv_file)
                
                # Write header for the specific data columns
                csv_writer.writerow(['Time', 'T-in', 'H-in', 'T-out', 'H-out', 'CO2', 'Smoke'])
                
                while True:
                    if ser.in_waiting > 0:
                        # Read line from Arduino
                        line = ser.readline().decode('utf-8').rstrip()
                        
                        # Get current timestamp
                        timestamp = datetime.now().strftime('%Y-%m-%d %H:%M')
                        try:
                            # Split the incoming data
                            data_parts = line.split(',')
                            
                            # Check if we have the expected number of values
                            # ignore the DATA and TIME (this is a part of PLX formatting)
                            if len(data_parts) >= 6 and data_parts[0] == "DATA" and data_parts[1] == "TIME":
                                # Extract the actual sensor values (skipping DATA,TIME)
                                temperature = data_parts[2]
                                humidity = data_parts[3]
                                temperature2 = data_parts[4]
                                humidity2 = data_parts[5]
                                # co2 = data_parts[7]
                                # smoke = data_parts[8]

                                co2 = 0
                                smoke = 0
                                
                                # Print to console
                                print(f"{timestamp}: Temp In={temperature}, Humidity In={humidity}, Temp Out={temperature2}, Humidity Out={humidity2}, CO2={co2}, Smoke={smoke}")
                                

                                # check if update text file exists
                                # (instead of the original function) this block of code needs to be within an open serial connection
                                # since you can only be have one connection at a time
                                filename = "../update.txt"
                                if os.path.exists(filename):
                                    # read the update file
                                    with open(filename, 'r') as command_file:
                                        command = command_file.read()
                                        array = command.split()
                                        
                                    # try to read update.txt
                                    arduino_variable = array[0]
                                    new_value = array[1]

                                    command = f'{arduino_variable}={new_value}\n' # changes value of desired variable
                                    ser.write(command.encode()) # serial.write expects a binary string
                                
                                # Write to CSV
                                csv_writer.writerow([timestamp, temperature, humidity, temperature2, humidity2, co2, smoke])
                            # if not data, just print the text
                            else:
                                print(f"{line}")
                        ## error parsing the data
                        except Exception as e:
                            print(f"Error parsing data '{line}': {e}")
                            # Write timestamp and mark other fields as error
                            csv_writer.writerow([timestamp, "ERROR", "ERROR", "ERROR", "ERROR"])
                        
                        # Flush to ensure data is written to the file
                        csv_file.flush()
    
    # Error checking
    except serial.SerialException as e:
        print(f"Error: {e}")
    except KeyboardInterrupt:
        print("Serial reading stopped.")
    except Exception as e:
        print(f"Unexpected error: {e}")

# Main method
if __name__ == "__main__":
    # read_arduino_serial(port='COM9', 
    #                    baud_rate=9600, 
    #                    csv_filename='arduino_data.csv')
    
    # COM9 is a windows port, mac uses the following:
    read_arduino_serial(port='/dev/cu.usbserial-1140', 
                       baud_rate=9600, 
                       csv_filename='arduino_data.csv')