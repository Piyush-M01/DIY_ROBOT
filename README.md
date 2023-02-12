# DIY_ROBOT
## Run Serial Communication to send velocity and receive left and right ticks   
```
roscore
```
```
rosrun serial_comm velocity_serial
```
```
rosrun teleop_twist_keyboard teleop_twist_keyboard.py
```

## Run odometry generator for robot   
```
roslaunch localization_data_pub rviz_click_to_2d.launch
```
After executing the above code, rviz window will open up. Now estimate the initial pose using rviz tools.

## To check if the data is getting published to odometry
```
rostopic echo /odom*
```
