#include "ros/ros.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string> 
#include <iostream>
#include <std_msgs/Float64.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Int32MultiArray.h>
#include <std_msgs/UInt32MultiArray.h>
#include <std_msgs/Int32.h>
#include <pthread.h>
#include <geometry_msgs/TwistStamped.h>
#include <mutex>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <geometry_msgs/TwistStamped.h>
#include "std_msgs/String.h"

using namespace std;

ros::Publisher mCmdvelPub_;
int agv_stop = 1;
uint8_t speed_running = 1;
void warning_player();
string playPath_path;

struct Sensor
{
	float Ultrasonic_data[3];    
	float imu_sensor;
	float robot_power;
	float stop;
        
}Sensor_DATA;

void Stop_Sensor_Data(const std_msgs::Int32::ConstPtr& data)
{
	Sensor_DATA.stop  = data->data;
	if(agv_stop) agv_stop = data->data ;
		
	if(data->data == 0)  
		warning_player();	
}

// void  my_player(int num)
// {

//      const char *mplayer[] = {       "mplayer //home/hgrobot/arv_ws/src/collision_prompt/"," < /dev/null > /dev/null 2>check_erro.log "};
//      const char *voise_tip[]={       "voice/stop.mp3"};
//      char check_tip[200] = {0};

//      strcpy(check_tip,mplayer[0]);
//      strcat(check_tip,voise_tip[num]);
//      strcat(check_tip,mplayer[1]);     
//      system(check_tip);
//      usleep(1000*500);

// }

// 语音播报函数
void  warning_player()
{
	ros::NodeHandle nh("~");    //用于launch文件传递参数
	nh.param("playPath_path", playPath_path, std::string("play ./voice.wav"));    //从launch文件获取参数
	// string path = playPath_path;
	const char *playPath = playPath_path.data();
	system(playPath);
	usleep(1000*1000);
	// sleep(3*1000);	//	延时5S
}


void  claer_sensor_data(void)
{
	Sensor_DATA.Ultrasonic_data[0] = 0;
	Sensor_DATA.Ultrasonic_data[1] = 0;
	Sensor_DATA.Ultrasonic_data[2] = 0;

        Sensor_DATA.robot_power = 0;
	Sensor_DATA.imu_sensor = 0;
}


void set_speed_dir(unsigned int dir)
{
	geometry_msgs::Twist current_vel; 

	if (dir == 0)
	{
		current_vel.linear.x = 0.15;
		current_vel.linear.y = 0;
		current_vel.linear.z = 0;
		current_vel.angular.x = 0;
		current_vel.angular.y = 0;
		current_vel.angular.z = 0;

	}else if (dir == 1)
	{
		current_vel.linear.x = -0.15;
		current_vel.linear.y = 0;
		current_vel.linear.z = 0;
		current_vel.angular.x = 0;
		current_vel.angular.y = 0;
		current_vel.angular.z = 0;
	}

        mCmdvelPub_.publish(current_vel);

}


void *speed_contorl(void *parameter)
{
	unsigned int count = 0;	
	unsigned int i = 0;	

	pthread_detach(pthread_self()); 
	while(speed_running)
	{
		if(agv_stop)
		{
			if(count % 50 == 0)
			{
				set_speed_dir(i%2);
		 		i++;
			}

		}
		usleep(1000*100);  //100MS
		count++;
	}

	pthread_exit(NULL);
}
 
int main(int argc,char **argv)
{

	ros::init(argc, argv, "COLLISION_PROMPT_NODE");
	ros::NodeHandle n;		//创建句柄
	ros::NodeHandle nh("~");	
	std::string playPath_path;

	nh.param("playPath_path", playPath_path, std::string("play ./voice.wav"));    //从launch文件获取参数

	ros::Subscriber Stop_Sub 	= n.subscribe("/Stop_Sensor_Data", 10, &Stop_Sensor_Data);
	mCmdvelPub_ = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1, true);			

	pthread_t  speed;
	pthread_create(&speed, NULL, speed_contorl, NULL);

	Sensor_DATA.stop = 1;

	printf("HG-AGV碰撞检测程序\n");

    while(ros::ok())
    {
		ros::spinOnce();//循环等待回调函数	
    }

	speed_running = 0;
  
}



