# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'InitialGUI.ui'
##
## Created by: Qt User Interface Compiler version 6.8.2
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt, QTimer)
from PySide6.QtGui import (QAction, QBrush, QColor, QConicalGradient,
    QCursor, QFont, QFontDatabase, QGradient,
    QIcon, QImage, QKeySequence, QLinearGradient,
    QPainter, QPalette, QPixmap, QRadialGradient,
    QTransform)
from PySide6.QtWidgets import (QApplication, QDoubleSpinBox, QFrame, QLabel,
    QMainWindow, QMenu, QMenuBar, QPushButton,
    QSizePolicy, QStackedWidget, QStatusBar, QWidget)
import pandas as pd

class Ui_MainWindow(object):

    file_path = r"C:\Users\kaitl\Desktop\PLX-DAQ\plx-daq-release2b\plx-daq-release2b\PLX_temp_hum.xlsm"

    DefaultTemp = 25
    DefaultHumid = 40

    # Dictionary for Indoor
    Indoor = {
        "Temperature": 22,
        "Humidity": 45,
        "Light": 200,
        "Sound": 50
    }

    Outdoor = {
        "Temperature": 30,
        "Humidity": 70,
        "Light": 500,
        "Sound": 70
    }

    Modify = {
        "Temperature": DefaultTemp,
        "Humidity": DefaultHumid
    }

    


    def setupUi(self, MainWindow):
        if not MainWindow.objectName():
            MainWindow.setObjectName(u"MainWindow")
        MainWindow.resize(798, 513)
        self.actionNew = QAction(MainWindow)

        # Tabs
        self.actionNew.setObjectName(u"actionNew")
        self.actionSave = QAction(MainWindow)
        self.actionSave.setObjectName(u"actionSave")
        self.actionLikes = QAction(MainWindow)
        self.actionLikes.setObjectName(u"actionLikes")
        self.actionPaste = QAction(MainWindow)
        self.actionPaste.setObjectName(u"actionPaste")
        self.centralwidget = QWidget(MainWindow)

        # Central Widget
        self.centralwidget.setObjectName(u"centralwidget")
        self.stackedWidget = QStackedWidget(self.centralwidget)
        self.stackedWidget.setObjectName(u"stackedWidget")
        self.stackedWidget.setGeometry(QRect(0, 0, 791, 451))
        self.stackedWidget.setFrameShadow(QFrame.Shadow.Raised)
        self.MainPage = QWidget()

        # Main Page
        self.MainPage.setObjectName(u"MainPage")

        self.CurrTime = QLabel(self.MainPage)
        self.CurrTime.setObjectName(u"CurrTime")
        self.CurrTime.setGeometry(QRect(290, 10, 221, 41))
        self.CurrTime.setFrameShape(QFrame.Shape.WinPanel)
        self.CurrTime.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.TimeTimer = QTimer()
        self.TimeTimer.timeout.connect(
            lambda: self.CurrTime.setText(f"Current Time: {QDateTime.currentDateTime().toString('hh:mm AP')}"))
        self.TimeTimer.start(60000)

        # Update data every min from the excel
        self.DataTimer = QTimer()
        self.DataTimer.timeout.connect(self.update_data)
        self.DataTimer.start(60000)
        #initial data update
        QTimer.singleShot(0,self.update_data)


        # Set up and connect modify button to function
        self.ModifyButt = QPushButton(self.MainPage)
        self.ModifyButt.setObjectName(u"ModifyButt")
        self.ModifyButt.setGeometry(QRect(260, 370, 271, 61))
        self.ModifyButt.clicked.connect(self.switch_to_modify)


        self.TempTitle1 = QLabel(self.MainPage)
        self.TempTitle1.setObjectName(u"TempTitle1")
        self.TempTitle1.setGeometry(QRect(60, 80, 111, 16))
        self.HumidityTitle1 = QLabel(self.MainPage)
        self.HumidityTitle1.setObjectName(u"HumidityTitle1")
        self.HumidityTitle1.setGeometry(QRect(60, 190, 58, 16))
        self.LightTitle1 = QLabel(self.MainPage)
        self.LightTitle1.setObjectName(u"LightTitle1")
        self.LightTitle1.setGeometry(QRect(60, 310, 81, 16))
        self.SoundTitle1 = QLabel(self.MainPage)
        self.SoundTitle1.setObjectName(u"SoundTitle1")
        self.SoundTitle1.setGeometry(QRect(60, 380, 81, 16))
        self.frame = QFrame(self.MainPage)
        self.frame.setObjectName(u"frame")
        self.frame.setGeometry(QRect(340, 70, 120, 281))
        self.frame.setAutoFillBackground(False)
        self.frame.setFrameShape(QFrame.Shape.VLine)
        self.frame.setFrameShadow(QFrame.Shadow.Raised)

        self.InTemp = QLabel(self.MainPage)
        self.InTemp.setObjectName(u"InTemp")
        self.InTemp.setGeometry(QRect(60, 110, 231, 71))
        self.InTemp.setFrameShape(QFrame.Shape.Box)
        self.InTemp.setFrameShadow(QFrame.Shadow.Raised)
        self.InTemp.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.InTemp.setWordWrap(False)

        self.InHumid = QLabel(self.MainPage)
        self.InHumid.setObjectName(u"InHumid")
        self.InHumid.setGeometry(QRect(60, 210, 231, 71))
        self.InHumid.setFrameShape(QFrame.Shape.Box)
        self.InHumid.setFrameShadow(QFrame.Shadow.Raised)
        self.InHumid.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.InLight = QLabel(self.MainPage)
        self.InLight.setObjectName(u"InLight")
        self.InLight.setGeometry(QRect(60, 330, 131, 41))
        self.InLight.setFrameShape(QFrame.Shape.Box)
        self.InLight.setFrameShadow(QFrame.Shadow.Raised)
        self.InLight.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.InSound = QLabel(self.MainPage)
        self.InSound.setObjectName(u"InSound")
        self.InSound.setGeometry(QRect(60, 400, 131, 41))
        self.InSound.setFrameShape(QFrame.Shape.Box)
        self.InSound.setFrameShadow(QFrame.Shadow.Raised)
        self.InSound.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.SoundTitle2 = QLabel(self.MainPage)
        self.SoundTitle2.setObjectName(u"SoundTitle2")
        self.SoundTitle2.setGeometry(QRect(680, 380, 81, 16))

        self.HumdityTitle2 = QLabel(self.MainPage)
        self.HumdityTitle2.setObjectName(u"HumdityTitle2")
        self.HumdityTitle2.setGeometry(QRect(700, 190, 58, 16))

        self.TempTitle2 = QLabel(self.MainPage)
        self.TempTitle2.setObjectName(u"TempTitle2")
        self.TempTitle2.setGeometry(QRect(650, 80, 111, 16))

        self.LightTitle2 = QLabel(self.MainPage)
        self.LightTitle2.setObjectName(u"LightTitle2")
        self.LightTitle2.setGeometry(QRect(670, 310, 81, 16))

        self.OutSound = QLabel(self.MainPage)
        self.OutSound.setObjectName(u"OutSound")
        self.OutSound.setGeometry(QRect(620, 400, 131, 41))
        self.OutSound.setFrameShape(QFrame.Shape.Box)
        self.OutSound.setFrameShadow(QFrame.Shadow.Raised)
        self.OutSound.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.OutTemp = QLabel(self.MainPage)
        self.OutTemp.setObjectName(u"OutTemp")
        self.OutTemp.setGeometry(QRect(520, 110, 231, 71))
        self.OutTemp.setFrameShape(QFrame.Shape.Box)
        self.OutTemp.setFrameShadow(QFrame.Shadow.Raised)
        self.OutTemp.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.OutHumid = QLabel(self.MainPage)
        self.OutHumid.setObjectName(u"OutHumid")
        self.OutHumid.setGeometry(QRect(520, 210, 231, 71))
        self.OutHumid.setFrameShape(QFrame.Shape.Box)
        self.OutHumid.setFrameShadow(QFrame.Shadow.Raised)
        self.OutHumid.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.OutLight = QLabel(self.MainPage)
        self.OutLight.setObjectName(u"OutLight")
        self.OutLight.setGeometry(QRect(620, 330, 131, 41))
        self.OutLight.setFrameShape(QFrame.Shape.Box)
        self.OutLight.setFrameShadow(QFrame.Shadow.Raised)
        self.OutLight.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.InTitle = QLabel(self.MainPage)
        self.InTitle.setObjectName(u"InTitle")
        self.InTitle.setGeometry(QRect(60, 20, 141, 41))

        font = QFont()
        font.setPointSize(40)
        font.setBold(True)
        font.setUnderline(False)
        font.setStrikeOut(False)
        font.setKerning(True)

        self.InTitle.setFont(font)
        self.InTitle.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.OutTitle = QLabel(self.MainPage)
        self.OutTitle.setObjectName(u"OutTitle")
        self.OutTitle.setGeometry(QRect(590, 20, 171, 41))
        self.OutTitle.setFont(font)
        self.OutTitle.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.HLine = QFrame(self.MainPage)
        self.HLine.setObjectName(u"HLine")
        self.HLine.setGeometry(QRect(20, 40, 761, 51))
        self.HLine.setFrameShape(QFrame.Shape.HLine)
        self.HLine.setFrameShadow(QFrame.Shadow.Raised)
        self.stackedWidget.addWidget(self.MainPage)
        self.ModifyPage = QWidget()

        # Page Modify
        self.ModifyPage.setObjectName(u"ModifyPage")
        self.DoneModButt = QPushButton(self.ModifyPage)
        self.DoneModButt.setObjectName(u"DoneModButt")
        self.DoneModButt.setGeometry(QRect(410, 340, 131, 41))
        self.TempModify = QDoubleSpinBox(self.ModifyPage)
        self.TempModify.setObjectName(u"TempModify")
        self.TempModify.setGeometry(QRect(150, 250, 151, 41))
        self.HumidModify = QDoubleSpinBox(self.ModifyPage)
        self.HumidModify.setObjectName(u"HumidModify")
        self.HumidModify.setGeometry(QRect(470, 250, 151, 41))
        self.TempTitle3 = QLabel(self.ModifyPage)
        self.TempTitle3.setObjectName(u"TempTitle3")
        self.TempTitle3.setGeometry(QRect(170, 140, 111, 16))
        self.HumidityTitle3 = QLabel(self.ModifyPage)
        self.HumidityTitle3.setObjectName(u"HumidityTitle3")
        self.HumidityTitle3.setGeometry(QRect(520, 140, 58, 16))

        self.CancelModButt = QPushButton(self.ModifyPage)
        self.CancelModButt.setObjectName(u"CancelModButt")
        self.CancelModButt.setGeometry(QRect(220, 340, 141, 41))
        self.CancelModButt.clicked.connect(self.switch_to_main)

        self.ModTemp = QLabel(self.ModifyPage)
        self.ModTemp.setObjectName(u"ModTemp")
        self.ModTemp.setGeometry(QRect(100, 170, 231, 71))
        self.ModTemp.setFrameShape(QFrame.Shape.Box)
        self.ModTemp.setFrameShadow(QFrame.Shadow.Raised)
        self.ModTemp.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.ModTemp.setWordWrap(False)
        self.ModHumid = QLabel(self.ModifyPage)
        self.ModHumid.setObjectName(u"ModHumid")
        self.ModHumid.setGeometry(QRect(440, 170, 231, 71))
        self.ModHumid.setFrameShape(QFrame.Shape.Box)
        self.ModHumid.setFrameShadow(QFrame.Shadow.Raised)
        self.ModHumid.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.ModHumid.setWordWrap(False)
        self.stackedWidget.addWidget(self.ModifyPage)
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QMenuBar(MainWindow)
        self.menubar.setObjectName(u"menubar")
        self.menubar.setGeometry(QRect(0, 0, 798, 24))
        self.menuFile = QMenu(self.menubar)
        self.menuFile.setObjectName(u"menuFile")
        self.menuEdit = QMenu(self.menubar)
        self.menuEdit.setObjectName(u"menuEdit")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QStatusBar(MainWindow)
        self.statusbar.setObjectName(u"statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.menubar.addAction(self.menuFile.menuAction())
        self.menubar.addAction(self.menuEdit.menuAction())
        self.menuFile.addAction(self.actionNew)
        self.menuFile.addAction(self.actionSave)
        self.menuEdit.addAction(self.actionLikes)
        self.menuEdit.addAction(self.actionPaste)

        self.retranslateUi(MainWindow)

        self.stackedWidget.setCurrentIndex(0)


        QMetaObject.connectSlotsByName(MainWindow)
        self.update_labels()
    # setupUi

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QCoreApplication.translate("MainWindow", u"MainWindow", None))
        self.actionNew.setText(QCoreApplication.translate("MainWindow", u"New", None))
#if QT_CONFIG(statustip)
        self.actionNew.setStatusTip(QCoreApplication.translate("MainWindow", u"Create a new file", None))
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(shortcut)
        self.actionNew.setShortcut(QCoreApplication.translate("MainWindow", u"Return", None))
#endif // QT_CONFIG(shortcut)
        self.actionSave.setText(QCoreApplication.translate("MainWindow", u"Save", None))
#if QT_CONFIG(statustip)
        self.actionSave.setStatusTip(QCoreApplication.translate("MainWindow", u"save a file", None))
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(shortcut)
        self.actionSave.setShortcut(QCoreApplication.translate("MainWindow", u"Return", None))
#endif // QT_CONFIG(shortcut)
        self.actionLikes.setText(QCoreApplication.translate("MainWindow", u"Add Current Setting", None))
#if QT_CONFIG(shortcut)
        self.actionLikes.setShortcut(QCoreApplication.translate("MainWindow", u"Backspace, Ctrl+C", None))
#endif // QT_CONFIG(shortcut)
        self.actionPaste.setText(QCoreApplication.translate("MainWindow", u"Load Saved Settings", None))
#if QT_CONFIG(shortcut)
        self.actionPaste.setShortcut(QCoreApplication.translate("MainWindow", u"Ctrl+V", None))
#endif // QT_CONFIG(shortcut)
        self.CurrTime.setText(QCoreApplication.translate("MainWindow", u"Current Time: 4:05 PM", None))
        self.ModifyButt.setText(QCoreApplication.translate("MainWindow", u"Modify", None))
        self.TempTitle1.setText(QCoreApplication.translate("MainWindow", u"Temperature (\u00b0F)", None))
        self.HumidityTitle1.setText(QCoreApplication.translate("MainWindow", u"Humidity", None))
        self.LightTitle1.setText(QCoreApplication.translate("MainWindow", u"Light (lumen)", None))
        self.SoundTitle1.setText(QCoreApplication.translate("MainWindow", u"Sound (dB)", None))
        self.InTemp.setText(QCoreApplication.translate("MainWindow", u"TextLabel", None))
        self.InHumid.setText(QCoreApplication.translate("MainWindow", u"TextLabel", None))
        self.InLight.setText(QCoreApplication.translate("MainWindow", u"TextLabel", None))
        self.InSound.setText(QCoreApplication.translate("MainWindow", u"TextLabel", None))
        self.SoundTitle2.setText(QCoreApplication.translate("MainWindow", u"Sound (dB)", None))
        self.HumdityTitle2.setText(QCoreApplication.translate("MainWindow", u"Humidity", None))
        self.TempTitle2.setText(QCoreApplication.translate("MainWindow", u"Temperature (\u00b0F)", None))
        self.LightTitle2.setText(QCoreApplication.translate("MainWindow", u"Light (lumen)", None))
        self.OutSound.setText(QCoreApplication.translate("MainWindow", u"TextLabel", None))
        self.OutTemp.setText(QCoreApplication.translate("MainWindow", u"TextLabel", None))
        self.OutHumid.setText(QCoreApplication.translate("MainWindow", u"TextLabel", None))
        self.OutLight.setText(QCoreApplication.translate("MainWindow", u"TextLabel", None))
        self.InTitle.setText(QCoreApplication.translate("MainWindow", u"Indoor", None))
        self.OutTitle.setText(QCoreApplication.translate("MainWindow", u"Outdoor", None))
        self.DoneModButt.setText(QCoreApplication.translate("MainWindow", u"Done", None))
        self.TempTitle3.setText(QCoreApplication.translate("MainWindow", u"Temperature (\u00b0F)", None))
        self.HumidityTitle3.setText(QCoreApplication.translate("MainWindow", u"Humidity", None))
        self.CancelModButt.setText(QCoreApplication.translate("MainWindow", u"Cancel", None))
        self.ModTemp.setText(QCoreApplication.translate("MainWindow", u"TextLabel", None))
        self.ModHumid.setText(QCoreApplication.translate("MainWindow", u"TextLabel", None))
        self.menuFile.setTitle(QCoreApplication.translate("MainWindow", u"File", None))
        self.menuEdit.setTitle(QCoreApplication.translate("MainWindow", u"Edit", None))
    # retranslateUi

    # Switch Page when Modify is Clicked
    def switch_to_modify(self):
        self.stackedWidget.setCurrentWidget(self.ModifyPage)

    def switch_to_main(self):
        self.stackedWidget.setCurrentWidget(self.MainPage)

    # Update the labels based on the dictionary
    def update_labels(self):
        self.InTemp.setText(f"{self.Indoor['Temperature']} °C")
        self.InHumid.setText(f"{self.Indoor['Humidity']}%")
        self.InLight.setText(f"{self.Indoor['Light']} lumen")
        self.InSound.setText(f"{self.Indoor['Sound']} db")

        self.OutTemp.setText(f"{self.Outdoor['Temperature']} °C")
        self.OutHumid.setText(f"{self.Outdoor['Humidity']}%")
        self.OutLight.setText(f"{self.Outdoor['Light']} lumen")
        self.OutSound.setText(f"{self.Outdoor['Sound']} db")

    
    def update_data(self):
        try:
            # Read the Excel file 
            df = pd.read_excel(self.file_path, usecols="A:I")

            # print("Columns in the DataFrame:", df.columns)
            df_clean = df.dropna(how="all")

            latest = df_clean.iloc[-1]

            # Update indoor values (adjust the column names if necessary)
            self.Indoor["Temperature"] = latest["T-in"]
            self.Indoor["Humidity"] = latest["H-in"]
            self.Indoor["Light"] = latest["Light"]
            self.Indoor["Sound"] = latest["Sound"]
            # Update outdoor values
            self.Outdoor["Temperature"] = latest["T-out"]
            self.Outdoor["Humidity"] = latest["H-out"]
            self.Outdoor["Light"] = latest["Light"]
            self.Outdoor["Sound"] = latest["Sound"]

        except Exception as e:
            print("Error reading xlsx file:", e)
        # Refresh the labels with new data
        self.update_labels()

        


if __name__ == "__main__":
    import sys
    app = QApplication(sys.argv)
    MainWindow = QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec())
