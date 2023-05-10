#!/usr/bin/env python3

import rospy
from geometry_msgs.msg import PointStamped
import actionlib
import math
from nav_msgs.msg import Odometry
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal

class WaypointPlanner:

    def __init__(self):
        self.waypoints = []
        self.status = ""
        rospy.Subscriber('/clicked_point', PointStamped, self.clicked_point_callback)
        rospy.Subscriber('/odom', Odometry, self.getPose)

    def movebase_client(self,waypoint):
        
        print("sending new goal")

        client = actionlib.SimpleActionClient('move_base',MoveBaseAction)
        client.wait_for_server()

        goal = MoveBaseGoal()
        goal.target_pose.header.frame_id = "map"
        goal.target_pose.header.stamp = rospy.Time.now()
        goal.target_pose.pose.position.x = waypoint[0]
        goal.target_pose.pose.position.y = waypoint[1]
        goal.target_pose.pose.orientation.w = 1

        client.send_goal(goal)
        self.status = "busy"


    def clicked_point_callback(self, msg):

        # Extract x, y coordinates from clicked point message
        x = msg.point.x
        y = msg.point.y
        z = msg.point.z

        # Create new waypoint and append it to list of waypoints
        waypoint = [x, y, z]
        self.waypoints.append(waypoint)

        if len(self.waypoints) > 0 and self.status!="busy":
            self.movebase_client(self.waypoints[0])

    def getPose(self,msg):

        if len(self.waypoints)>0:
            if math.sqrt((abs(msg.pose.pose.position.x) - abs(self.waypoints[0][0]))**2 + (abs(msg.pose.pose.position.y) - abs(self.waypoints[0][1]))**2) < 5.0: 
                self.waypoints.pop(0)
                self.status = ""
                print("freed")
                self.movebase_client(self.waypoints[0])

if __name__ == '__main__':
    rospy.init_node('waypoint_planner', anonymous=True)
    wp_planner = WaypointPlanner()
    rospy.spin()
