import matplotlib.pyplot as plt
from matplotlib import style
from functools import partial
import numpy as np
from datetime import datetime

FILE_NAME = "I:/video.dat"
WIDTH = 2096
HEIGHT = 150
CHANNELS = 4
PACK_SIZE = WIDTH * HEIGHT * CHANNELS

import cv2 as cv
def gen_image_from_file( file ):
    with open( file, 'rb' ) as f:
        records = iter( partial( f.read, PACK_SIZE ), b'' )
        frame = next( records )
        frame = next( records )
        img = np.zeros( ( HEIGHT, WIDTH, CHANNELS ), dtype = np.uint8)
        e1 = cv.getTickCount()
        for y in range(0, HEIGHT):
            for x in range( 0, WIDTH ):
                pos = (y * WIDTH + x) * CHANNELS
                for i in range( 0, CHANNELS - 1 ):
                    img[y][x][i] = frame[ pos + i ]
                img[y][x][3] = 255
        e2 = cv.getTickCount()
        elapsed = ( e2 - e1 ) / cv.getTickFrequency()
        print("Time Used: ", elapsed )
        plt.imshow( img )
        plt.tight_layout()
        plt.subplots_adjust(left=0, right=1, top=1, bottom=0)
        plt.xticks([])
        plt.yticks([])
        plt.show()

import matplotlib.animation as animation
def image_animation():
    fig = plt.figure()
    ax1 = fig.add_subplot(1, 1, 1)

    try:
        img = np.zeros( ( HEIGHT, WIDTH, CHANNELS ), dtype = np.uint8)
        f = open( FILE_NAME, 'rb' )
        records = iter( partial( f.read, PACK_SIZE ), b'' )
        
        def animateFromData(i):
            e1 = cv.getTickCount()
            frame = next( records ) # drop a line data
            for y in range( 0, HEIGHT ):
                for x in range( 0, WIDTH ):
                    pos = (y * WIDTH + x) * CHANNELS
                    for i in range( 0, CHANNELS - 1 ):
                        img[y][x][i] = frame[ pos + i]
                    img[y][x][3] = 255
            ax1.clear()
            ax1.imshow( img )
            e2 = cv.getTickCount()
            elapsed = ( e2 - e1 ) / cv.getTickFrequency()
            print( "FPS: %.2f,  Used time: %.3f" % (1 / elapsed, elapsed ))
        
        def optAnimateFromData(i):
            e1 = cv.getTickCount()
            frame = next( records ) # one image data
            img = np.reshape( np.array( list( frame ), dtype = np.uint8 ), ( HEIGHT, WIDTH, CHANNELS ) )
            img[ : , : , 3] = 255
            ax1.clear()
            ax1.imshow( img )
            e2 = cv.getTickCount()
            elapsed = ( e2 - e1 ) / cv.getTickFrequency()
            print( "FPS: %.2f,  Used time: %.3f" % (1 / elapsed, elapsed ))

        # a = animation.FuncAnimation( fig, animateFromData, interval=30 )
        a = animation.FuncAnimation( fig, optAnimateFromData, interval=30 )
        plt.tight_layout()
        plt.subplots_adjust(left=0, right=1, top=1, bottom=0)
        plt.xticks([])
        plt.yticks([])
        plt.show()
    except StopIteration:
        pass
    finally:
        f.close()

if __name__ == '__main__':
    image_animation()
    # gen_image_from_file( FILE_NAME )
