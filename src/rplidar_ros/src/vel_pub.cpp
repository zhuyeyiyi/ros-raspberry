#include<ros/ros.h>
#include<std_msgs/Float32.h>

int main(int argc, char** argv)
{
	// node name is "velnode"
	ros::init(argc, argv, "velnode");
	ros::NodeHandle n;
	//
	ros::Publisher vpublisher_l = n.advertise<std_msgs::Float32>("lwheel_vel",100); 
	ros::Publisher vpublisher_r = n.advertise<std_msgs::Float32>("rwheel_vel",100);
	ros::Rate loop_rate(10);
	while(ros::ok())
	{
		std_msgs::Float32 vel_l;
		std_msgs::Float32 vel_r;
		
		vel_l.data = 0.05;
		vel_r.data = 0.05;
		
		vpublisher_l.publish(vel_l);
		vpublisher_r.publish(vel_r);
		
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
}
