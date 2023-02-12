#!/usr/bin/env python3
import rospy
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
import cv2
import os
import numpy as np
# import tensorflow as tf

class Nodo(object):
    def __init__(self):
        # Params
        self.image = None
        self.br = CvBridge()
        # Node cycle rate (in Hz).
        self.loop_rate = rospy.Rate(1)
        self.cap=cv2.VideoCapture(2)
        # self.model=tf.keras.models.load_model('/home/piyush/lane_ws/src/lane_mapping/model/model.h5')
        # Publishers
        self.pub = rospy.Publisher('detected_lanes', Image,queue_size=10)
        self.callback()

        # Subscribers
        # rospy.Subscriber("/zed2/zed_node/rgb/image_rect_color",Image,self.callback)
        # rospy.Subscriber("/my_robot/camera/image_raw",Image,self.callback)

    def copy_image(self,frame):
        copied = frame.copy()
        print("copy")
        return copied
    
    def change(self,frame):
        frame[:,:,1] = 0
        frame[:,:,2] = 0
        #cv.imshow("blue_frame",frame)
        print("change")
        return frame
    
    def blurring(self,frame):
        kernel = (5,5)
        blurred = cv2.GaussianBlur(frame,kernel,0)
        print("blur")
        return blurred
    def threshold(self,frame):
        ret,thresh = cv2.threshold(frame,120,255,cv2.THRESH_BINARY)
        print("thresh")
        return thresh
    def dilation(self,frame):
        kernel = (3,3)
        #dilate = cv.dilate(frame,kernel,iterations = 1)
        opening = cv2.morphologyEx(frame, cv2.MORPH_OPEN, kernel)
        print("dilate")
        return opening
    def edge_detection(self,frame):
        edge = cv2.Canny(frame,0,200)
        print("edge")
        return edge
    
    def ROI(self,frame,vertices) :
        mask=np.zeros_like(frame)  
        match_mask_color=(255,)
        cv2.fillPoly(mask,vertices,match_mask_color)
        masked=cv2.bitwise_and(frame,mask)
        print("roi")
        return masked

    def hough_lines(self,roi, frame):
        lines = cv2.HoughLinesP(roi,1,np.pi/180,15,minLineLength=20,maxLineGap=15)
        try:
            for line in lines:
                for x1,y1,x2,y2 in line:
                    frames = cv2.line(frame,(x1,y1),(x2,y2),(255,0,0),3)
            lines_edges = cv2.addWeighted(frames, 0.8, frame, 1, 0)
        except:
            print("lines not detected")
        print("hough")
        return frame

    def callback(self):
        # self.image = self.br.imgmsg_to_cv2(msg,"bgr8")
        while True:
            ret,self.image=self.cap.read()
            self.height,self.width=self.image.shape[:2]
            print(self.width)
        
            # frame=cv2.cvtColor(self.image,cv2.COLOR_BGR2GRAY)
            
            frame=self.image
            # frame =cv2.convertScaleAbs(frame, 0.8, 0.5)
            height=frame.shape[0]
            width=frame.shape[1]
            colored = self.copy_image(frame)
            colored = self.change(colored)
            blur = self.blurring(colored)
            thresh = self.threshold(blur)
            dilate = self.dilation(thresh)
            edges = self.edge_detection(dilate)
            ROI_vertices=[(0,height),(width,height),(width,height-300),(0,height-300)]
            ROI_image=self.ROI(edges,np.array([ROI_vertices],np.int32))
            lines_edges = self.hough_lines(ROI_image, frame)


            # frame=frame[int(self.height//2):,:]
            # edges = cv2.Canny(frame,100,200)
            # ret,thresh=cv2.threshold(edges, 100, 250, cv2.THRESH_BINARY)

            # lines = cv2.HoughLinesP(thresh, 1, np.pi / 180, 30,maxLineGap=100)
            
            # mask=np.zeros(shape=(self.height,self.width))

            # if lines is not None:
            #     for line in lines:
            #         x1, y1, x2, y2 = line[0]
            #         cv2.line(self.image[int(self.height//2):,:], (x1, y1), (x2, y2), (255, 255, 255), 3)
            #         cv2.line(mask[int(self.height//2):,:], (x1, y1), (x2, y2), (255, 255, 255), 3)

            self.pub.publish(self.br.cv2_to_imgmsg(lines_edges))
            cv2.imshow("frame",lines_edges)
            cv2.waitKey(1)


    def start(self):
        c=0
        while not rospy.is_shutdown():
            if self.image is not None:
                c=0
            self.loop_rate.sleep()

if __name__ == '__main__':
    rospy.init_node("lane_detect")
    my_node = Nodo()
    my_node.start()
