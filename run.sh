#!/bin/bash
# This file is automatically generated by se306_p1_package/world/updateactors.py
export ROS_PACKAGE_PATH=`pwd`:$ROS_PACKAGE_PATH

roscore & ROSCORE_PID=$!
rosrun se306_p1_package ActorSpawner 0 R0 & ROBOT0_PID=$!
rosrun se306_p1_package ActorSpawner 1 R1 & ROBOT1_PID=$!
rosrun se306_p1_package ActorSpawner 2 Resident & ROBOT2_PID=$!

rosrun stage_ros stageros se306_p1_package/world/myworld.world

kill $ROBOT0_PID
kill $ROBOT1_PID
kill $ROBOT2_PID
kill $ROSCORE_PID