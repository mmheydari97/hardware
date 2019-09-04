from PyQt5.QtWidgets import QMainWindow, QApplication, QPushButton, QLineEdit, QLabel, QProgressBar, QFrame
from PyQt5.uic import loadUi
import numpy as np
import serial
import time
from matplotlib import pyplot as plt

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
        self.progressBar.setValue(0)
        self.progressBar.setEnabled(False)
        self.label_status.setText("UCTA 2018 Application started. Enter COM port name and click on Open Port button.")
        self.lineEdit_Fs.setEnabled(False)
        self.lineEdit_Length.setEnabled(False)
        self.pushButton_getSamples.setEnabled(False)

    def clone(self):
        # Clone
        self.pushButton_OpenSerial = QPushButton()
        self.lineEdit_COM = QLineEdit()
        self.lineEdit_Fs = QLineEdit()
        self.lineEdit_Length = QLineEdit()
        self.progressBar = QProgressBar()
        self.label_status = QLabel()
        self.pushButton_getSamples = QPushButton()
        # End of clones

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
            self.lineEdit_Fs.setEnabled(True)
            self.pushButton_getSamples.setEnabled(True)
            self.pushButton_OpenSerial.setEnabled(False)
            self.lineEdit_COM.setEnabled(False)
            self.port = port
            self.lineEdit_Fs.setText('1000')
            self.lineEdit_Length.setText('1000')
            self.stat('successfully connected to port: {}'.format(self.port))
        else:
            self.stat('Port {} exists but can not open the port. Is other application using it?'.format(self.port))
            self.port = None
            self.dev = None

    def stat(self, msg):
        self.label_status.setText(msg)
        QApplication.processEvents()

    def on_pushButton_getSamples_pressed(self):
        print('Getting samples')
        _Fs = int(self.lineEdit_Fs.text())
        _Length = int(self.lineEdit_Length.text())

        fs = _Fs.to_bytes(2, 'big')
        length = _Length.to_bytes(2, 'big')

        fs0 = bytes((fs[0],))
        fs1 = bytes((fs[1],))

        l0 = bytes((length[0],))
        l1 = bytes((length[1],))

        print('Communicating now:')

        if self.communicateAByte(b'S') != b'S':
            self.stat("Error: Device didn't say hi to protocol!")
            return
        self.stat('Device said Hi.')
        print('\tStarted messaging')

        if self.communicateAByte(fs0) != b'M':
            self.stat("Error: Problem while communicating Fs")
            return
        print('\tSent FS MSB.')

        if self.communicateAByte(fs1) != b'L':
            self.stat("Error: Problem while communicating Fs")
            return
        self.stat('Device got Fs.')
        print('\tSend FS LSB.')

        if self.communicateAByte(l0) != b'A':
            self.stat("Error: Problem while communicating length")
            return
        print('\tSent Length MSB')

        if self.communicateAByte(l1) != b'B':
            self.stat("Error: Problem while communicating length")
            return
        self.stat("Device got samples.")
        print('\tSend Length LSB. Device got samples.')

        self.samples = []
        chk = 0
        for i in range(_Length):
            self.stat('Downloading samples: {}/{}'.format(i, _Length))
            print('Data:{}/{}'.format(i, _Length))
            data = self.communicateAByte(b'N')
            chk += ord(data)
            self.samples.append(data[0])
            self.progressBar.setValue(100 * i / _Length)
            QApplication.processEvents()

        checkSum = self.communicateAByte(b'E')
        self.stat('Got the checksum:{}, calculated the checksum:{}'.format(ord(checkSum), chk & 255))

        t = np.linspace(0, (_Length-1)/_Fs, _Length)

        self.fig = plt.figure()
        self.fig.set_label('Monitor')
        plt.plot(t, self.samples)
        plt.xlabel('Time')
        plt.ylabel('Voltage level (mapped to 0-255)')
        plt.title('UCTA2018 Digital Oscilloscope Monitor!')
        plt.show()

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
