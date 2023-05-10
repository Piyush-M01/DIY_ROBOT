#include <ros/ros.h>
#include <tf/transform_broadcaster.h>

int main(int argc, char** argv){
  ros::init(argc, argv, "sbg_tf_publisher");
  ros::NodeHandle n;

  ros::Rate r(100);

  tf::TransformBroadcaster broadcaster,odom_to_base;

  while(n.ok()){
    broadcaster.sendTransform(
      tf::StampedTransform(
        tf::Transform(tf::Quaternion(0, 0, 0), tf::Vector3(0,0,0)),
        ros::Time::now(),"imu_link_ned","odom"));
    
    // odom_to_base.sendTransform(
    //   tf::StampedTransform(
    //     tf::Transform(tf::Quaternion(0, 0, 0), tf::Vector3(0,0,0)),
    //     ros::Time::now(),"odom_ekf","base_link"));

    r.sleep();
  }
}