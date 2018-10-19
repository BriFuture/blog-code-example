import mybase64
import base64, random, string, timeit, sys

def test():
    str = b'jz4Vxns'

    expect = base64.b64encode( str ) 
    actual = mybase64.b64encode( str )
    print( 'expect: ', expect )
    print( 'actual: ', actual )
    print( expect == actual )

_CHARS = string.ascii_letters + string.digits
def randomString():
    # print( chars )
    size = random.randint( 70, 100 )
    rstr = ''.join( random.SystemRandom().choices( _CHARS, k = size ) )
    return rstr.encode()

def encode1():
    rstr = randomString()
    base64.b64encode( rstr )

def encode2():
    rstr = randomString()
    mybase64.b64encode( rstr )

def compare():
    rstr = randomString()
    exp = base64.b64encode( rstr )
    act = mybase64.b64encode( rstr )
    if( exp != act ):
        print( rstr )
        print( exp )
        print( act )
        raise ValueError
    
loops = 10000
# print( 'encode comp: ', timeit.timeit( stmt = compare,  number = loops ) )

print( sys.version )

print( 'random: ', timeit.timeit( randomString, number = loops ) )
print( 'encode1: ', timeit.timeit( stmt = encode1, number = loops ) )
print( 'encode2: ', timeit.timeit( stmt = encode2, number = loops ) )

