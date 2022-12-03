#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>

class Broadcaster
{
  public:
    double x,y,th,vl,vr,vx,vy,vth;
    int count=0;
    Broadcaster(ros::NodeHandle);
    void velocityCallback(const geometry_msgs::Twist& msg);
    ros::Time current_time, last_time;
    ros::Publisher odom_pub; 
    tf::TransformBroadcaster odom_broadcaster;
    ros::Subscriber sub;
    geometry_msgs::TransformStamped odom_trans;
    nav_msgs::Odometry odom;
    void updateOdom(Broadcaster &ob);
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
  vx=0;
  vth=0;
  vy=0;
  sub = n.subscribe("cmd_vel", 1000, &Broadcaster::velocityCallback,this);
  odom_pub = n.advertise<nav_msgs::Odometry>("odom", 50);
  
}

void Broadcaster::velocityCallback(const geometry_msgs::Twist& msg)
{
    

    double length=0.15; //length in metres
    // vl=msg.linear.x-(msg.angular.z*(length/2));
    // vr=msg.linear.x+(msg.angular.z*(length/2));

    vx = msg.linear.x;
    vy = msg.linear.y;
    vth = msg.angular.z;

}

void Broadcaster::updateOdom(Broadcaster &ob)
{

  double dt = (ob.current_time - ob.last_time).toSec();
  double delta_x = (ob.vx * cos(ob.th) - ob.vy * sin(ob.th)) * dt;
  double delta_y = (ob.vx * sin(ob.th) + ob.vy * cos(ob.th)) * dt;
  double delta_th = ob.vth * dt;

  ob.x += delta_x;
  ob.y += delta_y;
  ob.th += delta_th;

  //since all odometry is 6DOF we'll need a quaternion created from yaw
  geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(ob.th);

  //next, we'll publish the odometry message over ROS
  ob.odom.header.stamp = ob.current_time;
  ob.odom.header.frame_id = "odom";

  //set the position
  ob.odom.pose.pose.position.x = ob.x;
  ob.odom.pose.pose.position.y = ob.y;
  ob.odom.pose.pose.position.z = 0.0;
  ob.odom.pose.pose.orientation = odom_quat;

  //set the velocity
  ob.odom.child_frame_id = "base_link";
  ob.odom.twist.twist.linear.x = ob.vx;
  ob.odom.twist.twist.linear.y = ob.vy;
  ob.odom.twist.twist.angular.z = ob.vth;


  ob.odom_pub.publish(ob.odom);
  ob.last_time = ob.current_time;

}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "velocity");
  ros::NodeHandle n;
  Broadcaster ob(n);

  ob.current_time = ros::Time::now();
  ob.last_time=ros::Time::now();

  while(true)
  {

    ob.current_time = ros::Time::now();

    ob.updateOdom(ob);   
    
    //publish the message
    ob.odom_pub.publish(ob.odom);

    ros::spinOnce();
  }
  return 0;
}