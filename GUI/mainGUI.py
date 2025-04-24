# main.py
import sys
import pandas as pd
import json
import os

from PySide6.QtCore import QTimer, QDateTime
from PySide6.QtWidgets import QApplication, QMainWindow

# import the auto-generated UI class
from ui_updatedgui import Ui_MainWindow

class MainWindow(QMainWindow):

    def __init__(self):
        super().__init__()
        # 1) instantiate & hook up the designer UI
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        # 2) “model” data
        self.file_path = "./arduino_code/arduino_data.csv"
        self.Indoor = {
            "Temperature": 22,
            "Humidity": 45,
            "Light": 200,
            "Sound": 50,
            "CO2": 0,
            "Smoke": 0
        }
        self.Outdoor = {
            "Temperature": 30,
            "Humidity": 70,
            "Light": 500,
            "Sound": 70
        }

        # 3) timers
        # 3a) clock
        self.time_timer = QTimer(self)
        self.time_timer.timeout.connect(self._update_clock)
        self.time_timer.start(1000)

        # 3b) data refresh
        self.data_timer = QTimer(self)
        self.data_timer.timeout.connect(self.update_data)
        self.data_timer.start(60_000)            # every minute
        QTimer.singleShot(0, self.update_data)   # initial fill

        # 4) button/page wiring
        self.ui.ModifyButt.clicked.connect(self.switch_to_modify)
        self.ui.CancelModButt.clicked.connect(self.switch_to_main)
        self.ui.DoneModButt.clicked.connect(self.apply_modifications)

        # 5) initial UI state
        self.switch_to_main()
        self.update_labels()

    # —————————————————————————————
    def _update_clock(self):
        now = QDateTime.currentDateTime().toString("hh:mm AP")
        self.ui.CurrTime.setText(f"Current Time: {now}")

    def update_data(self):
        try:
            df = pd.read_csv(self.file_path).dropna(how="all")
            latest = df.iloc[-1]
            self.Indoor["Temperature"]  = latest["T-in"]
            self.Indoor["Humidity"]     = latest["H-in"]
            self.Indoor["CO2"]          = latest["CO2"]
            self.Indoor["Smoke"]        = latest["Smoke"]
            self.Outdoor["Temperature"] = latest["T-out"]
            self.Outdoor["Humidity"]    = latest["H-out"]
        except Exception as e:
            print("Error reading data:", e)
        finally:
            self.update_labels()

    def update_labels(self):
        self.ui.InTemp.setText(f"{self.Indoor['Temperature']} °C")
        self.ui.InHumid.setText(f"{self.Indoor['Humidity']}%")
        self.ui.InLight.setText(f"{self.Indoor['Light']} lumen")
        self.ui.InSound.setText(f"{self.Indoor['Sound']} dB")
        self.ui.CO2  .setText(f"{self.Indoor['CO2']} ppm")
        self.ui.Smoke.setText(f"{self.Indoor['Smoke']} ppm")

        self.ui.OuTemp.setText(f"{self.Outdoor['Temperature']} °C")
        self.ui.OutHumid.setText(f"{self.Outdoor['Humidity']}%")
        self.ui.OutLight.setText(f"{self.Outdoor['Light']} lumen")
        self.ui.label_14.setText(f"{self.Outdoor['Sound']} dB")  # or `self.ui.OutSound`

    def switch_to_modify(self):
        # Load the current indoor values
        self.ui.ModTemp .setText(f"{self.Indoor['Temperature']} °C")
        self.ui.ModHumid.setText(f"{self.Indoor['Humidity']}%")
        # Switch the page
        self.ui.stackedWidget.setCurrentWidget(self.ui.ModifyPage)

    def switch_to_main(self):
        self.ui.stackedWidget.setCurrentWidget(self.ui.MainPage)

    def apply_modifications(self):
        # read values from the spin-boxes
        TempFrom = self.ui.TempModifyFrom.value()
        TempTo = self.ui.TempModifyTo.value()
        HumidFrom = self.ui.HumidModifyFrom.value()
        HumidTo = self.ui.HumidModifyTo.value()

        self.switch_to_main()

# —————————————————————————————

if __name__ == "__main__":
    app = QApplication(sys.argv)
    w = MainWindow()
    w.show()
    sys.exit(app.exec())
