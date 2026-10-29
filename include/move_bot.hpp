#pragma once

/**
 * MIT License
 *
 * Copyright (c) 2021 Maaruf Vazifdar
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to 
 * deal in the Software without restriction, including without limitation the  
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 * DEALINGS IN THE SOFTWARE. 
 */

#include <geometry_msgs/Twist.h>
#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>

class MoveBot {
 public:
  ros::NodeHandle n;
  ros::Publisher pub;
  ros::Subscriber sub;
  geometry_msgs::Twist vel;

  MoveBot() {
    // Publish velocity commands to /cmd_vel
    pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 10);
    // Subscribe to LiDAR data from /scan
    sub = n.subscribe("scan", 10, &MoveBot::LaserscanCallback, this);
  }

  void turnLeft();
  void turnRight();
  void moveForward();
  void LaserscanCallback(const sensor_msgs::LaserScan::ConstPtr& msg);
};
