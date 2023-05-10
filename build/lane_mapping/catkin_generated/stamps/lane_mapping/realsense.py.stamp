#!/usr/bin/env python3
import rospy
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
import cv2
import os
import numpy as np
import pandas as pd

class Nodo(object):
    def __init__(self):
        # Params
        self.image = None
        self.br = CvBridge()
        # Node cycle rate (in Hz).
        self.loop_rate = rospy.Rate(20)
        # use 6 for realsense
        # self.cap=cv2.VideoCapture(2)

        self.joy_sub = rospy.Subscriber("/camera/rgb/image_raw",Image,self.img_to_laser_callback)

        # Publishers
        self.pub = rospy.Publisher('detected_lanes', Image,queue_size=10)
        # self.callback()

    def copy_image(self,frame):
        copied = frame.copy()
        return copied
    
    def change(self,frame):
        frame[:,:,1] = 0
        frame[:,:,2] = 0
        return frame
    
    def blurring(self,frame):
        kernel = (5,5)
        blurred = cv2.GaussianBlur(frame,kernel,0)
        return blurred
    

    def threshold(self,frame):
        ret,thresh = cv2.threshold(frame,120,255,cv2.THRESH_BINARY)
        return thresh
    
    def dilation(self,frame):
        kernel = (3,3)
        #dilate = cv.dilate(frame,kernel,iterations = 1)
        opening = cv2.morphologyEx(frame, cv2.MORPH_OPEN, kernel)
        return opening
    
    def edge_detection(self,frame):
        edge = cv2.Canny(frame,0,200)
        return edge
    
    def ROI(self,frame,vertices) :
        mask=np.zeros_like(frame)  
        match_mask_color=(255,)
        cv2.fillPoly(mask,vertices,match_mask_color)
        masked=cv2.bitwise_and(frame,mask)
        return masked

    def hough_lines(self,roi, frame):

        lines = cv2.HoughLinesP(roi,1,np.pi/180,15,minLineLength=20,maxLineGap=15)
        try:
            for line in lines:
                for x1,y1,x2,y2 in line:
                    frames = cv2.line(frame,(x1,y1),(x2,y2),(255,0,0),3)
            lines_edges = cv2.addWeighted(frames, 0.8, frame, 1, 0)
            return lines_edges
        except:
            print("lines not detected")
            return frame 

    def img_to_laser_callback(self,msg):

        self.image = self.br.imgmsg_to_cv2(msg)
        self.height,self.width=self.image.shape[:2]

        frame=self.image

        data = pd.read_csv('matrix.csv')
        data = np.asarray(data.values.tolist())

        frame = cv2.warpPerspective(frame, data, (frame.shape[1], frame.shape[0]))

        # frame[:,200:500] = 0
        # frame[200:,100:550] = 0

        # frame = cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)

        # cv2.imshow("frame",frame)
        cv2.waitKey(1)

        self.pub.publish(self.br.cv2_to_imgmsg(frame))

    # def callback(self):

    #     while True:
    #         ret,self.image=self.cap.read()
    #         self.height,self.width=self.image.shape[:2]
    #         # print(self.width)
                    
    #         frame=self.image

    #         # frame =cv2.convertScaleAbs(frame, 0.3, 0.6)
            
    #         data = pd.read_csv('matrix.csv')
    #         data = np.asarray(data.values.tolist())

    #         frame = cv2.warpPerspective(frame, data, (frame.shape[1], frame.shape[0]))

    #         frame[:,100:550] = 0

    #         self.pub.publish(self.br.cv2_to_imgmsg(frame))

    #         # cv2.imshow("frame",frame)

    #         cv2.waitKey(1)


    def start(self):
        c=0
        while not rospy.is_shutdown():
            if self.image is not None:
                c=0
            self.loop_rate.sleep()

if __name__ == '__main__':
    rospy.init_node("realsense")
    my_node = Nodo()
    my_node.start()
