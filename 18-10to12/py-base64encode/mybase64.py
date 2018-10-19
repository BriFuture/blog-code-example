# print( len(str) )
from io import StringIO
import string

def constructTable():
    array = []
    for i in range( 65, 91 ):
        array.append( chr( i ) )
    for i in range( 97, 123 ):
        array.append( chr( i ) )
    for i in range( 0, 10 ):
        array.append( str( i ) )
    array.append( '+' )
    array.append( '-' )
    # print( array )
    return array

def constructTable2():
    str = string.ascii_uppercase + string.ascii_lowercase + string.digits
    return str + '+' + '-'


array = constructTable()

def b64encode( str ):
    # fill with zero
    length = len( str )
    remainder = length % 3

    if( remainder == 1 ):
        str = str + b'\x00\x00'  # add twice
        length += 2
    elif( remainder == 2 ):
        str = str + b'\x00'  # add once
        length += 1

    # encode
    i = 0
    buf = StringIO()
    if( remainder > 0 ):        
        # if zero is filled in tail
        while i < length - 3:
            en = _b64encode_str( str[ i ], str[ i+1 ], str[ i+2 ] )
            buf.write( en )
            i += 3
        # print( remainder, i, buf.getvalue() )
        en = _b64encode_str( str[ i ], str[ i+1 ], str[ i+2 ] )
        buf.write( en[ 0 ] )
        buf.write( en[ 1 ] )

        if( remainder == 2 ):
            buf.write( en[ 2 ] )  # add once
            buf.write( '=' )
        elif( remainder == 1 ):
            buf.write( '==' )   # add twice
    else:  # remainder == 0
        while i < length:
            en = _b64encode_str( str[ i ], str[ i+1 ], str[ i+2 ] )
            buf.write( en )
            i += 3
    
    # print( strlist )
    return buf.getvalue().encode()

def _b64encode_str( s0, s1, s2 ):
    """
    s0、s1、s2 依次为第一、二、三个字符
    """
    d = s2 & 63
    d = array[ d ]
    c1 = ( s1 & 15  ) << 2
    c2 = ( s2 & 192 ) >> 6
    c = c1 + c2
    c = array[ c ]
    b1 = ( s0   & 3 ) << 4
    b2 = ( s1 & 240 ) >> 4
    b = b1 + b2
    b = array[ b ]
    a = ( s0 & 252 ) >> 2
    a = array[ a ]
    return ''.join( [ a, b, c, d ] )

# TODO: reverse encode process 
def b64decode( estr ):
    return estr