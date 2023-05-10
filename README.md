# DIY_ROBOT

## Run odometry generator for robot and simulation of the bot and environment  
```
roslaunch diy_robot diy_robot.launch 
```

## Run Serial Communication to send velocity and receive left and right wheel velocities   
```
rosrun serial_comm velocity_serial
```
## To check if the data is getting published to odometry
```
rostopic echo /odom
```
## To map a new environment
```
roslaunch diy_robot mapping.launch
```
## lane detection
```
rosrun lane_mapping realsense.py 
```
```
rosrun lane_mapping img_to_laser.py
```
## Configure the scaling factor of lane point clouds
```
rosrun rqt_reconfigure rqt_reconfigure
```
## Run AutoNav
```
roslaunch diy_robot localisation.launch
```
## Provide WayPoints using Publish Point button in Rviz
# All the waypoints will be followed in a sequential order
```
rosrun diy_robot waypoints.py
```
