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

#include <ros/ros.h>
#include <move_bot.hpp>

// Turn counter-clockwise (left)
void MoveBot::turnLeft() {
  vel.linear.x = 0.0;
  vel.angular.z = 0.3;
  pub.publish(vel);
  ROS_INFO_STREAM("Turning Left");
}

// Turn clockwise (right)
void MoveBot::turnRight() {
  vel.linear.x = 0.0;
  vel.angular.z = -0.3;
  pub.publish(vel);
  ROS_INFO_STREAM("Turning Right");
}

// Move straight forward
void MoveBot::moveForward() {
  vel.linear.x = 0.2;
  vel.angular.z = 0.0;
  pub.publish(vel);
  ROS_INFO_STREAM("Moving Forward");
}

// Triggered every time LiDAR publishes a new laser scan
void MoveBot::LaserscanCallback(const sensor_msgs::LaserScan::ConstPtr& msg) {
  // If an obstacle is detected in front or front-right within 0.3m, turn left
  if (msg->ranges[330] < 0.3 || msg->ranges[0] < 0.3) {
    turnLeft();
  } else if (msg->ranges[30] < 0.3) {
    // If an obstacle is detected on the front-left within 0.3m, turn right
    turnRight();
  } else {
    // Path is clear, drive straight
    moveForward();
  }
}

int main(int argc, char **argv) {
  // Initialize the ROS node
  ros::init(argc, argv, "walker");

  // Create an instance of the robot controller
  MoveBot M1;

  // Keep processing incoming laser scans until Ctrl+C
  ros::spin();
  return 0;
}
