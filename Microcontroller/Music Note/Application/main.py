from PyQt5.QtWidgets import QMainWindow, QApplication, QPushButton, QLineEdit, QLabel, QProgressBar, QFrame
from PyQt5.uic import loadUi
import numpy as np
import serial
import time

class mainApplication(QMainWindow):
    def __del__(self):
        if self.dev != None:
            self.dev.close()

    def __init__(self):
        super().__init__()
        self.ui = None
        self.load_ui()
        self.init()
        self.show()

        self.dev = None
        self.port = None

    def init(self):
        self.setWindowTitle('UCTA2018 application')
        self.label_status.setText("UCTA 2018 Application started. Enter COM port name and click on Open Port button.")
        self.lineEdit_noteNumber.setEnabled(False)
        self.lineEdit_Length.setEnabled(False)
        self.pushButton_sendNote.setEnabled(False)

    def load_ui(self):
        self.ui = loadUi('app.ui', self)

    def on_pushButton_OpenSerial_pressed(self):
        print('Openning serial')

        port = self.lineEdit_COM.text()
        print('Connecting to {}'.format(port))

        try:
            self.dev = serial.Serial(port, 115200, timeout=1)
        except serial.serialutil.SerialException:
            self.stat("Port {} does not exist, or other application is using it.".format(port))
            return

        try:
            o = self.dev.is_open
        except serial.serialutil.SerialException:
            self.stat('Port {} exists but can not open the port. Is other application using it?'.format(self.port))
            self.port = None
            self.dev = None
            return

        if o:
            self.lineEdit_Length.setEnabled(True)
            self.lineEdit_noteNumber.setEnabled(True)
            self.pushButton_sendNote.setEnabled(True)
            self.pushButton_OpenSerial.setEnabled(False)
            self.lineEdit_COM.setEnabled(False)
            self.port = port
            self.lineEdit_noteNumber.setText('1')
            self.lineEdit_Length.setText('10')
            self.stat('successfully connected to port: {}'.format(self.port))
        else:
            self.stat('Port {} exists but can not open the port. Is other application using it?'.format(self.port))
            self.port = None
            self.dev = None

    def stat(self, msg):
        self.label_status.setText(msg)
        QApplication.processEvents()

    def on_pushButton_sendNote_pressed(self):
        print('Getting samples')
        _noteNumber = int(self.lineEdit_noteNumber.text())
        _Length = int(self.lineEdit_Length.text())

        noteNumber = _noteNumber.to_bytes(1, 'big')
        length = _Length.to_bytes(1, 'big')

        print('Communicating now:')

        if self.communicateAByte(b'S') != b'S':
            self.stat("Error: Device didn't say hi to protocol!")
            return
        self.stat('Device said Hi.')
        print('\tStarted messaging')

        if self.communicateAByte(noteNumber) != b'N':
            self.stat("Error: Problem while communicating note number")
            return
        print('\tSent note number')

        if self.communicateAByte(length) != b'D':
            self.stat("Error: Problem while communicating duration")
            return
        self.stat('Device got data. Note number = {}, Length = {} x 50 msecs'.format(_noteNumber, _Length))
        print('\tSent length')

    def communicateAByte(self, byteToSend):
        try:
            self.dev.write(byteToSend)
        except (serial.serialutil.SerialException, serial.serialutil.SerialTimeoutException) as e:
            self.stat('Error: Serial problem.')
            print('Write error.')
            self.exit()

        ch = None
        try:
            ch = self.dev.read(1)
        except serial.serialutil.SerialTimeoutException:
            self.stat('Error: Serial timeout.')
            print('timeout error.')
            exit()
        except serial.serialutil.SerialException:
            self.stat('Error: Serial problem.')
            print('Read error.')
            exit()

        print('\t\tSent:{}, got:{}'.format(byteToSend, ch))
        return ch

app = QApplication([])
window = mainApplication()
app.exec_()
