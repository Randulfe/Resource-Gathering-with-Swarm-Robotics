#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <iostream>
#include <stdlib.h>

std::string param;

int main(int argc, char** argv){
  ros::init(argc, argv, "map_broadcaster");
  ros::NodeHandle node;
  
  node.getParam("map_broadcaster/param",param);

  tf::TransformBroadcaster br;
  tf::Transform transform;

  ros::Rate rate(10.0);
  while (node.ok()){
    transform.setOrigin( tf::Vector3(0.0, 0.0, 0.0) );
    transform.setRotation( tf::Quaternion(0, 0, 0, 1) );
    br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "map", param));
    rate.sleep();
  }
  return 0;
};

