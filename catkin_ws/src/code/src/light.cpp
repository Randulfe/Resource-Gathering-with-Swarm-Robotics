#include <ros/ros.h>
#include <iostream>
#include <stdlib.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Range.h>
#include <sensor_msgs/Illuminance.h>
#include<cmath>

float dist = 0, dist_right = 0, dist_left = 0;
float linear = -0.5;
float angular;
int front_beam = 0, right_beam = 0, left_beam = 0;
int data = 0;
int size = 0;
int randoms = 0;
int flag = 0;
float total_right=0, total_left=0;
float lux0 = 0, lux1 = 0, average=0;
//ros::Duration(5).sleep(); // sleep for half a second

void callback_l0(const sensor_msgs::Illuminance::ConstPtr& msg)
{
    ros::Rate loop_rate(10);
     lux0 = msg->illuminance;
}


void callback_l1(const sensor_msgs::Illuminance::ConstPtr& msg)
{
    ros::Rate loop_rate(10);
     lux1 = msg->illuminance;
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "main");

    //ros::NodeHandle n;
     ros::NodeHandle n; 
    
    ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/mybot1/cmd_vel", 100);
    ros::Subscriber sub_light0 = n.subscribe<sensor_msgs::Illuminance>("mybot1/lightSensor",1000,callback_l0);
    ros::Subscriber sub_light1= n.subscribe<sensor_msgs::Illuminance>("mybot1/lightSensor_second",1000,callback_l1);        
    srand(time(0));
    ros::Rate loop_rate(10);
    
    geometry_msgs::Twist cmd_msg;
    
    
    while (ros::ok())
    {

        cmd_msg.linear.x = 0;  
	//if(lux0<lux1){angular=0.5;}else{angular=-0.5;} 
        average = (lux0+lux1)/2;	
        printf("lux=%f", average);
        cmd_msg.angular.z = 0;
	pub.publish(cmd_msg);
 
        ros::spinOnce();
        loop_rate.sleep();
    }
    
    
    return 0;
}


	//randoms = int(rand()%10+1);
        //printf("|  %d |",randoms);

	//if(randoms%2==0 && flag==1){angular = angular*-1;}
