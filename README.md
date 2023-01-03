# DIY_ROBOT

roslaunch diy_robot diy_robot.launch   
rosrun teleop_twist_keyboard teleop_twist_keyboard.py   
   
For sending values using RPI4:   
python3 src/diy_robot/src/cmd_vel.py   
   
For mapping using rplidar:   
roslaunch diy_robot mapping.launch   
   
For auto-nav:   
roslaunch diy_robot localisation.launch
