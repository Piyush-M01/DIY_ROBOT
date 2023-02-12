#!/usr/bin/env python3
import roslib
import numpy as np
import math
from mpmath import mp
import rospy
import cv2
from sensor_msgs.msg import LaserScan, Image
from cv_bridge import CvBridge, CvBridgeError

from dynamic_reconfigure.server import Server 
from lane_mapping.cfg import parametersConfig

class image_to_laser:

    def __init__(self):
        # Laser Scan To Subscribe to
        self.joy_sub = rospy.Subscriber("/detected_lanes",Image,self.img_to_laser_callback)
        # Publisher for Image
        self.pub = rospy.Publisher("/img_scan",LaserScan, queue_size = 100)
        # CvBridge Setup
        self.br = CvBridge()
        self.scan=LaserScan()
        self.scaling_factor=100000

        # disc factor = original length of object (metres) / width of the object in image
        self.disc_factor=0.0009525
        # self.disc_factor=0.81/640
        
        self.srv = Server(parametersConfig, self.callback)
        self.angle_min=-math.pi/2
        self.angle_max=math.pi/2
        self.samples=360
        # self.angle_increment=(abs(self.angle_max)+abs(self.angle_min))/self.samples
        self.angle_increment=1

    def callback(self,config, level):
        
        #find the appropriate scaling factor using dynamic reconfigure 
        self.scaling_factor=config.scaling_factor

        self.disc_factor=config.disc_factor
        # self.angle_increment=config.angle_increment
        # self.range_size=int(180/math.degrees(self.angle_increment))
        
        # print(self.scaling_factor)
        return config

    def convert_pix_to_coord(self,pix_x,pix_y):

        pt_x=(pix_x/self.disc_factor)
        pt_y=(pix_y/self.disc_factor)

        if(pt_x != 0):
            theta=math.atan(pt_y/pt_x)
        else:
            theta=math.pi/2

        r=pt_x/mp.cos(theta)
        theta=int(math.degrees(theta))


        return r,theta

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
            print("hough")
        except:
            print("Lines not detected")
        return frame,lines

    def img_to_laser_callback(self,msg):

        self.image = self.br.imgmsg_to_cv2(msg)
        self.height,self.width=self.image.shape[:2]
        # print(self.wid)
        # frame=cv2.cvtColor(self.image,cv2.COLOR_BGR2GRAY)
        # frame=frame[int(self.height//3):,:]
        # edges = cv2.Canny(frame,100,200)
        # ret,thresh=cv2.threshold(edges, 100, 250, cv2.THRESH_BINARY)

        frame=self.image
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
        lines_edges,lines = self.hough_lines(ROI_image, frame)

        ranges = dict()
        intensities=dict()


        for i in range(-180,180,1):
            ranges[i]=0
            intensities[i]=0

        # lines = cv2.HoughLinesP(thresh, 1, np.pi / 180, 30,maxLineGap=100)
        # cv2.imshow("SS",frame)
        # mask=np.zeros(shape=(self.height,self.width))

        # following specifications and lane detection code works 
        #
        if lines is not None:
            for line in lines:
                x1, y1, x2, y2 = line[0]
                r,theta=self.convert_pix_to_coord(x1,y1)
                ranges[theta]=abs(r)/self.scaling_factor
                intensities[theta]=10
                r,theta=self.convert_pix_to_coord(x2,y2)
                ranges[theta]=abs(r)/self.scaling_factor
                intensities[theta]=100
            
        self.scan.angle_max=math.pi/2
        self.scan.angle_min=-math.pi/2
        self.scan.angle_increment=0.017453


        # self.ranges=[]
        # for i in ranges.keys():
        #     self.ranges.append(ranges[i])

        # print(self.ranges)


        # this method makes code slower and the laser points are getting projected behind the bot
        # inv=cv2.flip(self.image,0)
        # coords=np.where(self.image == 255)
        # coordinates = zip(coords[0], coords[1])
        # j=1
        # for i in coordinates:
        #     # print("coords",i)
        #     # if j<50:
        #     r,theta=self.convert_pix_to_coord(i[0],i[1])
        #     ranges[theta]=abs(r)/self.scaling_factor
        #     intensities[theta]=10
        #         # j=j+1

        # self.scan.angle_max=3.14*4
        # self.scan.angle_min=-3.14*4
        # self.scan.angle_increment=-0.0174533

        # for i in ranges.values():
        #     print(i)
        # print(list(ranges.values()))



        self.scan.header.frame_id="laser"
        self.scan.header.stamp.secs=rospy.get_rostime().secs
        self.scan.header.stamp.nsecs=rospy.get_rostime().nsecs
        self.scan.time_increment=0.0
        self.scan.scan_time=0.0
        self.scan.range_min=0.04
        self.scan.range_max=10.0
        self.ranges=[]
        self.intensities=[]
        self.ranges=list(ranges.values())
        self.intensities=list(intensities.values())
        self.scan.ranges=self.ranges
        self.scan.intensities=self.intensities
        self.pub.publish(self.scan)


if __name__=='__main__':
	rospy.init_node('image_to_laser')
	laser_to_image = image_to_laser()
	rospy.spin()    
