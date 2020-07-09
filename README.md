# Dissertation Project : Developing a Swarm Robotics System: resource gathering using cooperation and competition in robotics
I developed this project based on my previous work and knoledge with the turtlebot and ROS (Robotic Operating System) and with my passion for AI and robotics. 

The main objective of this project was to analyse the efficency in gathering resources using Swarm robots (multiple mini robots that do not have much intelligence separately but that working together, just like ants, a bigger group intelligence arises). 
This is just a very brief explanation of the design of the problem but you can find a detailed report of my research in this repo. 

The idea was to evaluate the performance of an individual and them of a group of two that compete against other similar team.
The project had two main parts, a computer simulation experimentation and a mechanical/electronic implementation. I did way more progress on the algorithmic and computational side of the project as I started building the actual swarm robots at the end of the course, so I could not manage to finished them fully as I wished. However, I earnt huge experience in combining CS, Electronics and Mechanics. 

These were the main specifications of the problem: 
* Two teams of two robots would be deployed in a maze in their respective bases.
* Their objective would be to map and explore a completely unknown maze looking for resources.
* These resources were spots of light that would be placed at random position with high probability of respawning in their first position.
* Once a robot finds a resource, the light spot would be off (the resource has been collected) and the robot must find the easiest path to his base.
* Robots must communicate effectively to be both exploring and visiting previous resource spots in the most optimised way without hitting the walls of the maze. 
---
In terms of the actual robots, these were their specifications:
* The robots used had a 180º sonar sensor, two powered front wheels and one rear small wheel. 
* They had a Atmel microcontroller connected to a wifi module to use communications. 

I created the robotic models with `ROS` and used `Gazebo` as the simulation environment. For mapping the sensorial and path planning information I used `Rviz`. 

You can find more about this project and specially the technologies and AI methods I used and why in the [final report](https://github.com/Randulfe/Resource-Gathering-with-Swarm-Robotics/blob/master/FinalReport.pdf). 
