#ifndef bot_serial
#define bot_serial
#include <rclcpp/rclcpp.hpp>
#include <serial/serial.h>
#include <string>
#include <stdio.h>
#include "geometry_msgs/msg/twist.hpp"
#include <nav_msgs/msg/odometry.hpp>
#include <sensor_msgs/msg/imu.hpp>
#include <std_msgs/msg/float32.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"
#include "std_msgs/msg/bool.hpp"

//宏定义
#define SEND_DATA_CHECK   1          //Send data check flag bits //发送数据校验标志位
#define READ_DATA_CHECK   0          //Receive data to check flag bits //接收数据校验标志位
#define FRAME_HEADER      0X7B       //Frame head //帧头
#define FRAME_TAIL        0X7D       //Frame tail //帧尾
#define RECEIVE_DATA_SIZE 24         //The length of the data sent by the lower computer //下位机发送过来的数据的长度
#define SEND_DATA_SIZE    11         //The length of data sent by ROS to the lower machine //ROS向下位机发送的数据的长度
#define PI 				  3.1415926f //PI //圆周率
using std::placeholders::_1;
const double odom_pose_covariance[36]   = {1e-3,    0,    0,   0,   0,    0, 
  0, 1e-3,    0,   0,   0,    0,
  0,    0,  1e6,   0,   0,    0,
  0,    0,    0, 1e6,   0,    0,
  0,    0,    0,   0, 1e6,    0,
  0,    0,    0,   0,   0,  1e3 };

const double odom_pose_covariance2[36]  = {1e-9,    0,    0,   0,   0,    0, 
  0, 1e-3, 1e-9,   0,   0,    0,
  0,    0,  1e6,   0,   0,    0,
  0,    0,    0, 1e6,   0,    0,
  0,    0,    0,   0, 1e6,    0,
  0,    0,    0,   0,   0, 1e-9 };

const double odom_twist_covariance[36]  = {1e-3,    0,    0,   0,   0,    0, 
  0, 1e-3,    0,   0,   0,    0,
  0,    0,  1e6,   0,   0,    0,
  0,    0,    0, 1e6,   0,    0,
  0,    0,    0,   0, 1e6,    0,
  0,    0,    0,   0,   0,  1e3 };
  
const double odom_twist_covariance2[36] = {1e-9,    0,    0,   0,   0,    0, 
  0, 1e-3, 1e-9,   0,   0,    0,
  0,    0,  1e6,   0,   0,    0,
  0,    0,    0, 1e6,   0,    0,
  0,    0,    0,   0, 1e6,    0,
  0,    0,    0,   0,   0, 1e-9} ;
typedef struct _RECEIVE_DATA_     
{
    uint8_t rx[RECEIVE_DATA_SIZE];
    uint8_t Flag_Stop;
    unsigned char Frame_Header;
    float X_speed;  
    float Y_speed;  
    float Z_speed;  	
    // 新增 IMU 数据字段
    float Accel_X;
    float Accel_Y;
    float Accel_Z;
    float Gyro_X;
    float Gyro_Y;
    float Gyro_Z;
    // 新增电压字段
    float Battery_Voltage;
    unsigned char Frame_Tail;
}RECEIVE_DATA;
//ROS向下位机发送数据的结构体
typedef struct _SEND_DATA_  
{
	  uint8_t tx[SEND_DATA_SIZE];
		float X_speed;	       
		float Y_speed;           
		float Z_speed;         
		unsigned char Frame_Tail; 
}SEND_DATA;
typedef struct __Vel_Pos_Data_
{
	float X;
	float Y;
	float Z;
}Vel_Pos_Data;
class turn_on_robot : public rclcpp::Node
{
	public:
		turn_on_robot();  //Constructor //构造函数
		~turn_on_robot(); //Destructor //析构函数
		void Control();   //Loop control code //循环控制代码
		serial::Serial Stm32_Serial; //Declare a serial object //声明串口对象
    std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_; 
	private:
		rclcpp::Time _Now, _Last_Time;  //Time dependent, used for integration to find displacement (mileage) //时间相关，用于积分求位移(里程)
		rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr Cmd_Vel_Sub;
    rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odom_publisher; 
    rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr imu_publisher;
    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr battery_publisher;
    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr Fan_Cmd_Sub;
    //Initialize the topic subscriber //初始化话题订阅者
    float Sampling_Time = 0.0f;         //Sampling time, used for integration to find displacement (mileage) //采样时间，用于积分求位移(里程)
    Vel_Pos_Data Robot_Vel{0.0f, 0.0f, 0.0f};
    Vel_Pos_Data Robot_Pos{0.0f, 0.0f, 0.0f};
    RECEIVE_DATA Receive_Data{}; //The serial port receives the data structure //串口接收数据结构体
    SEND_DATA Send_Data{};       //The serial port sends the data structure //串口发送数据结构体 
    bool Get_Sensor_Data_New();
    unsigned char Check_Sum(unsigned char Count_Number,unsigned char mode);
    void Cmd_Vel_Callback(const geometry_msgs::msg::Twist::SharedPtr twist_aux);
    void Publish_Odom();      //Pub the speedometer topic //发布里程计话题 
    short IMU_Trans(uint8_t Data_High,uint8_t Data_Low);
    float Odom_Trans(uint8_t Data_High,uint8_t Data_Low);
    bool Fan_State = false;
    float Cmd_Linear_X = 0;
    float Cmd_Linear_Y = 0;
    float Cmd_Angular_Z = 0;
    bool pub_odom_tf = true;
    // 反向停止功能相关变量
    rclcpp::Time last_cmd_time_{0, 0, RCL_ROS_TIME};
    rclcpp::TimerBase::SharedPtr brake_timer_;
    bool brake_active_ = false;
    int brake_phase_ = 0; // 0: 无, 1: 反向制动中, 2: 停止中
    rclcpp::Time brake_start_time_{0, 0, RCL_ROS_TIME};
    // 保存最后有效的速度命令（用于反向制动）
    float last_valid_linear_x_ = 0.0f;
    float last_valid_linear_y_ = 0.0f;
    float last_valid_angular_z_ = 0.0f;
    bool have_last_valid_cmd_ = false;
    // 零速度检测相关
    bool zero_cmd_received_ = false;
    rclcpp::Time zero_cmd_time_{0, 0, RCL_ROS_TIME};
    const double BRAKE_REVERSE_DURATION = 0.1;  // 反向时间 100ms
    const double BRAKE_STOP_DURATION = 0.1;      // 停止时间 100ms
    const double ZERO_CMD_DELAY = 0.1;            // 收到零速度后等待时间再制动
    const float BRAKE_REVERSE_SCALE = -10.0f;      // 反向系数
    uint64_t rx_frame_attempts_ = 0;
    uint64_t rx_frame_success_ = 0;
    uint64_t rx_tail_error_ = 0;
    uint64_t rx_checksum_error_ = 0;
    uint64_t rx_sync_drop_bytes_ = 0;
    rclcpp::Time last_good_frame_time_{0, 0, RCL_ROS_TIME};

    void Fan_Cmd_Callback(const std_msgs::msg::Bool::SharedPtr msg);
    void Send_Control_Frame(float linear_x, float linear_y, float angular_z);
    void Brake_Timer_Callback(); // 反向停止定时器回调
 

    };
short turn_on_robot::IMU_Trans(uint8_t Data_High,uint8_t Data_Low)
{
  short transition_16;
  transition_16 = 0;
  transition_16 |=  Data_High<<8;   
  transition_16 |=  Data_Low;
  return transition_16;     
}
float turn_on_robot::Odom_Trans(uint8_t Data_High,uint8_t Data_Low)
{
  float data_return;
  short transition_16;
  transition_16 = 0;
  transition_16 |=  Data_High<<8;  //Get the high 8 bits of data   //获取数据的高8位
  transition_16 |=  Data_Low;      //Get the lowest 8 bits of data //获取数据的低8位
  data_return   =  (transition_16 / 1000)+(transition_16 % 1000)*0.001; // The speed unit is changed from mm/s to m/s //速度单位从mm/s转换为m/s
  return data_return;
}
unsigned char turn_on_robot::Check_Sum(unsigned char Count_Number,unsigned char mode)
{
    unsigned char check_sum=0,k;
    if(mode==0) //Receive data mode 
    {
        for(k=0;k<Count_Number;k++)
        {
            check_sum ^= Receive_Data.rx[k]; // 回传帧接收改为 XOR 校验
        }
    }
    if(mode==1) //Send data mode 
    {
        for(k=0;k<Count_Number;k++)
        {
            check_sum ^= Send_Data.tx[k]; // 控制帧发送保持 XOR 校验
        }
    }
    return check_sum;
}
bool turn_on_robot::Get_Sensor_Data_New()
{
  uint8_t check=0;
  static int count = 0;
  uint8_t data_byte;

  while (Stm32_Serial.available() > 0)
  {
    Stm32_Serial.read(&data_byte, 1);

    if (count == 0 && data_byte != FRAME_HEADER)
    {
      rx_sync_drop_bytes_++;
      continue;
    }

    Receive_Data.rx[count] = data_byte;
    count++;

    if (count >= 24)
    {
      count = 0;
      rx_frame_attempts_++;
      Receive_Data.Frame_Header = Receive_Data.rx[0];
      Receive_Data.Frame_Tail = Receive_Data.rx[23];

      if (Receive_Data.Frame_Tail != FRAME_TAIL)
      {
        rx_tail_error_++;
      }
      else
      {
        check = Check_Sum(22, READ_DATA_CHECK);
        if (check == Receive_Data.rx[22])
        {
          rx_frame_success_++;
          rclcpp::Time now = this->get_clock()->now();
          if (last_good_frame_time_.nanoseconds() != 0) {
            double dt = (now - last_good_frame_time_).seconds();
            if (dt > 0.2) {
              RCLCPP_WARN(this->get_logger(), "[rxdbg] slow frame dt=%.3fs attempts=%lu success=%lu tail_err=%lu checksum_err=%lu sync_drop=%lu", dt, rx_frame_attempts_, rx_frame_success_, rx_tail_error_, rx_checksum_error_, rx_sync_drop_bytes_);
            }
          }
          last_good_frame_time_ = now;
          if (rx_frame_success_ % 20 == 0) {
            RCLCPP_INFO(this->get_logger(), "[rxdbg] summary attempts=%lu success=%lu tail_err=%lu checksum_err=%lu sync_drop=%lu", rx_frame_attempts_, rx_frame_success_, rx_tail_error_, rx_checksum_error_, rx_sync_drop_bytes_);
          }

          Receive_Data.Flag_Stop = Receive_Data.rx[1];
          Robot_Vel.X = -Odom_Trans(Receive_Data.rx[2], Receive_Data.rx[3]);
          Robot_Vel.Y = Odom_Trans(Receive_Data.rx[4], Receive_Data.rx[5]);
          Robot_Vel.Z = Odom_Trans(Receive_Data.rx[6], Receive_Data.rx[7]);
          Receive_Data.Accel_X = (short)((Receive_Data.rx[8]<<8)  | Receive_Data.rx[9]);
          Receive_Data.Accel_Y = (short)((Receive_Data.rx[10]<<8) | Receive_Data.rx[11]);
          Receive_Data.Accel_Z = (short)((Receive_Data.rx[12]<<8) | Receive_Data.rx[13]);
          Receive_Data.Gyro_X = (short)((Receive_Data.rx[14]<<8)  | Receive_Data.rx[15]);
          Receive_Data.Gyro_Y = (short)((Receive_Data.rx[16]<<8)  | Receive_Data.rx[17]);
          Receive_Data.Gyro_Z = (short)((Receive_Data.rx[18]<<8)  | Receive_Data.rx[19]);
          Receive_Data.Battery_Voltage = ((Receive_Data.rx[20]<<8) | Receive_Data.rx[21]) / 1000.0;
          return true;
        }
        rx_checksum_error_++;
      }

      if ((rx_tail_error_ + rx_checksum_error_) % 20 == 0) {
        RCLCPP_WARN(this->get_logger(), "[rxdbg] parse_fail attempts=%lu success=%lu tail_err=%lu checksum_err=%lu sync_drop=%lu", rx_frame_attempts_, rx_frame_success_, rx_tail_error_, rx_checksum_error_, rx_sync_drop_bytes_);
      }
    }
  }
  return false;
}
turn_on_robot::~turn_on_robot()
{
   //对象turn_on_robot结束前向下位机发送停止运动命令
  Send_Data.tx[0]=FRAME_HEADER;
  Send_Data.tx[1] = 0;  
  Send_Data.tx[2] = 0; 

  //The target velocity of the X-axis of the robot //机器人X轴的目标线速度 
  Send_Data.tx[4] = 0;     
  Send_Data.tx[3] = 0;  

  //The target velocity of the Y-axis of the robot //机器人Y轴的目标线速度 
  Send_Data.tx[6] = 0;
  Send_Data.tx[5] = 0;  

  //The target velocity of the Z-axis of the robot //机器人Z轴的目标角速度 
  Send_Data.tx[8] = 0;  
  Send_Data.tx[7] = 0;    
  Send_Data.tx[9]=Check_Sum(9,SEND_DATA_CHECK); //Check the bits for the Check_Sum function //校验位，规则参见Check_Sum函数
  Send_Data.tx[10]=FRAME_TAIL; 
  try
   {
     Stm32_Serial.write(Send_Data.tx,sizeof (Send_Data.tx)); //Send data to the serial port //向串口发数据  
   }
   catch (serial::IOException& e)   
   {
     RCLCPP_ERROR(this->get_logger(),"Unable to send data through serial port"); //If sending data fails, an error message is printed //如果发送数据失败,打印错误信息
   }
   Stm32_Serial.close(); //Close the serial port //关闭串口  
   RCLCPP_INFO(this->get_logger(),"Shutting down"); //Prompt message //提示信息
}
#endif