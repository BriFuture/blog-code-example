import sys
# from PyQt5 import  QtGui
from PyQt5.QtWidgets import QApplication
from PyQt5.QtCore import QDir, QUrl, Qt, QEvent, QThread
from PyQt5.QtWebEngineWidgets import QWebEngineView, QWebEngineSettings, QWebEnginePage
from gevent import monkey
monkey.patch_all()

class WebServerThread( QThread ):
    def __init__(self):
        super( WebServerThread, self).__init__()

    def __del__(self):
        self.wait()

    def run(self):
        from webserver import get_web_server
        self.server = get_web_server()
        self.server.serve_forever()

class MyWebView(QWebEngineView):
    def __init__(self, app):
        super(MyWebView, self).__init__()
        self.app = app

    def closeEvent(self, event):
        self.activateWindow()
        self.destroy()
        self.app.quit()
        from time import sleep
        sleep(1)
        sys.exit(0)
        
    def changeEvent(self, event):
        if event.type() == QEvent.ActivationChange:
            active = self.isActiveWindow()
            # print( 'On Change Event', active, self.isVisible() )
            if active:
                self.setWindowFlags( self.windowFlags() | Qt.WindowStaysOnTopHint )
                # print('On Active')
                if not self.isVisible():
                    # print('On Active to show')
                    self.show()
                    self.activateWindow()
            else:
                # print('On Deactive')
                self.setWindowFlags( self.windowFlags() & ~Qt.WindowStaysOnTopHint )
                if not self.isVisible():
                    self.show()
                # self.showNormal()
        ## 异常代码，切换几次应用后，本应用窗口无法置顶或将永远置顶，导致代码没有作用
        # if self.skip:
        #     print( 'skip this event =========', self.isActiveWindow())
        #     self.skip = False
        #     return
        # if event.type() == QEvent.ActivationChange:
        #     print( 'On Change Event', self.isActiveWindow(), self.top )
        #     active = self.isActiveWindow()
        #     if self.top == 0 and active:
        #         self.top = 1
        #     elif self.top == 1 and not active:
        #         print('On Deactive')
        #         self.setWindowFlags( self.windowFlags() & ~Qt.WindowStaysOnTopHint)
        #         self.top = 2
        #         self.skip = True
        #         self.showNormal() # it will active this window
        #         print('after Show AS Normal', self.top)
        #     elif self.top == 2 and active:
        #     #     self.top = 3
        #     # elif self.top == 3 and active:
        #         self.top = 1
        #         self.setWindowFlags( self.windowFlags()  | Qt.WindowStaysOnTopHint )
        #         print('On Action top is true')
        #         self.show()
        #         print('On Active')
        #     # elif self.top == 4 and  active:
        #     #     self.top = 2
            
        #     print('-----------', self.top)


def init_gui(port=5000, width=1024, height=768, fullScreen = False,
             window_title="Test", icon="logo.png", argv=None):
    if argv is None:
        argv = sys.argv

    # Application Level
    qtapp = QApplication(argv)
    webapp = WebServerThread()
    webapp.start()
    qtapp.aboutToQuit.connect(webapp.terminate)

    view = MyWebView( qtapp )
    view.setWindowFlags( Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint )
    view.setAttribute( Qt.WA_ShowWithoutActivating )
    # view.setAttribute( Qt.WA_AlwaysStackOnTop )

    view.settings().setAttribute( QWebEngineSettings.Accelerated2dCanvasEnabled, False)

    view.setUrl( QUrl("http://localhost:{}/".format(port)) )
    view.resize( width, height )
    view.move(0, 0)
    view.show()
    view.activateWindow()

    return qtapp.exec_()

if __name__ == '__main__':
    init_gui(5050)