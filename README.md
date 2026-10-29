# TurtleBot3 Obstacle Avoidance Simulation (`ros_walker_bot`)

A ROS Melodic simulation of a TurtleBot3 Burger performing autonomous reactive obstacle avoidance in Gazebo using 360-degree planar LiDAR data.

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![ROS Version](https://img.shields.io/badge/ROS-Melodic-orange.svg)](http://wiki.ros.org/melodic)
[![Language](https://img.shields.io/badge/Language-C++11-brightgreen.svg)]()

---

## Authors & Attribution
* **Maintainer & Developer:** Parth Kharat ([kharatparth362006@gmail.com](mailto:kharatparth362006@gmail.com))
* **Original Reference:** Mohammed Maaruf Vazifdar

---

## Project Overview

This project implements a reactive obstacle avoidance algorithm (often called a "walker" node) for the TurtleBot3 Burger platform. 

The robot drives forward through a closed, cluttered world until an obstacle is detected within a designated safety threshold ($0.3\text{ m}$). Upon detection, the robot executes an evasive turn away from the obstacle until a clear path is found, resuming forward motion.

### Architecture & Data Flow

```text
  +------------------+         /scan (sensor_msgs/LaserScan)         +------------------+
  |  Gazebo / LiDAR  | --------------------------------------------> |   walker node    |
  | (LDS-01 Scanner) |                                               |    (move_bot)    |
  +------------------+                                               +------------------+
                                                                              |
                                                                              | /cmd_vel (geometry_msgs/Twist)
                                                                              v
                                                                     +------------------+
                                                                     |  TurtleBot3 Base |
                                                                     |  (Wheels/Motor)  |
                                                                     +------------------+
```

* **Subscriber:** Subscribes to the `/scan` topic to receive 360-degree laser range measurements (`sensor_msgs/LaserScan`).
* **Publisher:** Publishes velocity commands to the `/cmd_vel` topic (`geometry_msgs/Twist`).
* **Visualization:** RViz config displaying robot transform tree (TF), robot model, and real-time laser point cloud.
* **Data Logging:** Integrated ROS bag support to record and replay simulation runs.

---

## Algorithm Logic

The walker node evaluates laser rays at key directions relative to the robot's heading:

| Laser Ray Index | Relative Angle | Region | Action if Range $< 0.3\text{ m}$ |
| :--- | :--- | :--- | :--- |
| `0` | $0^\circ$ (Direct Front) | Front | Turn Left ($\omega_z = +0.3\text{ rad/s}$) |
| `330` | $330^\circ$ ($-30^\circ$) | Front-Right | Turn Left ($\omega_z = +0.3\text{ rad/s}$) |
| `30` | $30^\circ$ ($+30^\circ$) | Front-Left | Turn Right ($\omega_z = -0.3\text{ rad/s}$) |
| *All above $\ge 0.3\text{ m}$* | — | Clear | Move Forward ($v_x = 0.2\text{ m/s}$) |

---

## Prerequisites & Dependencies

* **OS:** Ubuntu 18.04 LTS (or Ubuntu Docker container)
* **ROS:** ROS Melodic Desktop-Full
* **Simulator:** Gazebo 9+
* **TurtleBot3 Packages:**
  ```bash
  sudo apt-get update
  sudo apt-get install ros-melodic-turtlebot3-simulations ros-melodic-turtlebot3-msgs
  ```

---

## Workspace Setup & Build

1. Set the TurtleBot3 model environment variable:
   ```bash
   echo "export TURTLEBOT3_MODEL=burger" >> ~/.bashrc
   source ~/.bashrc
   ```

2. Clone this repository into your Catkin workspace `src/` directory:
   ```bash
   cd ~/<your_catkin_ws>/src
   git clone https://github.com/parthkharat20/R-A-SIM.git ros_walker_bot
   ```

3. Build the package:
   ```bash
   cd ~/<your_catkin_ws>
   catkin_make
   source devel/setup.bash
   ```

---

## Running the Simulation

### 1. Launch Simulation with Obstacle Avoidance
To launch Gazebo world, spawn TurtleBot3, launch the `walker` node, and visualize in RViz:
```bash
roslaunch ros_walker_bot ros_walker_bot.launch
```

### 2. Launch with ROS Bag Recording
To record simulation topics to a bag file during runtime:
```bash
roslaunch ros_walker_bot ros_walker_bot.launch record_rosbag:=true
```
The bag file will be saved under the `results/` folder as `ros_walker_bot_bagfile.bag`.

### 3. Inspect and Replay Bag File
To inspect the contents of the recorded bag file:
```bash
rosbag info src/ros_walker_bot/results/ros_walker_bot_bagfile.bag
```

To replay the recorded bag data and visualize the robot state in RViz:
```bash
# Terminal 1: Launch master
roscore

# Terminal 2: Launch RViz configured for bag playback
cd ~/<your_catkin_ws>
source devel/setup.bash
rosrun rviz rviz -d src/ros_walker_bot/rviz/ros_walker_bot_rosbag.rviz

# Terminal 3: Play bag file
cd ~/<your_catkin_ws>
source devel/setup.bash
rosbag play src/ros_walker_bot/results/ros_walker_bot_bagfile.bag
```

---

## Code Quality & Static Analysis

Static analysis was performed using **cppcheck** and **cpplint** to ensure memory safety and adherence to Google C++ coding standards:

```bash
# Run cpplint
cpplint $(find . -name "*.hpp" -or -name "*.cpp") > results/cpplint_result.txt

# Run cppcheck
cppcheck --language=c++ --std=c++11 -I include --suppress=missingIncludeSystem $(find . -name "*.hpp" -or -name "*.cpp") > results/cppcheck_result.txt
```

---

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.
