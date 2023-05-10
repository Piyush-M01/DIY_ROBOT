#include <ros/ros.h>

#include <stdlib.h>
#include <unistd.h>

#include "serial_comm/Ticks.h"

#include <std_msgs/Bool.h>
#include <std_msgs/Int32.h>
#include <geometry_msgs/Vector3.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <cmath>
#include <boost/assign/list_of.hpp>



#define PI 3.14159265
#define TwoPI 6.28318531
#define wheeltrack 0.94   // units : m  
#define EncoderCountsPerWheel 2000
#define wheelradius 0.127  // units : m


long _PreviousLeftEncoderCounts = 0;
long _PreviousRightEncoderCounts = 0;
ros::Time current_time, last_time;
double DistancePerCount = (3.14159265 * 2 * wheelradius) / (EncoderCountsPerWheel * 4); //the wheel diameter is 0.1m

//final odometric datas
double x;
double y;
double th;
double v_left;//left motor speed
double v_right;//right motor speed
double vth;//angular velocity of robot
double deltaLeft;//no of ticks in left encoder since last update
double deltaRight;//no of ticks in right encoder since last update
double dt;
double delta_distance;//distance moved by robot since last update
double delta_th;//corresponging change in heading
double delta_x ;//corresponding change in x direction
double delta_y;//corresponding change in y direction


void WheelCallback(const serial_comm::TicksPtr& motor)
{
  current_time = ros::Time::now();

  deltaLeft = motor->left - _PreviousLeftEncoderCounts;
  deltaRight = motor->right - _PreviousRightEncoderCounts;

  dt = (current_time - last_time).toSec();

  v_left = deltaLeft * DistancePerCount/dt;    //speed = m/s
  v_right = deltaRight * DistancePerCount/dt;  //speed = m/s

  delta_distance=0.5f * (double)(deltaLeft + deltaRight) * DistancePerCount;
  delta_th = (double)(deltaRight-deltaLeft)*DistancePerCount/wheeltrack; //Distance between the two wheels is 0.36m
  
  delta_x = delta_distance*(double)cos(th);
  delta_y = delta_distance*(double)sin(th);

  x += delta_x;
  y += delta_y;
  th += delta_th;
 
  // if (th > PI)
  //   th -= TwoPI;
  // else if ( th <= -PI)
  //   th += TwoPI;
  
  _PreviousLeftEncoderCounts = motor->left;
  _PreviousRightEncoderCounts = motor->right;

  last_time = current_time;
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "odometry_publisher");
  ros::NodeHandle nh;
  // ros::Subscriber sub = nh.subscribe("pos", 100, WheelCallback);
  ros::Subscriber motor_encoder_sub = nh.subscribe("/motor_encoder", 50, WheelCallback);
  ros::Publisher odom_pub = nh.advertise<nav_msgs::Odometry>("odom", 50);
  tf::TransformBroadcaster odom_broadcaster;

  ros::Rate r(100);

  while(nh.ok())
  {
    //since all odometry is 6DO F we'll need a quaternion created from yaw
    geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th);

    //first, we'll publish the transform over tf
    geometry_msgs::TransformStamped odom_trans;
    odom_trans.header.stamp = current_time;
    odom_trans.header.frame_id = "odom";
    odom_trans.child_frame_id = "base_link";

    odom_trans.transform.translation.x = x;
    odom_trans.transform.translation.y = y;

    // ROS_INFO("x %d",x);
    // ROS_INFO("y %d",y);
    std::cout<<x;

    odom_trans.transform.translation.z = 0.0;
    odom_trans.transform.rotation = odom_quat;

    //send the transform
    odom_broadcaster.sendTransform(odom_trans);

    //next, we'll publish the odometry message over ROS
    nav_msgs::Odometry odom;
    odom.header.stamp = current_time;
    odom.header.frame_id = "odom";
    //set the position
    odom.pose.pose.position.x = x;
    odom.pose.pose.position.y = y;
    odom.pose.pose.position.z = 0.0;
    odom.pose.pose.orientation = odom_quat;
    odom.pose.covariance =  boost::assign::list_of(1e-3) (0)   (0)  (0)  (0)  (0)
                                                   (0) (1e-3)  (0)  (0)  (0)  (0)
                                                   (0)   (0)  (1e6) (0)  (0)  (0)
                                                   (0)   (0)   (0) (1e6) (0)  (0)
                                                   (0)   (0)   (0)  (0) (1e6) (0)
                                                   (0)   (0)   (0)  (0)  (0)  (1e3) ;
    //set the velocity
    odom.child_frame_id = "base_link";
    odom.twist.twist.linear.x=delta_x/dt;
    odom.twist.twist.linear.y=delta_y/dt;
    odom.twist.twist.angular.z = delta_th/dt;
    odom.twist.covariance =  boost::assign::list_of(1e-3) (0)   (0)  (0)  (0)  (0)
                                                    (0) (1e-3)  (0)  (0)  (0)  (0)
                                                    (0)   (0)  (1e6) (0)  (0)  (0)
                                                    (0)   (0)   (0) (1e6) (0)  (0)
                                                    (0)   (0)   (0)  (0) (1e6) (0)
                                                    (0)   (0)   (0)  (0)  (0)  (1e3) ; 

    // ROS_INFO("Position x = %d and Position y = %d ",x,y);

    //publish the message
    odom_pub.publish(odom);

    last_time = current_time;
    ros::spinOnce();
    r.sleep();
    }
}











// #include "ros/ros.h"
// #include "std_msgs/Int16.h"
// #include <nav_msgs/Odometry.h>
// #include <geometry_msgs/PoseStamped.h>
// #include <tf2/LinearMath/Quaternion.h>
// #include <tf2_ros/transform_broadcaster.h>
// #include <cmath>

// // Create odometry data publishers
// ros::Publisher odom_data_pub;
// ros::Publisher odom_data_pub_quat;
// nav_msgs::Odometry odomNew;
// nav_msgs::Odometry odomOld;

// // Initial pose
// const double initialX = 0.0;
// const double initialY = 0.0;
// const double initialTheta = 0.00000000001;
// const double PI = 3.141592;

// // Robot physical constants
// const double TICKS_PER_REVOLUTION = 2000; // For reference purposes.
// const double WHEEL_RADIUS = 0.127; // Wheel radius in meters
// const double WHEEL_BASE = 0.94; // Center of left tire to center of right tire
// const double TICKS_PER_METER = 2506.37705656528; // Original was 2800

// // Distance both wheels have traveled
// double distanceLeft = 0;
// double distanceRight = 0;

// static int lastCountR = 0;
// static int lastCountL = 0;

// int l_position=0,r_position=0;

// int32_t dl_ticks=0,dr_ticks=0;
// int theta=0;

// // Flag to see if initial pose has been received
// bool initialPoseRecieved = false;

// using namespace std;

// // Get initial_2d message from either Rviz clicks or a manual pose publisher
// void set_initial_2d(const geometry_msgs::PoseStamped &rvizClick) {

//     odomOld.pose.pose.position.x = rvizClick.pose.position.x;
//     odomOld.pose.pose.position.y = rvizClick.pose.position.y;
//     odomOld.pose.pose.orientation.z = rvizClick.pose.orientation.z;
//     initialPoseRecieved = true;
// }

// // Calculate the distance the left wheel has traveled since the last cycle
// void Calc_Left(const std_msgs::Int16& leftCount) {

//     int32_t l_ticks = lastCountL;

//     l_position = (2.0 * M_PI) * l_ticks / TICKS_PER_REVOLUTION;

//     dl_ticks = l_ticks - lastCountL;

// }

// // Calculate the distance the right wheel has traveled since the last cycle
// void Calc_Right(const std_msgs::Int16& rightCount) {


//     int32_t r_ticks = lastCountR;

//     l_position = (2.0 * M_PI) * r_ticks / TICKS_PER_REVOLUTION;

//     dl_ticks = r_ticks - lastCountR;

// }

// // Publish a nav_msgs::Odometry message in quaternion format
// void publish_quat() {
//     tf2::Quaternion q;

//     q.setRPY(0, 0, odomNew.pose.pose.orientation.z);

//     nav_msgs::Odometry quatOdom;

//     quatOdom.header.stamp = odomNew.header.stamp;
//     quatOdom.header.frame_id = "odom";
//     quatOdom.child_frame_id = "base_link";
//     quatOdom.pose.pose.position.x = odomNew.pose.pose.position.x;
//     quatOdom.pose.pose.position.y = odomNew.pose.pose.position.y;
//     quatOdom.pose.pose.position.z = odomNew.pose.pose.position.z;
//     quatOdom.pose.pose.orientation.x = q.x();
//     quatOdom.pose.pose.orientation.y = q.y();
//     quatOdom.pose.pose.orientation.z = q.z();
//     quatOdom.pose.pose.orientation.w = q.w();
//     quatOdom.twist.twist.linear.x = odomNew.twist.twist.linear.x;
//     quatOdom.twist.twist.linear.y = odomNew.twist.twist.linear.y;
//     quatOdom.twist.twist.linear.z = odomNew.twist.twist.linear.z;
//     quatOdom.twist.twist.angular.x = odomNew.twist.twist.angular.x;
//     quatOdom.twist.twist.angular.y = odomNew.twist.twist.angular.y;
//     quatOdom.twist.twist.angular.z = odomNew.twist.twist.angular.z;

//     for(int i = 0; i<36; i++) {
//         if(i == 0 || i == 7 || i == 14) {
//             quatOdom.pose.covariance[i] = .01;
//         }
//         else if (i == 21 || i == 28 || i== 35) {
//             quatOdom.pose.covariance[i] += 0.1;
//         }
//         else {
//             quatOdom.pose.covariance[i] = 0;
//         }
//     }
//         odom_data_pub_quat.publish(quatOdom);
// }

// // Update odometry information
// void update_odom() {

//     float delta_l = (2 * M_PI * WHEEL_RADIUS * dl_ticks) / TICKS_PER_REVOLUTION;
//     float delta_r = (2 * M_PI * WHEEL_RADIUS * dr_ticks) / TICKS_PER_REVOLUTION;
//     float delta_center = (delta_l + delta_r) / 2;

//     // Calculate the average distance
//     double cycleDistance = (distanceRight + distanceLeft) / 2;

//     // Calculate the number of radians the robot has turned since the last cycle
//     double cycleAngle = asin((distanceRight-distanceLeft)/WHEEL_BASE);

//     // Average angle during the last cycle
//     double avgAngle = cycleAngle/2 + odomOld.pose.pose.orientation.z;

//     if (avgAngle > PI) {
//         avgAngle -= 2*PI;
//     }
//     else if (avgAngle < -PI) {
//         avgAngle += 2*PI;
//     }
//     else{}

//     // Calculate the new pose (x, y, and theta)
//     odomNew.pose.pose.position.x += delta_center * cosf(theta);
//     odomNew.pose.pose.position.y += delta_center * cosf(theta);
//     theta += (delta_r - delta_l) / WHEEL_BASE;
//     odomNew.pose.pose.position.z = theta;
    

//     // Prevent lockup from a single bad cycle
    
//     if (isnan(odomNew.pose.pose.position.x) || isnan(odomNew.pose.pose.position.y)
//         || isnan(odomNew.pose.pose.position.z)) {
//     odomNew.pose.pose.position.x = odomOld.pose.pose.position.x;
//     odomNew.pose.pose.position.y = odomOld.pose.pose.position.y;
//     odomNew.pose.pose.orientation.z = odomOld.pose.pose.orientation.z;

// }

//     // Make sure theta stays in the correct range
//     if (odomNew.pose.pose.orientation.z > PI) {
//         odomNew.pose.pose.orientation.z -= 2 * PI;
//     }
//     else if (odomNew.pose.pose.orientation.z < -PI) {
//         odomNew.pose.pose.orientation.z += 2 * PI;
//     }
//     else{}

//     // Compute the velocity
//     odomNew.header.stamp = ros::Time::now();
//     odomNew.twist.twist.linear.x = cycleDistance/(odomNew.header.stamp.toSec() - odomOld.header.stamp.toSec());
//     odomNew.twist.twist.angular.z = cycleAngle/(odomNew.header.stamp.toSec() - odomOld.header.stamp.toSec());

//     // Save the pose data for the next cycle
//     odomOld.pose.pose.position.x = odomNew.pose.pose.position.x;
//     odomOld.pose.pose.position.y = odomNew.pose.pose.position.y;
//     odomOld.pose.pose.orientation.z = odomNew.pose.pose.orientation.z;
//     odomOld.header.stamp = odomNew.header.stamp;

//     // Publish the odometry message
//     odom_data_pub.publish(odomNew);
// }

// int main(int argc, char **argv) {

//     // Set the data fields of the odometry message
//     odomNew.header.frame_id = "odom";
//     odomNew.pose.pose.position.z = 0;
//     odomNew.pose.pose.orientation.x = 0;
//     odomNew.pose.pose.orientation.y = 0;
//     odomNew.twist.twist.linear.x = 0;
//     odomNew.twist.twist.linear.y = 0;
//     odomNew.twist.twist.linear.z = 0;
//     odomNew.twist.twist.angular.x = 0;
//     odomNew.twist.twist.angular.y = 0;
//     odomNew.twist.twist.angular.z = 0;
//     odomOld.pose.pose.position.x = initialX;
//     odomOld.pose.pose.position.y = initialY;
//     odomOld.pose.pose.orientation.z = initialTheta; 

//     // Launch ROS and create a node
//     ros::init(argc, argv, "ekf_odom_pub");
//     ros::NodeHandle node;  

//     // Subscribe to ROS topics
//     ros::Subscriber subForRightCounts = node.subscribe("right_ticks", 100, Calc_Right, ros::TransportHints().tcpNoDelay());
//     ros::Subscriber subForLeftCounts = node.subscribe("left_ticks", 100, Calc_Left, ros::TransportHints().tcpNoDelay());
//     ros::Subscriber subInitialPose = node.subscribe("initial_2d", 1, set_initial_2d);

//     // Publisher of simple odom message where orientation.z is an euler angle
//     odom_data_pub = node.advertise<nav_msgs::Odometry>("odom_data_euler", 100);

//     // Publisher of full odom message where orientation is quaternion
//     odom_data_pub_quat = node.advertise<nav_msgs::Odometry>("odom_data_quat", 100);

//     ros::Rate loop_rate(10); 

//     while(ros::ok()) {

//     if(initialPoseRecieved) {
//         update_odom();
//         publish_quat();
//     }
//     ros::spinOnce();
//     loop_rate.sleep();
//     }

//     return 0;
// }


// /*

// #include "ros/ros.h"
// #include <geometry_msgs/Vector3.h>
// #include <tf/transform_broadcaster.h>
// #include <nav_msgs/Odometry.h>

// long _PreviousLeftEncoderCounts = 0;
// long _PreviousRightEncoderCounts = 0;
// ros::Time current_time, last_time;
// double DistancePerCount = (3.14159265 * 0.1)/30; //the wheel diameter is 0.1m
// //final odometric datas
// double x;
// double y;
// double th;
// double v_left;//left motor speed
// double v_right;//right motor speed
// double vth;//angular velocity of robot
// double deltaLeft;//no of ticks in left encoder since last update
// double deltaRight;//no of ticks in right encoder since last update
// double dt;
// double delta_distance;//distance moved by robot since last update
// double delta_th;//corresponging change in heading
// double delta_x ;//corresponding change in x direction
// double delta_y;//corresponding change in y direction
// #define PI 3.14159265
// #define TwoPI 6.28318531


// void WheelCallback(const geometry_msgs::Vector3::ConstPtr& ticks)
// {
//  current_time = ros::Time::now();
//  deltaLeft = ticks->x - _PreviousLeftEncoderCounts;
//  deltaRight = ticks->y - _PreviousRightEncoderCounts;
//  dt = (current_time - last_time).toSec();
//  v_left = deltaLeft * DistancePerCount/dt;
//  v_right = deltaRight * DistancePerCount/dt;
//  delta_distance=0.5f*(double)(deltaLeft+deltaRight)*DistancePerCount;
//  delta_th = (double)(deltaRight-deltaLeft)*DistancePerCount/0.36f; //Distance between the two wheels is 0.36m
//  delta_x = delta_distance*(double)cos(th);
//  delta_y = delta_distance*(double)sin(th);
//  x += delta_x;
//  y += delta_y;
//  th += delta_th;
//  if (th > PI)
//  th -= TwoPI;
//  else
//  if ( th <= -PI)
//  th += TwoPI;
//  _PreviousLeftEncoderCounts = ticks->x;
//  _PreviousRightEncoderCounts = ticks->y;
//  last_time = current_time;
// }


// int main(int argc, char **argv)
// {
//  ros::init(argc, argv, "odometry_publisher");
//  ros::NodeHandle n;
//  ros::Subscriber sub = n.subscribe("pos", 100, WheelCallback);
//  ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 50);
//  tf::TransformBroadcaster odom_broadcaster;
//  ros::Rate r(40);

//  while(n.ok()){
//     //since all odometry is 6DOF we'll need a quaternion created from yaw
//     geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th);
//     //first, we'll publish the transform over tf
//     geometry_msgs::TransformStamped odom_trans;
//     odom_trans.header.stamp = current_time;
//     odom_trans.header.frame_id = "odom";
//     odom_trans.child_frame_id = "base_link";
//     odom_trans.transform.translation.x = x;
//     odom_trans.transform.translation.y = y;
//     odom_trans.transform.translation.z = 0.0;
//     odom_trans.transform.rotation = odom_quat;
//     //send the transform
//     odom_broadcaster.sendTransform(odom_trans);
//     //next, we'll publish the odometry message over ROS
//     nav_msgs::Odometry odom;
//     odom.header.stamp = current_time;
//     odom.header.frame_id = "odom";
//     //set the position
//     odom.pose.pose.position.x = x;
//     odom.pose.pose.position.y = y;
//     odom.pose.pose.position.z = 0.0;
//     odom.pose.pose.orientation = odom_quat;
//     //set the velocity
//     odom.child_frame_id = "base_link";
//     odom.twist.twist.linear.x=delta_x/dt;
//     odom.twist.twist.linear.y=delta_y/dt;
//     odom.twist.twist.angular.z = delta_th/dt; 
    
//     ROS_INFO("Position x = %d and Position y = %d ",x,y);
	
//     //publish the message
//     odom_pub.publish(odom);
//     last_time = current_time;
//     ros::spinOnce();
//     r.sleep();
//     }
// }

// */