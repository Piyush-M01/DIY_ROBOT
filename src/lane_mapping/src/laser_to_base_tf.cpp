#include <ros/ros.h>
#include <tf/transform_broadcaster.h>

int main(int argc, char** argv){
  ros::init(argc, argv, "laser_tf_publisher");
  ros::NodeHandle n;

  ros::Rate r(100);

  tf::TransformBroadcaster broadcaster;

  while(n.ok()){
    broadcaster.sendTransform(
      tf::StampedTransform(
        tf::Transform(tf::Quaternion(0, 0, 0), tf::Vector3(0,0,0)),
        ros::Time::now(),"base_link","laser"));
    r.sleep();
  
  }

  // required transform to (2,-1,0.5) tf::Transform(tf::Quaternion(0, 0, 3.14), tf::Vector3(0,0.75,0)),
}