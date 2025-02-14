import time
import random # Simulate sensor data

comfort_zone = {
    "temperature_min": 20.0, # Temperature in celsius
    "temperature_max": 25.0,
    "humidity_min": 30.0, # Humidity in %
    "humidity_max": 60.0
}

# Mock function to read sensor data (replace with actual Arduino code)
def read_sensor_data():
    # Simulate reading indoor and outdoor temperature and humidity data
    indoor_temp = random.uniform(15, 30) # Example range
    indoor_humidity = random.uniform(10, 70)
    outdoor_temp = random.uniform(10, 35)
    outdoor_humidity = random.uniform(5, 80)
    
    return {
        "indoor": {"temperature": indoor_temp, "humidity": indoor_humidity},
        "outdoor": {"temperature": outdoor_temp, "humidity": outdoor_humidity}
    }

# Check if given temperature and humidity are within the comfort zone
def is_within_comfort_zone(temp, humidity):
    return (
        comfort_zone["temperature_min"] <= temp <= comfort_zone["temperature_max"] and
        comfort_zone["humidity_min"] <= humidity <= comfort_zone["humidity_max"]
    )

# Control algorithm to decide whether to open or close breathing window
def control_breathing_window(sensor_data):
    indoor = sensor_data["indoor"]
    outdoor = sensor_data["outdoor"]

    print(f"Indoor Conditions: {indoor['temperature']}°C, {indoor['humidity']}%")
    print(f"Outdoor Conditions: {outdoor['temperature']}°C, {outdoor['humidity']}%")

    if not is_within_comfort_zone(indoor["temperature"], indoor["humidity"]):
        if outdoor["temperature"] < indoor["temperature"] and outdoor["humidity"] < indoor["humidity"]:
            print("Opening window to improve indoor comfort.")
            apply_voltage_to_window(True)
        else:
            print("Keeping window closed")
            apply_voltage_to_window(False)
    else:
        print("Indoor conditions are comfortable. Keeping window closed")
        apply_voltage_to_window(False)

# Simulate applying or removing voltage to control the window
def apply_voltage_to_window(state):
    if state:
        print("Voltage applied: Window is OPEN")
    else:
        print("No voltage: Window is CLOSED")

def main():
    print("Starting Environment Control System...")
    while True:
        sensor_data = read_sensor_data()
        control_breathing_window(sensor_data)
        time.sleep(5) # 5 second latency before the next check

if __name__ == "__main__":
    main()
