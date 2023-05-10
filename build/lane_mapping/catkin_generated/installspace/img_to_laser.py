#!/usr/bin/env python3
import roslib
import numpy as np
import math
from mpmath import mp
import tf2_ros
import geometry_msgs.msg
import tf
import rospy
import cv2
from sensor_msgs.msg import LaserScan, Image
from cv_bridge import CvBridge, CvBridgeError

from dynamic_reconfigure.server import Server 
from lane_mapping.cfg import parametersConfig

# scan.time_increment = (1 / laser_frequency) / (num_readings); 

class image_to_laser:

    def __init__(self):
        # CvBridge Setup
        self.br = CvBridge()
        
        self.scaling_factor=816303

        # disc factor = original length of object (metres) / width of the object in image
        self.disc_factor=0.0009525
        
        self.laser_frequency = 40
        
        self.srv = Server(parametersConfig, self.callback)
        self.angle_min=-math.pi/2
        self.angle_max=math.pi/2
        self.samples=180
        self.loop_rate = rospy.Rate(20)
        self.joy_sub = rospy.Subscriber("/detected_lanes",Image,self.img_to_laser_callback)
        self.pub = rospy.Publisher("/img_scan",LaserScan, queue_size = 20)


    def callback(self,config, level):
        
        #find the appropriate scaling factor using dynamic reconfigure 
        self.scaling_factor=config.scaling_factor

        self.disc_factor=config.disc_factor

        return config

    def convert_pix_to_coord(self,pix_x,pix_y):
        
        # self.disc_factor = 250000
        pt_x=(pix_x/self.disc_factor)
        pt_y=(pix_y/self.disc_factor)


        if(pt_x != 0):
            theta=math.atan((pt_y ) / (pt_x))
        else:
            theta=math.pi/2

        r=abs(pt_x/mp.cos(theta))

        theta=int(math.degrees(theta))

        if theta<0:
            theta = 90 + (90-abs(theta))

        return r,theta

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
        # ret,thresh = cv2.threshold(frame,120,255,cv2.THRESH_BINARY)
        ret,thresh = cv2.threshold(frame,100,250,cv2.THRESH_BINARY)
        return thresh
    
    def dilation(self,frame):
        kernel = (3,3)
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
            return lines_edges,lines
        except:
            print("Lines not detected")
            return frame,lines
        
    def send_transform(self):

        broadcaster = tf2_ros.TransformBroadcaster()
        static_transformStamped = geometry_msgs.msg.TransformStamped()

        static_transformStamped.header.stamp = rospy.Time.now()
        static_transformStamped.header.frame_id = "base_link"
        static_transformStamped.child_frame_id = "laser"

        x = 0
        y = 0
        z = 0.25

        roll = 0
        pitch = 0
        yaw = 0

        static_transformStamped.transform.translation.x = float(x)
        static_transformStamped.transform.translation.y = float(y)
        static_transformStamped.transform.translation.z = float(z)

        quat = tf.transformations.quaternion_from_euler(
                   float(roll),float(pitch),float(yaw))
        static_transformStamped.transform.rotation.x = quat[0]
        static_transformStamped.transform.rotation.y = quat[1]
        static_transformStamped.transform.rotation.z = quat[2]
        static_transformStamped.transform.rotation.w = quat[3]

        broadcaster.sendTransform(static_transformStamped)

    def img_to_laser_callback(self,msg):

        self.send_transform()

        self.image = self.br.imgmsg_to_cv2(msg)
        self.height,self.width=self.image.shape[:2]

        frame=self.image
        frame = cv2.flip(frame, 0)

        # comment when using actual camera
        frame = cv2.flip(frame, 1)

        # frame =cv2.convertScaleAbs(frame, 0.5, 0.8)
        height=frame.shape[0]
        width=frame.shape[1]
        colored = self.copy_image(frame)
        colored = self.change(colored)
        blur = self.blurring(colored)
        thresh = self.threshold(blur)
        dilate = self.dilation(thresh)
        edges = self.edge_detection(dilate)
        # ROI_vertices=[(0,height),(width,height),(width,height-300),(0,height-300)]
        # ROI_image=self.ROI(edges,np.array([ROI_vertices],np.int32))
        lines_edges,lines = self.hough_lines(edges, frame)

        ranges = dict()
        intensities=dict()
        
        cv2.imshow("lanes",lines_edges)
        cv2.waitKey(1)

        for i in range(0,360,1):
            ranges[i]=0
            intensities[i]=0

        # following specifications and lane detection code works 
        #
        if lines is not None:
            for line in lines:
                x1, y1, x2, y2 = line[0]
                r,theta=self.convert_pix_to_coord(self.width/2-x1,y1)
                ranges[theta]=abs(r)/self.scaling_factor
                intensities[theta]=10
                r,theta=self.convert_pix_to_coord(self.width/2-x2,y2)
                ranges[theta]=abs(r)/self.scaling_factor
                intensities[theta]=100
        
        current_time = rospy.Time.now()
        self.scan=LaserScan()
        self.scan.angle_max=math.pi/2
        self.scan.angle_min=-math.pi/2
        self.scan.angle_increment=0.017453
        self.scan.time_increment = (1 / self.laser_frequency) / (360)
        self.scan.header.frame_id="laser"        
        self.scan.range_min=0.04
        self.scan.range_max=100.0
        self.scan.header.stamp = current_time
        self.ranges=[]
        self.intensities=[]
        self.ranges=list(ranges.values())
        self.intensities=list(intensities.values())
        self.scan.ranges=self.ranges
        self.scan.intensities=self.intensities
        self.pub.publish(self.scan)

    def start(self):
        while not rospy.is_shutdown():
            self.loop_rate.sleep()


if __name__=='__main__':
	rospy.init_node('image_to_laser')
	laser_to_image = image_to_laser().start()
	# rospy.spin()    
