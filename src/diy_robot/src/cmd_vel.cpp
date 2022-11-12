#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>

class Broadcaster
{
  public:
    double x,y,th,vl,vr;
    Broadcaster(ros::NodeHandle);
    void velocityCallback(const geometry_msgs::Twist& msg);
  private:
    ros::Time current_time, last_time;
    ros::Publisher odom_pub; 
    tf::TransformBroadcaster odom_broadcaster;
    ros::Subscriber sub;
    geometry_msgs::TransformStamped odom_trans;
    nav_msgs::Odometry odom;
};

Broadcaster::Broadcaster(ros::NodeHandle n)
{
  current_time = ros::Time::now();
  last_time = ros::Time::now();
  x=0;
  y=0;
  th=0;
  vl=0;
  vr=0;
  sub = n.subscribe("cmd_vel", 1000, &Broadcaster::velocityCallback,this);
  odom_pub = n.advertise<nav_msgs::Odometry>("odom", 50);
  
}

void Broadcaster::velocityCallback(const geometry_msgs::Twist& msg)
{
    current_time = ros::Time::now();

    double length=0.15; //length in metres
    vl=msg.linear.x-(msg.angular.z*(length/2));
    vr=msg.linear.x+(msg.angular.z*(length/2));

    double vx = msg.linear.x;
    double vy = msg.linear.y;
    double vth = msg.angular.z;

    double dt = (current_time - last_time).toSec();
    double delta_x = (vx * cos(th) - vy * sin(th)) * dt;
    double delta_y = (vx * sin(th) + vy * cos(th)) * dt;
    double delta_th = vth * dt;

    x += delta_x;
    y += delta_y;
    th += delta_th;

    //since all odometry is 6DOF we'll need a quaternion created from yaw
    geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th);

    //first, we'll publish the transform over tf
    
    odom_trans.header.stamp = current_time;
    odom_trans.header.frame_id = "odom";
    odom_trans.child_frame_id = "base_link";

    odom_trans.transform.translation.x = x;
    odom_trans.transform.translation.y = y;
    odom_trans.transform.translation.z = 0.0;
    odom_trans.transform.rotation = odom_quat;

    //send the transform
    odom_broadcaster.sendTransform(odom_trans);

    //next, we'll publish the odometry message over ROS
    odom.header.stamp = current_time;
    odom.header.frame_id = "odom";

    //set the position
    odom.pose.pose.position.x = x;
    odom.pose.pose.position.y = y;
    odom.pose.pose.position.z = 0.0;
    odom.pose.pose.orientation = odom_quat;

    //set the velocity
    odom.child_frame_id = "base_link";
    odom.twist.twist.linear.x = vx;
    odom.twist.twist.linear.y = vy;
    odom.twist.twist.angular.z = vth;

    //publish the message
    odom_pub.publish(odom);

    last_time = current_time;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "velocity");
  ros::NodeHandle n;
  Broadcaster ob(n);
  ros::spin();
  return 0;
}