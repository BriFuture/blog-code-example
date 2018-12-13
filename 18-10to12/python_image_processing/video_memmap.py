# -*- coding: utf-8 -*-

"""
Created on Wed Dec  5 16:16:33 2018

@author: BriFuture
"""
import cv2 as cv
import numpy as np

FILE_NAME = "I:/video.dat"
WIDTH = 2096
LINE_PER_IMAGE = 150
CHANNELS = 4
PACK_SIZE = WIDTH * LINE_PER_IMAGE * CHANNELS
SHAPE = (LINE_PER_IMAGE, WIDTH, CHANNELS )

import time
def showWithFile():
    cv.namedWindow("file")
    count = 0
    start = time.time()
    try:
        number = 1
        while True:
            e1 = cv.getTickCount()
            img = np.memmap(filename=FILE_NAME, dtype=np.uint8, shape=SHAPE, mode="r+", offset=count )
            count += PACK_SIZE
            cv.imshow( "file", img )
            e2 = cv.getTickCount()
            elapsed = ( e2 - e1 ) / cv.getTickFrequency()
            print("FPS: %.2f Used time: %.3f" % (number / elapsed, elapsed ))
            key = cv.waitKey(20)
            if key == 27:  # exit on ESC
                break
    except StopIteration:
        pass
    finally:
        end = time.time()
        print( 'File Data read: {:.2f}Gb'.format( count / 1024 / 1024 / 1024), ' time used: {:.2f}s'.format( end - start ) )
        cv.destroyAllWindows()


if __name__ == '__main__':
    showWithFile()
