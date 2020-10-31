import cv2, queue, threading
import os
import numpy as np
import sys
from joblib import Parallel, delayed
import multiprocessing


def create_folder(PATH):
    if not os.path.exists(PATH):
        os.makedirs(PATH)


class Calibrator:    
    def __init__(self,size = (6,9),PATH_RESULTS='results/'):
        self.CNT = 0
        self.PATH_RESULTS = PATH_RESULTS
        self.create_folders()
        self.NUM_CORES = multiprocessing.cpu_count()
        self.CHESSBOARD_SIZE = size
        self.CHESSBOARD_OPTIONS = (cv2.CALIB_CB_ADAPTIVE_THRESH | cv2.CALIB_CB_NORMALIZE_IMAGE | cv2.CALIB_CB_FAST_CHECK)
        self.OBJECT_POINT_ZERO = np.zeros((self.CHESSBOARD_SIZE[0] * self.CHESSBOARD_SIZE[1], 3), np.float32)
        self.OBJECT_POINT_ZERO[:, :2] = np.mgrid[0:self.CHESSBOARD_SIZE[0],0:self.CHESSBOARD_SIZE[1]].T.reshape(-1, 2)
        self.OPTIMIZE_ALPHA = 0.25
        self.TERMINATION_CRITERIA = (cv2.TERM_CRITERIA_EPS | cv2.TERM_CRITERIA_MAX_ITER, 30,0.001)  
        
    def create_folders(self):
        self.PATH_IMAGES = self.PATH_RESULTS+'images/'
        self.PATH_VIDEOS = self.PATH_RESULTS+'videos/'
        create_folder(self.PATH_RESULTS)
        create_folder(self.PATH_IMAGES)
        create_folder(self.PATH_VIDEOS)
        for name in ['left','right','join']:
            create_folder(self.PATH_IMAGES+name)
    def save_images(self,left,right,join):
        cv2.imwrite(self.PATH_IMAGES+"left/image_"+str(self.CNT)+".jpg",left)
        cv2.imwrite(self.PATH_IMAGES+"right/image_"+str(self.CNT)+".jpg",right)
        cv2.imwrite(self.PATH_IMAGES+"join/image_"+str(self.CNT)+".jpg",join)
        self.CNT+=1
    def get_points(self,images):
        hasCorners,original_images,chess_images,object_points,corners = zip(*Parallel(n_jobs=self.NUM_CORES)(delayed(self._get_points)(i) for i in images))
        return hasCorners,original_images,chess_images,object_points,corners
    def _get_points(self,image):
        grayImage = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        newSize = grayImage.shape[::-1]
        hasCorners, corners = cv2.findChessboardCorners(grayImage,self.CHESSBOARD_SIZE, cv2.CALIB_CB_FAST_CHECK)
        original_image = image.copy()
        if hasCorners:
            cv2.drawChessboardCorners(image, self.CHESSBOARD_SIZE, corners, hasCorners)
            cv2.cornerSubPix(grayImage, corners, (11, 11), (-1, -1),self.TERMINATION_CRITERIA)
            return True,original_image,image,self.OBJECT_POINT_ZERO,corners
        return False,image,image,None,None
    

    
    
class StereoCapture:
    def __init__(self, name,size=(640,480)):
        self.cap = cv2.VideoCapture(name)
        self.CAMERA_WIDTH = size[0]
        self.CAMERA_HEIGHT  = size[1]
        self.cap.set(cv2.CAP_PROP_FRAME_WIDTH, self.CAMERA_WIDTH*2)
        self.cap.set(cv2.CAP_PROP_FRAME_HEIGHT,self.CAMERA_HEIGHT)
        self.q = queue.Queue()
        self.t = threading.Thread(target=self._reader)
        self.t.daemon = True
        self.end = False
        self.t.start()
    def _reader(self):
        while True:
            if self.end:
                break
            try:
                ret, frame = self.cap.read()
                if not ret:
                    break
                if not self.q.empty():
                    try:
                        self.q.get_nowait()   # discard previous (unprocessed) frame
                    except queue.Empty:
                        pass
            except:
                break
            self.q.put(frame)
    def read(self):
        return self.q.get()
    def release(self):
        self.end = True
        self.cap.release()
        self.t.join()
