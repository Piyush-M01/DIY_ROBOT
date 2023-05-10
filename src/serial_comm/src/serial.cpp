// C library headers
#include "serial_init.cpp"

int main(int argc, char **argv) {
  ros::init(argc, argv, "serial_node");
  // ros::Rate loop_rate(10);
  Serial ob; //class object which calls the constructor to initialize all the values
  while(ros::ok())
  {
    ros::spinOnce();
    // loop_rate.sleep();
    ob.pub_left_right_ticks();
  }
  return 0; // success
};