#include <ros/ros.h>
#include <string.h>
#include <tf/transform_listener.h>
#include <pcl_ros/transforms.h>
#include <laser_geometry/laser_geometry.h>
#include "sensor_msgs/LaserScan.h"
#include <bits/stdc++.h>


class Laser_Merger
{
    private:
        int counter;
        ros::Publisher pub;
        ros::Subscriber laser_1,laser_2;
        ros::ServiceServer reset_service;
        std::vector<double> v;
    public:
        Laser_Merger(ros::NodeHandle *nh);
        void callback_laser_1(const sensor_msgs::LaserScan::ConstPtr& msg);
        void callback_laser_2(const sensor_msgs::LaserScan::ConstPtr& msg);

};

Laser_Merger::Laser_Merger(ros::NodeHandle *nh) {

    pub = nh->advertise<sensor_msgs::LaserScan>("/scan_multi", 10);    
    laser_1 = nh->subscribe("/my_robot/laser/scan", 100, &Laser_Merger::callback_laser_1, this);
    laser_2 = nh->subscribe("/img_scan", 100, &Laser_Merger::callback_laser_2, this);        
}

void Laser_Merger::callback_laser_1(const sensor_msgs::LaserScan::ConstPtr& msg)
{
    for(auto i:msg->ranges)
    v.push_back(i);
}

void Laser_Merger::callback_laser_2(const sensor_msgs::LaserScan::ConstPtr& msg)
{
    if(v.size()==360)
    {
        int size=360;
        for(auto i:msg->ranges)
        {
            i=min(v[size-1],msg->ranges[size-1]);
            size--;
        }
    }
}

int main (int argc, char **argv)
{
    ros::init(argc, argv, "laser_merger");
    ros::NodeHandle nh;
    Laser_Merger ob(&nh);
    ros::spin();
}