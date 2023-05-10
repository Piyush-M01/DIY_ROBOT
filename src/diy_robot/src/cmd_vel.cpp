#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include <tf/transform_broadcaster.h>
#include "std_msgs/Float64.h"
#include <nav_msgs/Odometry.h>

class Broadcaster
{
  public:
    double x,y,th,radius=0.127;
    double vr=0,vl=0,delta_x=0,delta_y=0,delta_th=0;
    bool left_ack=false,right_ack=false;
    double linear=0, angular=0;
    Broadcaster(ros::NodeHandle);
    void Calc_Left(const std_msgs::Float64& leftCount);
    void Calc_Right(const std_msgs::Float64& rightCount);
    void set_initial_2d(const geometry_msgs::PoseStamped &rvizClick);
    ros::Time current_time, last_time;
    ros::Publisher odom_pub; 
    tf::TransformBroadcaster odom_broadcaster;
    ros::Subscriber subForRightCounts,subForLeftCounts,subInitialPose;
    geometry_msgs::TransformStamped odom_trans;
    nav_msgs::Odometry odom;
    bool initialPoseRecieved = false;
    void updateOdom();
    void publish_odom();
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
  subInitialPose = n.subscribe("initial_2d", 1, &Broadcaster::set_initial_2d,this);
  subForRightCounts = n.subscribe("right_ticks", 1, &Broadcaster::Calc_Right,this);
  subForLeftCounts = n.subscribe("left_ticks", 1, &Broadcaster::Calc_Left,this);
  odom_pub = n.advertise<nav_msgs::Odometry>("odom", 1);
}

void Broadcaster::set_initial_2d(const geometry_msgs::PoseStamped &rvizClick) {
 
  odom.pose.pose.position.x = rvizClick.pose.position.x;
  odom.pose.pose.position.y = rvizClick.pose.position.y;
  odom.pose.pose.orientation.z = rvizClick.pose.orientation.z;
  initialPoseRecieved = true;
}

void Broadcaster::Calc_Left(const std_msgs::Float64& leftCount) {
    vl = leftCount.data;
    left_ack=true;

}

// Calculate the distance the right wheel has traveled since the last cycle
void Broadcaster::Calc_Right(const std_msgs::Float64& rightCount) {
    vr = rightCount.data;
    right_ack=true;
}

void Broadcaster::updateOdom()
{

  // if (abs(vr-vl)<0.05)
  //     vl = vr;

  linear = (vr+vl)/2;
  angular = (vr - vl)/0.8;

  current_time = ros::Time::now();
  delta_x = ((vr + vl) * (radius/2)) * cos(th) * 2.67;  //2.67 is a multiplication factor we calculated from the time taken by stm to send us new calculated value
  delta_y = ((vr + vl) * (radius/2)) * sin(th) * 2.67;
  delta_th = -((vr - vl) * (radius/0.8)) * 2.67;

  x += delta_x;
  y += delta_y;

  th += delta_th;

  ROS_INFO("x: %f y: %f z: %f",x,y,th);

  if(th > 3.14)
    th-= 2 * 3.14;
  else if (th < -3.14)
    th += 2 * 3.14;

  last_time=current_time;

}

void Broadcaster::publish_odom()
{

  tf2::Quaternion q;

  q.setRPY(0, 0, odom.pose.pose.orientation.z);

  nav_msgs::Odometry quatOdom;

  geometry_msgs::TransformStamped odom_trans;
  odom_trans.header.stamp = ros::Time::now();
  odom_trans.header.frame_id = "odom";
  odom_trans.child_frame_id = "base_link";
  odom_trans.transform.translation.x = odom.pose.pose.position.x;
  odom_trans.transform.translation.y = odom.pose.pose.position.y;
  odom_trans.transform.translation.z = 0.0;
  // odom_trans.transform.rotation.z=odom.pose.pose.orientation.z;
  odom_trans.transform.rotation = tf::createQuaternionMsgFromYaw(th);

  //send the transform
  odom_broadcaster.sendTransform(odom_trans);

  // since all odometry is 6DOF we'll need a quaternion created from yaw
  geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th);

  //next, we'll publish the odometry message over ROS
  odom.header.stamp = current_time;
  odom.header.frame_id = "odom";

  //set the position
  odom.pose.pose.position.x = x;
  odom.pose.pose.position.y = y;
  odom.pose.pose.position.z = 0.0;
  odom.pose.pose.orientation = odom_quat;

  // //set the velocity
  odom.child_frame_id = "base_link";
  odom.twist.twist.linear.x = linear;
  odom.twist.twist.linear.y = 0;
  odom.twist.twist.angular.z = angular;

  for(int i = 0; i<36; i++) {
        if(i == 0 || i == 7 || i == 14) {
            odom.pose.covariance[i] = .01;
        }
        else if (i == 21 || i == 28 || i== 35) {
            odom.pose.covariance[i] += 0.1;
        }
        else {
            odom.pose.covariance[i] = 0;
        }
    }

  odom_pub.publish(odom);
  last_time = current_time;

}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "velocity");
  ros::NodeHandle n;
  Broadcaster ob(n);
  ob.publish_odom();
  ros::Rate rate(100);
  while(true)
  {

    ob.current_time = ros::Time::now();

    if (ob.left_ack && ob.right_ack)
    {
        ob.updateOdom();
        ob.left_ack=false;
        ob.right_ack=false;
    }  
    ob.publish_odom();
    rate.sleep();
    ros::spinOnce();
  }
  return 0;
}