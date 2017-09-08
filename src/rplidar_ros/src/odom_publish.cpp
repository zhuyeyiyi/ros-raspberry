#include <stdio.h>
#include <stdlib.h>

#include <ros/ros.h>
#include <serial/serial.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <string.h> 
#include <unistd.h> // include sleep(s) and usleep(us) function

#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/Int8MultiArray.h>


#define SERIAL_TIMEOUT 2 // ms
#define ARR_LEN(array,length){length = sizeof(array)/sizeof(array[0]);}

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
    usleep(1000); // sleep 200 ms
}

void rmotor_setting(int velocity_rpm){
    
    //velocity parameter setting, rpm    
    speedpara[4] = (unsigned char)((velocity_rpm)>>8&0xFF);
    speedpara[5] = (unsigned char)(velocity_rpm&0xFF);

    ARR_LEN(speedpara,arrlen);
    ros_rser.write(speedpara,arrlen);
    usleep(1000); // sleep 200 ms
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




int main (int argc, char** argv){
     ros::init(argc, argv, "my_serial_node");
     ros::NodeHandle n;
     //subscribe command
     ros::Subscriber command_sub = n.subscribe("command", 1000, callback);
     // publish sensor
     ros::Publisher sensor_pub = n.advertise<std_msgs::String>("sensor", 50);
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
     lmotor_setting(20);
     rmotor_setting(20);
   
/*     
     sleep(3);


     lmotor_setting(0);
     rmotor_setting(0);
*/
     ros::Rate loop_rate(50);
    
     ros::Time start_time;
    
     while(ros::ok()){
         ros::spinOnce();

         start_time = ros::Time::now();

         clear_serial_buffer();
         enable_serial_feedback();
         usleep(4000);
         disable_serial_feedback();

/*     
         recbuffer = ros_ser.readline(12,"\n");
         const char *recdata = recbuffer.data();
         std_msgs::String recresult;
         recresult.data = recbuffer;
         if(recbuffer.length() == 10){
            ROS_INFO_STREAM("Read:");
            for(int i = 0; i<10; i++){
           // cout << sdata;
            }
         }
    */
        
       if(ros_rser.available()){
             
             ROS_INFO_STREAM("Reading from right serial port:");
             std_msgs::String rserial_data;
             
            // string rserial_data = ros_rser.read(ros_rser.available());
             rserial_data.data = ros_rser.read(60);
             int datalen = rserial_data.data.length();
             
             cout << int(datalen)<<endl;
          
             for(int i = 0; i < 10; i++){
                char tmp = rserial_data.data.at(i);
                cout << int(tmp) <<endl;
             }
            //sensor
             sensor_pub.publish(rserial_data);
       }
         double dtime = (ros::Time::now()-start_time).toSec();
         //cout << dtime << endl;
         clear_serial_buffer();
         loop_rate.sleep();
    }
}

