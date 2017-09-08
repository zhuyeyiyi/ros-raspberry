#include <stdio.h> /* Standard input/output definitions */
#include <string.h> /* String function definitions */
#include <unistd.h> /* UNIX standard function definitions */
#include <fcntl.h> /* File control definitions */
#include <errno.h> /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <unistd.h>

#include "packet.h"
#include "imu_data_decode.h"

#include <math.h>

#include <ros/ros.h>
#include <ros/console.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/MagneticField.h>
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/PoseStamped.h>
#include <tf/tf.h>



/*
 * @brief Open serial port with the given device name
 *
 * @return The file descriptor on success or -1 on error.
 */
int open_port(char *port_device)
{
    struct termios options;

    int fd = open(port_device, O_RDWR | O_NOCTTY | O_NONBLOCK);

	tcgetattr(fd, &options);

	if (fd == -1)
    {
        perror("open_port: Unable to open SerialPort");
		return(-1);
    }

    if(fcntl(fd, F_SETFL, 0)<0)
	{
		printf("fcntl failed\n");
	}
    else
	{
		fcntl(fd, F_SETFL, 0);
	}
  
	if(isatty(STDIN_FILENO)==0)
	{
		printf("standard input is not a terminal device\n");
	}
	else 
	{
		printf("isatty success!\n");
	}

	bzero(&options,sizeof(options));

	options.c_cflag = B115200 | CS8 | CLOCAL |CREAD;
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	options.c_cc[VTIME] = 0;
    options.c_cc[VMIN] = 0;
	tcflush(fd,TCIFLUSH);
    tcsetattr(fd, TCSANOW, &options);
	return (fd);
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "imu_publisher");

    int fd=open_port("/dev/imu");

    ros::NodeHandle n;
    ros::Publisher imu_pub = n.advertise<sensor_msgs::Imu>("imu", 100);

	
    //tcgetattr(fd, &options);


    //Enable the receiver and set local mode...
   /* options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~CSIZE; 
    options.c_cflag |= CS8;

    //No parity
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
	options.c_cc[VTIME] = 0;
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	options.c_oflag &= ~OPOST; */

	
	
	imu_data_decode_init();
    
   
	uint8_t ID = 0;
	int16_t Acc[3] = {0};
	int16_t Gyo[3] = {0};
    int16_t Mag[3] = {0};
	float Eular[3] = {0};
	float Quat[4]  = {0};
    int32_t Pressure = 0;

	int i;
	uint8_t buf[1024];


    ros::Time current_time, last_time;
    current_time = ros::Time::now();
    last_time = ros::Time::now();


    ros::Rate r(30);
    while(n.ok())
	{
        ros::spinOnce();
        current_time = ros::Time::now();

        double dtime = (current_time - last_time).toSec();
        double hz=1/dtime;
        ssize_t n = read(fd, buf, sizeof(buf));
	
        for(i=0; i<n; i++)
           {
              Packet_Decode(buf[i]);
           }
			
			get_raw_acc(Acc);
			get_raw_gyo(Gyo);
			get_raw_mag(Mag);
			get_eular(Eular);
            get_quat(Quat);
			get_id(&ID);

     		printf("Time:%f, Hz=%f \r\n",dtime,hz);

			printf("Acc:%d %d %d\r\n",Acc[0], Acc[1], Acc[2]);
			printf("Gyo:%d %d %d\r\n",Gyo[0], Gyo[1], Gyo[2]);
			printf("Mag:%d %d %d\r\n",Mag[0], Mag[1], Mag[2]);
			printf("Eular(P R Y):%0.2f %0.2f %0.2f\r\n",Eular[0], Eular[1], Eular[2]);
			printf("quat(W X Y Z):%0.3f %0.3f %0.3f %0.3f\r\n",Quat[0], Quat[1], Quat[2], Quat[3]);

             //set the header
             sensor_msgs::Imu imu;
             imu.header.stamp = current_time;
	   
             //set the acceleration
             imu.linear_acceleration.x = double(Acc[0] * 0.0098);
             imu.linear_acceleration.y = double(Acc[1] * 0.0098);
             imu.linear_acceleration.z = double(Acc[2] * 0.0098);
             imu.linear_acceleration_covariance = {{0,-1,0},{0,0,0},{0,0,0}};
 
             //set the angular_velocity
             imu.angular_velocity.x = double(Gyo[0] * 0.31415965 / 180.0);
             imu.angular_velocity.y = double(Gyo[1] * 0.31415965 / 180.0);
             imu.angular_velocity.z = double(Gyo[2] * 0.31415965 / 180.0);
             imu.angular_velocity_covariance = {{0,-1,0},{0,0,0},{0,0,0}};

             //set the quaternion
             imu.orientation.x = double(Quat[1]);
             imu.orientation.y = double(Quat[2]);
             imu.orientation.z = double(Quat[3]);
             imu.orientation.w = double(Quat[0]);
             imu.orientation_covariance = {{0,-1,0},{0,0,0},{0,0,0}};


             imu_pub.publish(imu);

             last_time = current_time;
             r.sleep();
			
	}


    close(fd);
}

