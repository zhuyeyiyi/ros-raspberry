#include <stdio.h>
#include <stdlib.h>
#include <cmath>


#include <ros/ros.h>
#include <serial/serial.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <string.h> 
#include <unistd.h> // include sleep(s) and usleep(us) function

#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Float32.h>


#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/Int8MultiArray.h>


#define SERIAL_TIMEOUT 2 // ms
#define ARR_LEN(array,length){length = sizeof(array)/sizeof(array[0]);}

#define pi 3.141592627

// car parameters: 
#define WHEEL_RADIUS 0.039 
#define WHEEL_DISTANCE 0.423

// motor: reduction ratio
#define REDUCTIONi  5.8

// encoder: 
#define ENCODERNUM 4000.0 
#define CIRCLENUM 23200.0
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::exception;

serial::Serial ros_lser;    //left motor serial
serial::Serial ros_rser;    //right motor serial

string lserialstr("/dev/lmotor");
string rserialstr("/dev/rmotor");


// reset command
unsigned char reset[] = {0x23, 0x00, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55};

// speed mode
unsigned char speedmode[] ={0x23, 0x01, 0x03, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55};

// speed parameter setting
unsigned char speedpara[] = {0x23, 0x04, 0x00, 0x00, 0x00, 0x00, 0x55, 0x55, 0x55, 0x55};

// feed back parameter setting
unsigned char feedbackpara[] = {0x23, 0x0A, 0x00, 0x00, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55};

// send data buffer
unsigned char sdata[20];

// receive data buffer
string recbuffer; 

int arrlen = 0;

double cmd_velocity; // line velocity
double cmd_anglevel; // angle velocity

int cmd_vel_lwheel;
int cmd_vel_rwheel;

//call back function
void callback(const std_msgs::String::ConstPtr& msg){
     ROS_INFO_STREAM("Write to serial port" << msg->data);
     ros_lser.write(msg->data);
 }

void init_rserial(string rserialport){ 
    ros_rser.setPort(rserialport);
    ros_rser.setBaudrate(115200);
    serial::Timeout to = serial::Timeout::simpleTimeout(SERIAL_TIMEOUT);
    ros_rser.setTimeout(to);
    ros_rser.open();
}

void init_lserial(string lserialport){
    ros_lser.setPort(lserialport);
    ros_lser.setBaudrate(115200);
    serial::Timeout to = serial::Timeout::simpleTimeout(SERIAL_TIMEOUT);
    ros_lser.setTimeout(to);
    ros_lser.open();
}

void lserial_setting(){

     //pwm limitting setting, max = 5000
     unsigned int pwmlimit = 4800;
     speedpara[2] = (unsigned char)((pwmlimit>>8)&0xFF);
     speedpara[3] = (unsigned char)(pwmlimit&0xFF);
     
     //velocity parameter setting, rpm    
     int velocity_rpm = 0;
     speedpara[4] = (unsigned char)((velocity_rpm)>>8&0xFF);
     speedpara[5] = (unsigned char)(velocity_rpm&0xFF);

     //feedback parameter setting, period(20 ms), CTL(disable)
     feedbackpara[2] = (unsigned char)(0); 
     feedbackpara[3] = (unsigned char)(0x00); 


     ARR_LEN(reset,arrlen);
     ros_lser.write(reset,arrlen);
     usleep(200000); // sleep 200 ms

     ARR_LEN(speedmode,arrlen);
     ros_lser.write(speedmode,arrlen);
     usleep(200000); // sleep 200 ms

     ARR_LEN(speedpara,arrlen);
     ros_lser.write(speedpara,arrlen);
     usleep(200000); // sleep 200 ms

     ARR_LEN(feedbackpara,arrlen);
     ros_lser.write(feedbackpara,arrlen);
     usleep(200000); // sleep 200 ms
}


void rserial_setting(){

     //pwm limitting setting, max = 5000
     unsigned int pwmlimit = 4800;
     speedpara[2] = (unsigned char)((pwmlimit>>8)&0xFF);
     speedpara[3] = (unsigned char)(pwmlimit&0xFF);
     
     //velocity parameter setting, rpm    
     int velocity_rpm = 0;
     speedpara[4] = (unsigned char)((velocity_rpm)>>8&0xFF);
     speedpara[5] = (unsigned char)(velocity_rpm&0xFF);

     //feedback parameter setting, period(0 ms), CTL(disable)
     feedbackpara[2] = (unsigned char)(0); 
     feedbackpara[3] = (unsigned char)(0x00); 


     ARR_LEN(reset,arrlen);
     ros_rser.write(reset,arrlen);
     usleep(200000); // sleep 200 ms

     ARR_LEN(speedmode,arrlen);
     ros_rser.write(speedmode,arrlen);
     usleep(200000); // sleep 200 ms

     ARR_LEN(speedpara,arrlen);
     ros_rser.write(speedpara,arrlen);
     usleep(200000); // sleep 200 ms

     ARR_LEN(feedbackpara,arrlen);
     ros_rser.write(feedbackpara,arrlen);
     usleep(200000); // sleep 200 ms
}


void lmotor_setting(int velocity_rpm){
           
    //velocity parameter setting, rpm    
    speedpara[4] = (unsigned char)((velocity_rpm)>>8&0xFF);
    speedpara[5] = (unsigned char)(velocity_rpm&0xFF);

    ARR_LEN(speedpara,arrlen);
    ros_lser.write(speedpara,arrlen);
    usleep(2000); // sleep 200 ms
}

void rmotor_setting(int velocity_rpm){
    
    //velocity parameter setting, rpm    
    speedpara[4] = (unsigned char)((velocity_rpm)>>8&0xFF);
    speedpara[5] = (unsigned char)(velocity_rpm&0xFF);

    ARR_LEN(speedpara,arrlen);
    ros_rser.write(speedpara,arrlen);
    usleep(2000); // sleep 200 ms
}


void enable_serial_feedback(){
    
     //feedback parameter setting, period(20 ms), CTL(disable)
     feedbackpara[2] = (unsigned char)(2); 
     feedbackpara[3] = (unsigned char)(0x00);

     ARR_LEN(feedbackpara,arrlen);
     
     ros_lser.write(feedbackpara,arrlen);
     ros_rser.write(feedbackpara,arrlen);

/*
     if(!ros_lser.isOpen()){
         init_lserial(lserialstr);
     }

     if(!ros_rser.isOpen()){
         init_rserial(rserialstr);
     }
  */  
}

void disable_serial_feedback(){
    
     //feedback parameter setting, period(20 ms), CTL(disable)
     feedbackpara[2] = (unsigned char)(0); 
     feedbackpara[3] = (unsigned char)(0x00);

     ARR_LEN(feedbackpara,arrlen);
     
     ros_lser.write(feedbackpara,arrlen);
     ros_rser.write(feedbackpara,arrlen);

/*
     if(ros_lser.isOpen()){
        ros_lser.close();
     }

     if(ros_rser.isOpen()){
        ros_rser.close();
     }
*/
}

void clear_serial_buffer(){
     ros_lser.flush();
     ros_rser.flush();
}



//cmd_vel callback function
void velcmd_callback(const geometry_msgs::Twist::ConstPtr & vel_msg){
    
    cmd_velocity = vel_msg->linear.x;    
    cmd_anglevel = vel_msg->angular.z;    
    
    double rwheel_vel = 0;
    double lwheel_vel = 0;
    
    if( cmd_velocity == 0){         // turning 
        rwheel_vel = cmd_anglevel*WHEEL_DISTANCE/2.0;
        lwheel_vel = (-1)*rwheel_vel;
    }
    else if( cmd_anglevel == 0 ){   // forward or backward
        rwheel_vel = cmd_velocity;
        lwheel_vel = cmd_velocity;
    }
    else{                           // do arcs moving
        rwheel_vel = cmd_velocity + cmd_anglevel*WHEEL_DISTANCE/2.0 ; 
        lwheel_vel = cmd_velocity - cmd_anglevel*WHEEL_DISTANCE/2.0 ; 
    }

    int lmotor_vel = (int)(lwheel_vel*60*REDUCTIONi/(2*pi*WHEEL_RADIUS));
    int rmotor_vel = (int)(rwheel_vel*60*REDUCTIONi/(2*pi*WHEEL_RADIUS));
   

    cmd_vel_lwheel = lmotor_vel;
    cmd_vel_rwheel = rmotor_vel;

    lmotor_setting(lmotor_vel);
    rmotor_setting(rmotor_vel);


    cout<<"ml: "<<lmotor_vel<<endl;
    cout<<"mr: "<<rmotor_vel<<endl;
}



int main (int argc, char** argv){
     ros::init(argc, argv, "odometry_publisher");
     ros::NodeHandle n;

     ros::Time current_time, last_time;

     // encoder value
     double current_lencoder = 0;
     double current_rencoder = 0;

     double last_lencoder = 0;
     double last_rencoder = 0;

     double ldistance = 0;
     double rdistance = 0;

     double x = 0.0;
     double y = 0.0;
     double th = 0.0;

     double vx = 0.0;
     double vy = 0.0;
     double vth = 0.0;

     double dxy = 0.0;
     double dth = 0.0;
     

     double v_left = 0.0;
     double v_right = 0.0;
     // tf transform
     tf::TransformBroadcaster odom_broadcaster;
     //subscribe command
     ros::Subscriber velcmd_sub = n.subscribe("cmd_vel", 50, velcmd_callback);
     // publish sensor
     ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 500);
     try
     {
/*
         ros_ser.setPort("/dev/lmotor");
         ros_ser.setBaudrate(115200);
         serial::Timeout to = serial::Timeout::simpleTimeout(1000);
         ros_ser.setTimeout(to);
         ros_ser.open();
         */
         init_lserial(lserialstr);
         init_rserial(rserialstr);
     }
     catch (serial::IOException& e)
     {
         ROS_ERROR_STREAM("Unable to open port ");
         return -1;
     }
     if(ros_lser.isOpen() && ros_rser.isOpen()){
         ROS_INFO_STREAM("Serial Port opened");
     }else{
         return -1;
     }

/*
     for(unsigned char i = 0; i<9;i++)
     {
	sdata[i] = i;
     }
     sdata[9] = 0x0a;
*/

     lserial_setting();
     rserial_setting();
    // lmotor_setting(-120);
    // rmotor_setting(120);
   
/*     
     sleep(3);


     lmotor_setting(0);
     rmotor_setting(0);
*/
     ros::Rate loop_rate(100);
    
     ros::Time start_time;
    
     current_time = ros::Time::now(); 
     last_time = ros::Time::now(); 

     while(ros::ok()){
         ros::spinOnce();
         
         start_time = ros::Time::now();
         /*
         enable_serial_feedback();
         usleep(5800);
         disable_serial_feedback();
         */

         current_time = ros::Time::now();
         double dtime = (current_time - last_time).toSec();
/*      
       if(ros_lser.available() && ros_rser.available()){
             
             //ROS_INFO_STREAM("Reading from right serial port:");
             std_msgs::String lserial_data;
             std_msgs::String rserial_data;
             
            // string rserial_data = ros_rser.read(ros_rser.available());
             lserial_data.data = ros_lser.read(100);
             rserial_data.data = ros_rser.read(100);
             
             int datalen = rserial_data.data.length();
             cout << int(datalen)<<endl;
             
             int ltemp[10], rtemp[10];
             for(int i = 0; i < 10; i++){
                ltemp[i] = lserial_data.data.at(i);
                rtemp[i] = rserial_data.data.at(i);
               // cout << int(temp) <<endl;
             }

             int vl = (ltemp[4]<<8)|(ltemp[5]);
             int vr = (rtemp[4]<<8)|(rtemp[5]);
             
             if(abs(vl)>200 || abs(vr)>200){
                cout<<vl <<endl;
                cout<<vr <<endl;
             }
             
             v_left = (double)(((ltemp[4]<<8)|(ltemp[5]))*((double)(2.0))*pi*WHEEL_RADIUS/(REDUCTIONi*60.0));
             v_right = (double)(((rtemp[4]<<8)|(rtemp[5]))*((double)(2.0))*pi*WHEEL_RADIUS/(REDUCTIONi*60.0));
    */
             v_left = (double)(cmd_vel_lwheel*((double)(2.0))*pi*WHEEL_RADIUS/(REDUCTIONi*60.0));
             v_right = (double)(cmd_vel_rwheel*((double)(2.0))*pi*WHEEL_RADIUS/(REDUCTIONi*60.0));
             
             //cout<<"left: "<<v_left <<endl;
             //cout<<"right: "<<v_right<<endl;
             
             vx = (v_left + v_right)/((double)(2.0)); 
             vth = (v_right - v_left)/(WHEEL_DISTANCE);

             double dth = vth*dtime;    
             
             // model a
/*
             x = x + vx*cos(th)*dtime;
             y = y + vx*sin(th)*dtime;
*/

             // model b
             if(abs(vth) < 0.0001){
                x = x + vx*cos(th)*dtime;
                y = y + vx*sin(th)*dtime;
             }
             else{
                double move_radius = vx/vth;

                x = x - move_radius*sin(th) + move_radius*sin(th+dth);
                y = y - move_radius*cos(th) - move_radius*cos(th+dth);
             }
             th = th + dth;

             /* 
             current_lencoder = (double)((ltemp[6]<<24)|(ltemp[7]<<16)|(ltemp[8]<<8)|(ltemp[9]));
             current_rencoder = (double)((rtemp[6]<<24)|(rtemp[7]<<16)|(rtemp[8]<<8)|(rtemp[9]));
             
            // cout << current_lencoder  << endl;
            // cout << current_rencoder  << endl;

             ldistance = (double)((current_lencoder - last_lencoder)*((double)(2.0))*pi*WHEEL_RADIUS/CIRCLENUM); // m
             rdistance = (double)((current_rencoder - last_rencoder)*((double)(2.0))*pi*WHEEL_RADIUS/CIRCLENUM); // m

             last_lencoder = current_lencoder;
             last_rencoder = current_rencoder;

             dxy = (ldistance + rdistance)/((double)(2.0));
             dth = (rdistance - ldistance)/((double)(WHEEL_DISTANCE));

             x = x + dxy*cos(dth); 
             y = y + dxy*sin(dth);

             th = th + dth;

             vx = dxy/dtime;
             vth = dth/dtime;
            */



             geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th);



             //short real_lvel = (short)(ltemp[4]<<8|ltemp[5]);
             //short real_rvel = (short)(rtemp[4]<<8|rtemp[5]);
             //cout << short(real_lvel)<<endl;

             geometry_msgs::TransformStamped odom_trans;
             nav_msgs::Odometry odom;
            

             // tf transform
             //odom_trans.header.stamp = current_time;
             odom_trans.header.stamp = current_time;
             odom_trans.header.frame_id = "odom";
             odom_trans.child_frame_id = "base_link";
             
             odom_trans.transform.translation.x = x;
             odom_trans.transform.translation.y = y;
             odom_trans.transform.translation.z = 0;
             odom_trans.transform.rotation = odom_quat;

             odom_broadcaster.sendTransform(odom_trans);
            
             
             // odometry
             //odom.header.stamp = current_time;
             odom.header.stamp = current_time;
             odom.header.frame_id = "odom";

             // position
             odom.pose.pose.position.x = x;
             odom.pose.pose.position.y = y;
             odom.pose.pose.position.z = 0.0;
             odom.pose.pose.orientation = odom_quat;

             // velocity
             odom.child_frame_id = "base_link";
             odom.twist.twist.linear.x = vx;
             odom.twist.twist.linear.y = 0.0;
             odom.twist.twist.angular.z = vth;

             // publish odometry
             odom_pub.publish(odom);
         
         //double dt = (ros::Time::now()-start_time).toSec();
         //cout << "loop time: " << dt << endl;
         //clear_serial_buffer();
         
         last_time = current_time;
         loop_rate.sleep();
    }
}

