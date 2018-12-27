# -*- coding: utf-8 -*-

from PyQt5.QtWidgets import QApplication
from PyQt5.QtWebEngineWidgets import QWebEngineView
from PyQt5.QtCore import QUrl

def startWeb():
    from app import app
    app.run()

def main(argv):
    qtapp = QApplication(argv)
    from threading import Thread
    webapp = Thread(target=startWeb)
    webapp.daemon = True
    webapp.start()
    view = QWebEngineView()
    view.setWindowTitle('DeskApp')
    port = 5000
    view.setUrl( QUrl("http://localhost:{}/".format(port)))
    view.show()

    return qtapp.exec_()

import sys
if __name__ == '__main__':
    main(sys.argv)
