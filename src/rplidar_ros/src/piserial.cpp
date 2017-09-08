/*
#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
*/

#include <string>
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <math.h>
#include "serial/serial.h"

using std::string;
using std::exception;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;

unsigned char data_terminal0 = 0x0d; // "/r" 
unsigned char data_terminal1 = 0x0a; // "/n"
unsigned char datatx[10] = {0};

int main(int argc, char **argv)
{
	string port("/dev/rmotor");
	unsigned long baud = 115200;
	serial::Timeout to = serial::Timeout::simpleTimeout(1000);
	serial::Serial pi_serial(port, baud, to);
	
	return 0;
}



