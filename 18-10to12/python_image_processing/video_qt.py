import matplotlib.animation as animation
from functools import partial
import numpy as np

FILE_NAME = "I:/video.dat"
WIDTH = 2096 # 256
LINE_PER_IMAGE = 150  #HEIGHT
LINE_PER_PACKET = 5
PACKET_SIZE = 2096 * 4 * LINE_PER_IMAGE # bytes per line
CHANNELS = 3

# test_image_animation_rgba()


import sys, random, matplotlib
from PyQt5 import QtCore, QtGui, QtWidgets
import numpy as np

matplotlib.use('Qt5Agg')
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
import matplotlib.pyplot as plt
from matplotlib import animation

class Window(QtWidgets.QWidget):
    def __init__(self, parent=None):
        super(Window, self).__init__(parent)
        self.setupUi(self)

    def setupUi(self, Form):
        Form.setObjectName("Form")
        Form.resize(800, 600)

        self.figure = plt.figure()
        self.canvas = FigureCanvas(self.figure)
        self.axes = self.figure.add_subplot(111)
        plt.tight_layout()
        plt.subplots_adjust(left=0, right=1, top=1, bottom=0)

        self.setLayout(QtWidgets.QVBoxLayout())
        self.layout().addWidget(self.canvas)

        # self.img = np.random.rand(320, 250)
        self.img = np.zeros( ( LINE_PER_IMAGE, WIDTH, CHANNELS ), dtype = np.uint8)
        self.file = open( FILE_NAME, 'rb' )
        self.records = iter( partial( self.file.read, PACKET_SIZE ), b'' )

        self.im = self.axes.imshow(self.img, interpolation = 'none')
        self.im.set_data(self.img)
        self.canvas.draw()

        self.anim = animation.FuncAnimation(self.figure, self.animate, interval=10, blit=True)

        self.count = 0

        self.timer = QtCore.QTimer(self)
        self.timer.timeout.connect(self.NumberRefreshPerSecond)
        self.timer.start(1000)

    def animate(self, i):
        frame = next( self.records ) # one image data
        self.img = np.reshape( np.array( list( frame ), dtype = np.uint8 ), 
                ( LINE_PER_IMAGE, WIDTH, CHANNELS+1 ) )
        self.img[ : , : , 3] = 255
        # for y in range( 0, LINE_PER_IMAGE ):
        # # print( len( frame ) )
        #     for x in range( 0, WIDTH ):
        #         for z in range( 0, 3 ):
        #             self.img[y][x][z] = frame[ x * 4+z]
                # self.img[y][x][3] = 0
        self.im.set_array( self.img )
        self.count = self.count + 1
        return [self.im]

    def NumberRefreshPerSecond(self):
        print(self.count)
        self.count = 0

    def closeEvent(self, event):
        self.timer.stop()
    
    def __del__(self):
        self.file.close()

if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    form = Window()
    form.showMaximized()
    sys.exit(app.exec_())