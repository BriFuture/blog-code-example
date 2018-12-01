# app.py
from gevent import monkey
monkey.patch_all ()

from flask import Flask
app = Flask( __name__ )

@app.route( '/')
def hello():
    raise ValueError('Test Error')
    return 'Hello World'

from time import sleep
@app.route( '/testsleep' )
def test_sleep():

    sleep( 10 )
    return 'Hi, You wait for about 10 seconds, right?'

def main():
    app.run( degug = True )

def main2():
    from gevent import pywsgi
    app.debug = True
    server = pywsgi.WSGIServer( ( '127.0.0.1', 5000 ), dapp )
    server.serve_forever()

def main3():
    from gevent import pywsgi
    from werkzeug.debug import DebuggedApplication
    app.debug = True
    dapp = DebuggedApplication( app, evalex= True)
    server = pywsgi.WSGIServer( ( '127.0.0.1', 5000 ), dapp )
    server.serve_forever()

def main5():
    from gevent import pywsgi
    from werkzeug.debug import DebuggedApplication
    dapp = DebuggedApplication( app, evalex= True)
    from werkzeug.serving import run_with_reloader
    server = pywsgi.WSGIServer( ( '127.0.0.1', 5000 ), dapp )
    run_with_reloader( server ).serve_forever()  

if __name__ == '__main__':
    main()