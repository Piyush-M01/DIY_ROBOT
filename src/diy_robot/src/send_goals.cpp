#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <iostream>

using namespace std;

// Action specification for move_base
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv){

    // Connect to ROS
    ros::init(argc, argv, "simple_navigation_goals");

    //tell the action client that we want to spin a thread by default
    MoveBaseClient ac("move_base", true);

    // Wait for the action server to come up so that we can begin processing goals.
    while(!ac.waitForServer(ros::Duration(5.0))){
        ROS_INFO("Waiting for the move_base action server to come up");
    }   
    double x,y,w;
    char choice_to_continue = 'Y';
    bool run = true;
    
    while(run) {    
        // Ask the user where he wants the robot to go?
        cout << "\nWhere do you want to go?" << endl;
        cout << "\nEnter X coordinate" << endl;
        cin>>x;
        cout << "\nEnter Y coordinate" << endl;
        cin>>y;
        cout << "\nEnter orientation" << endl;
        cin>>w;

        // Create a new goal to send to move_base 
        move_base_msgs::MoveBaseGoal goal;

        // Send a goal to the robot
        goal.target_pose.header.frame_id = "map";
        goal.target_pose.header.stamp = ros::Time::now();
        bool valid_selection = true;

        // Use map_server to load the map of the environment on the /map topic. 
        // Launch RViz and click the Publish Point button in RViz to 
        // display the coordinates to the /clicked_point topic.

        goal.target_pose.pose.position.x = x;
        goal.target_pose.pose.position.y = y;
        goal.target_pose.pose.orientation.w = w;
        // Go back to beginning if the selection is invalid.
        if(!valid_selection) {
            continue;
        }

        ROS_INFO("Sending goal");
        ac.sendGoal(goal);

        // Wait until the robot reaches the goal
        ac.waitForResult();

        if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
            ROS_INFO("The robot has arrived at the goal location");
        else
            ROS_INFO("The robot failed to reach the goal location for some reason");

        // Ask the user if he wants to continue giving goals
        do {
            cout << "\nWould you like to go to another destination? (Y/N)" << endl;
            cin >> choice_to_continue;
            choice_to_continue = tolower(choice_to_continue); // Put your letter to its lower case
        } while (choice_to_continue != 'n' && choice_to_continue != 'y'); 

        if(choice_to_continue =='n') {
            run = false;
        }  
    }
    return 0;
}