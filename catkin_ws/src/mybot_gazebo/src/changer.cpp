#include <ros/ros.h>
#include <iostream>
#include <stdlib.h>
#include <geometry_msgs/Twist.h>
#include "std_msgs/String.h"
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

float linearx,lineary,linearz,angularx,angulary,angularz;


void callback(const geometry_msgs::Twist::ConstPtr& msg)
{
    ros::Rate loop_rate(100);
     linearx = msg->linear.x;
     lineary = msg->linear.y;
     linearz = msg->linear.z;
     angularx = msg->angular.x;
     angulary = msg->angular.y;
     angularz = msg->angular.z;
}


int main(int argc, char** argv){

     ros::init(argc, argv, "changer");
     ros::NodeHandle n; 

    ros::Subscriber sub = n.subscribe<geometry_msgs::Twist>("/cmd_vel",100,callback); 
    ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/mybot1/cmd_vel", 100);

    tf::TransformListener listener;

    ros::Rate rate(100.0);
    
    geometry_msgs::Twist cmd_msg;


  while (n.ok()){
        cmd_msg.linear.x = linearx;
        cmd_msg.linear.y = lineary;
        cmd_msg.linear.z = linearz;
        cmd_msg.angular.x = -angularx;
        cmd_msg.angular.y = -angulary;
        cmd_msg.angular.z = -angularz;

	pub.publish(cmd_msg);
 
        ros::spinOnce();
        rate.sleep();
  }
  return 0;
};
