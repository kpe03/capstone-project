import serial

# an alternative method to reading the Serial output from the Arduino code
# (instead of using the PLX DAQ library which can only be used with Windows OS)

# set the port accordingly
def read_arduino_serial(port='COM9', baud_rate=9600):
    try:
        # Establish the connection
        # baud rate is the speed of data transmission
        with serial.Serial(port, baud_rate, timeout=1) as ser:
            print(f"Connected to {port} at {baud_rate} baud.")
            
            while True:
                if ser.in_waiting > 0:
                    line = ser.readline().decode('utf-8').rstrip()
                    print(line)

    # error checking
    except serial.SerialException as e:
        print(f"Error: {e}")
    except KeyboardInterrupt:
        print("Serial reading stopped.")

#main method
if __name__ == "__main__":
    read_arduino_serial()
