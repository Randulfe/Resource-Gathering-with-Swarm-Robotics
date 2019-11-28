#include <ros/ros.h>
#include <iostream>
#include <stdlib.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Range.h>
#include <sensor_msgs/Illuminance.h>
#include "std_msgs/String.h"
#include <cmath>
#include <tf/transform_listener.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

float dist = 0, dist_right = 0, dist_left = 0;
float linear = 0.5;
float angular;
int front_beam = 0, right_beam = 0, left_beam = 0;
int data = 0;
int size = 0;
int randoms = 0;
int flag = 0;
float total_right=0, total_left=0;
float lux0 = 0, lux1 = 0;
float light_x[100],light_y[100];
float lightx, lighty;
int take_first = 0,obstacle = 0, flag_goal = 0;
float origin_x = 8.2, origin_y = -0.5, origin_z;
int vel_controller = 0;
int check_new = 0; 
int repetido=0;

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;



//////////////////////////////////    CALLBACKS      /////////////////////////////////////////////////////////////////////////
/////////////////////////////////   LIGHT SENSORS    ///////////////////////////////////////////////////////////////////////

void callback_l0(const sensor_msgs::Illuminance::ConstPtr& msg)
{
    ros::Rate loop_rate(100);
     lux0 = msg->illuminance;
}


void callback_l1(const sensor_msgs::Illuminance::ConstPtr& msg)
{
    ros::Rate loop_rate(100);
     lux1 = msg->illuminance;
}

//////////////////////////////   LASER   ///////////////////////////////////////////////////////////////////////////////////

void callback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
    ros::Rate loop_rate(100);
    total_right= 0;
    total_left = 0;
    size = (scan->angle_max - scan->angle_min)/scan->angle_increment;
    float ranges[size-1];
    for(data=1;data<size;data++)
{
	ranges[data] = scan->ranges[data];
}
    front_beam = (size-1)/2;
    right_beam = front_beam + 240;
    left_beam = front_beam - 240;

for(right_beam;right_beam!=front_beam;right_beam--){total_right = total_right+ranges[right_beam];}
for(left_beam;left_beam!=front_beam;left_beam++){total_left = total_left+ranges[left_beam];}

total_right = total_right/240;
total_left = total_left/240;


 float smallest=1000;  /*order array by size*/
    for ( int i=1;  i < size;  ++i ){
        if ( ranges[i] < smallest ){
             smallest = ranges[i] ;}
    }


    dist = ranges[front_beam];  /*particular distances*/
    dist_right= ranges[right_beam];
    dist_left= ranges[left_beam];



   if( smallest<2.5) //this refers to the laser scan that is just in front of the robot, the number 360
   {
   obstacle = 0;
    if((total_left*10)<((total_right*10)) )
	{angular = -1/total_left *4;
	}
     else
	{angular = 1/total_right *4;	
	}   
  }

  else { obstacle=1; }//velocity is inversely proportional to the distance to the wall in front
   
}


/////////////////// MOVE TO POSE FUNCTION /////////////////////////////////////////////////////

void goToPose(float x, float y, float z)
{
  vel_controller = 1;
  MoveBaseClient ac("mybot1/move_base", true);   //tell the action client that we want to spin a thread by default
  printf("STARTING MOVEMENT\n");

  while(!ac.waitForServer(ros::Duration(5.0))){     //wait for the action server to come up
    ROS_INFO("Waiting for the move_base action server to come up");
      }

  move_base_msgs::MoveBaseGoal goal;


  goal.target_pose.header.frame_id = "map";    //we'll send a goal to the robot to move 1 meter forward
  goal.target_pose.header.stamp = ros::Time::now();

  goal.target_pose.pose.position.x = x ;
  goal.target_pose.pose.position.y = y ;
  goal.target_pose.pose.orientation.z = 1.57 ;
  goal.target_pose.pose.orientation.w = 1 ;
  //goal.target_pose.pose.orientation = odom_quat ;

  ROS_INFO("Sending goal to %f, %f, %f \n",goal.target_pose.pose.position.x,goal.target_pose.pose.position.y,z);
  ac.sendGoal(goal);


  ac.waitForResult();

  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
    ROS_INFO("Task completed"); vel_controller = 0; }
  else{
    ROS_INFO("Task failed"); vel_controller = 0; }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char **argv)
{
    ros::init(argc, argv, "main");
	
     ros::NodeHandle n; 
    
    ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/mybot1/cmd__vel", 100);
    ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
    ros::Subscriber sub = n.subscribe<sensor_msgs::LaserScan>("/mybot1/laser",1000,callback); 
    ros::Subscriber sub_light0 = n.subscribe<sensor_msgs::Illuminance>("mybot1/lightSensor",1000,callback_l0); 
    ros::Subscriber sub_light1= n.subscribe<sensor_msgs::Illuminance>("mybot1/lightSensor_second",1000,callback_l1);
    ros::Duration(3).sleep(); 
    tf::TransformListener listener;

       
    ros::Rate loop_rate(100);
    
    geometry_msgs::Twist cmd_msg;
    std_msgs::String str_msg;
    int count=0;
    light_x[0]=1;

    while (ros::ok())
    {

    tf::StampedTransform transform;
    std::stringstream ss;
    ss << "first point" << light_x[0] ;//<< count;
    str_msg.data = ss.str();
    float luxavg = (lux0+lux1)/2;
    //printf("Average Light: %.2f\n",luxavg);
/////////////////////////////////// SAVE POSITION WHEN LIGHT FOUND ////////////////////////////////////////////////////////////// 
    
    if(take_first==0 || lux0>150 || lux1>150){
      
        try{
          ros::Time now = ros::Time(0);
          listener.waitForTransform("map","mybot1/chassis",now,ros::Duration(3.0));
          listener.lookupTransform("map", "mybot1/chassis",  
                               now, transform);}


       catch (tf::TransformException ex){
          ROS_ERROR("%s",ex.what());
          ros::Duration(1.0).sleep();}

        if(take_first==0){
        	take_first=1;
            origin_x = transform.getOrigin().getX();
          	origin_y = transform.getOrigin().getY();
          	origin_z = transform.getOrigin().getZ();
          	printf(" oringin, x: %f, y:%f, z:%f \n",origin_x, origin_y,origin_z);}


        else{
	   
	    lightx = transform.getOrigin().getX();
         lighty = transform.getOrigin().getY();
	    printf("something found at lightx: %f, lighty: %f\n", lightx,lighty);
            for(int i=0;i<20;i++){
	        	if(light_x[i]==1){ 
	        		for(int k=0; k<20; k++){
	        			if((lightx<(light_x[k]+0.5) && lightx>(light_x[k]-0.5)) || (lighty<(light_y[k]+0.5) && lighty>(light_y[k]-0.5))){
	        					repetido=1;
	        			}
	        			printf("REP: %d\n",repetido);
	        		}
	        		if (repetido==0){	            	
	        			printf("NEW LIGHT SPOT\n");
	            		light_x[i] = lightx;
	            		light_y[i] = lighty;
	            		check_new = 1;
	            		printf("Light number %d , at x: %f, y: %f\n",i,light_x[i],light_y[i]);
	            	}
				}	            

        	}
        	repetido=0;
            for(int i=0;i<20;i++){
            	if((light_x[i]!=0 && light_x[i]!=1) && (light_x[i+1]==0))
            	{
            		light_x[i+1]=1;
            	}
            }
    	}
    }
    

	if(check_new==1){
            goToPose(origin_x,origin_y,origin_z);
	    	check_new = 0;}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        else{  
	    	if(obstacle==1){
	        	if(lux0<lux1){angular=0.5;}else{angular=-0.5;} 	
	    	}  	
        	cmd_msg.angular.z = angular*2;
            cmd_msg.linear.x = linear*2;
            if(vel_controller==0){
	        	pub.publish(cmd_msg); }
         
            }
            
            chatter_pub.publish(str_msg);

 	//printf("x1: %f,y1: %f",light_x[0],light_y[0]);
        ros::spinOnce();
        loop_rate.sleep();
    }
    
    return 0;
    //1over10 is sending
}


