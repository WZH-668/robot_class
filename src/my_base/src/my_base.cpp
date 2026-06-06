#include "bot_serial.h"

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    turn_on_robot Robot_Control;//Instantiate an object //实例化一个对象
    Robot_Control.Control();
    rclcpp::shutdown();
    return 0;
}
void turn_on_robot::Control()
{ 
    _Last_Time=rclcpp::Node::now();
    while(rclcpp::ok())
    {
        try
        {
             _Now = rclcpp::Node::now();
             Sampling_Time = (_Now - _Last_Time).seconds(); 
             if (true == Get_Sensor_Data_New()) 
             {
                printf("new!");
                 Robot_Pos.X+=(Robot_Vel.X * cos(Robot_Pos.Z) - Robot_Vel.Y * sin(Robot_Pos.Z)) * Sampling_Time;
                 RCLCPP_INFO(this->get_logger(),"RobotX:%f",Robot_Pos.X);
                
            //     // 计算X方向的位移，单位：m
                 Robot_Pos.Y+=(Robot_Vel.X * sin(Robot_Pos.Z) + Robot_Vel.Y * cos(Robot_Pos.Z)) * Sampling_Time; 
            //    //计算Y方向的位移，单位：m
                 Robot_Pos.Z+=Robot_Vel.Z * Sampling_Time; 
             //绕Z轴的角位移，单位：rad 
                 Publish_Odom();      
                 
                 // 发布 IMU 数据
                 sensor_msgs::msg::Imu imu_msg;
                 imu_msg.header.stamp = _Now;
                 imu_msg.header.frame_id = "imu_Link";
                 
                 // 转换物理单位 (假设 STM32 发送的是原始数据，按常用量程转换)
                 // 加速度转换 (假设量程为 2g, 1g=9.80665 m/s^2)
                 imu_msg.linear_acceleration.x = Receive_Data.Accel_X / 16384.0 * 9.80665;
                 imu_msg.linear_acceleration.y = Receive_Data.Accel_Y / 16384.0 * 9.80665;
                 imu_msg.linear_acceleration.z = Receive_Data.Accel_Z / 16384.0 * 9.80665;
                 
                 // 角速度转换 (假设量程为 2000 deg/s, 转换为 rad/s)
                 imu_msg.angular_velocity.x = Receive_Data.Gyro_X / 16.4 * (3.14159 / 180.0);
                 imu_msg.angular_velocity.y = Receive_Data.Gyro_Y / 16.4 * (3.14159 / 180.0);
                 imu_msg.angular_velocity.z = Receive_Data.Gyro_Z / 16.4 * (3.14159 / 180.0);
                 
                 // 设置协方差
                 for(int i=0; i<9; i++) {
                     imu_msg.orientation_covariance[i] = (i%4==0) ? -1.0 : 0.0; // 不提供姿态
                     imu_msg.angular_velocity_covariance[i] = (i%4==0) ? 0.02 : 0.0;
                     imu_msg.linear_acceleration_covariance[i] = (i%4==0) ? 0.04 : 0.0;
                 }
                 
                 imu_publisher->publish(imu_msg);
                 
                 // 发布电池电压
                 std_msgs::msg::Float32 batt_msg;
                 batt_msg.data = Receive_Data.Battery_Voltage;
                 battery_publisher->publish(batt_msg);
                 
                 _Last_Time = _Now;
             }
             
            rclcpp::spin_some(this->get_node_base_interface());
        }
        catch (const rclcpp::exceptions::RCLError & e )
        {
            RCLCPP_ERROR(this->get_logger(),"unexpectedly failed whith %s",e.what());	
        }
    }
}
turn_on_robot::turn_on_robot():rclcpp::Node ("wheeltec_robot")
{
    // Declare parameters
    this->declare_parameter("pub_odom_tf", true);
    pub_odom_tf = this->get_parameter("pub_odom_tf").as_bool();
    
    // Initialize state variables
    Fan_State = false;
    Cmd_Linear_X = 0.0;
    Cmd_Linear_Y = 0.0;
    Cmd_Angular_Z = 0.0;

    tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(this);
    Cmd_Vel_Sub = create_subscription<geometry_msgs::msg::Twist>(
    "/cmd_vel", 2, std::bind(&turn_on_robot::Cmd_Vel_Callback, this, _1));
    Fan_Cmd_Sub = create_subscription<std_msgs::msg::Bool>(
    "/fan_cmd", 2, std::bind(&turn_on_robot::Fan_Cmd_Callback, this, _1));
     RCLCPP_INFO(this->get_logger(),"wheeltec_robot Data ready"); //Prompt message //提示信息
     RCLCPP_INFO(this->get_logger(), "pub_odom_tf: %s", pub_odom_tf ? "true" : "false");
     odom_publisher= create_publisher<nav_msgs::msg::Odometry>("odom",2);
     imu_publisher = create_publisher<sensor_msgs::msg::Imu>("imu/data_raw", 2);
     battery_publisher = create_publisher<std_msgs::msg::Float32>("battery_voltage", 2);
      try
     { 

         Stm32_Serial.setPort("/dev/wheeltec_controller"); //Select the serial port number to enable //选择要开启的串口号
       
         Stm32_Serial.setBaudrate(115200); //Set the baud rate //设置波特率
         serial::Timeout _time = serial::Timeout::simpleTimeout(2000); //Timeout //超时等待
         Stm32_Serial.setTimeout(_time);
         Stm32_Serial.open(); //Open the serial port //开启串口
     }
     catch (serial::IOException& e)
     {
         std::cerr<<e.what()<<std::endl;
         RCLCPP_ERROR(this->get_logger(),"wheeltec_robot can not open serial port,Please check the serial port cable! "); //If opening the serial port fails, an error message is printed //如果开启串口失败，打印错误信息
     }
     if(Stm32_Serial.isOpen())
     {
         RCLCPP_INFO(this->get_logger(),"wheeltec_robot serial port opened"); //Serial port opened successfully //串口开启成功提示
     }
    


     
}

void turn_on_robot::Send_Control_Frame(float linear_x, float linear_y, float angular_z)
{
  RCLCPP_INFO(this->get_logger(),
    "[dbg] send_control_frame input linear_x=%.3f linear_y=%.3f angular_z=%.3f serial_open=%s",
    linear_x, linear_y, angular_z, Stm32_Serial.isOpen() ? "true" : "false");

  Send_Data.tx[0] = FRAME_HEADER;
  Send_Data.tx[1] = 0;
  Send_Data.tx[2] = Fan_State ? 1 : 0;

  short temp = 0;
  temp = -linear_x * 1000;
  Send_Data.tx[4] = temp;
  Send_Data.tx[3] = temp >> 8;

  temp = linear_y * 1000;
  Send_Data.tx[6] = temp;
  Send_Data.tx[5] = temp >> 8;

  temp = angular_z * 1000;
  Send_Data.tx[8] = temp;
  Send_Data.tx[7] = temp >> 8;

  Send_Data.tx[9] = Check_Sum(9, SEND_DATA_CHECK);
  Send_Data.tx[10] = FRAME_TAIL;

  try
  {
    size_t written = Stm32_Serial.write(Send_Data.tx, sizeof(Send_Data.tx));
    RCLCPP_INFO(this->get_logger(),
      "[dbg] serial_write bytes=%zu frame=[%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u]",
      written,
      Send_Data.tx[0], Send_Data.tx[1], Send_Data.tx[2], Send_Data.tx[3], Send_Data.tx[4],
      Send_Data.tx[5], Send_Data.tx[6], Send_Data.tx[7], Send_Data.tx[8], Send_Data.tx[9], Send_Data.tx[10]);
  }
  catch (serial::IOException& e)
  {
    RCLCPP_ERROR(this->get_logger(), "Unable to send data through serial port");
  }
}




void turn_on_robot::Cmd_Vel_Callback(const geometry_msgs::msg::Twist::SharedPtr twist_aux)
{
  RCLCPP_INFO(this->get_logger(),
    "[dbg] cmd_vel received linear=(%.3f, %.3f, %.3f) angular=(%.3f, %.3f, %.3f)",
    twist_aux->linear.x, twist_aux->linear.y, twist_aux->linear.z,
    twist_aux->angular.x, twist_aux->angular.y, twist_aux->angular.z);

  Cmd_Linear_X = twist_aux->linear.x;
  Cmd_Linear_Y = twist_aux->linear.y;
  Cmd_Angular_Z = twist_aux->angular.z;

  Send_Control_Frame(Cmd_Linear_X, Cmd_Linear_Y, Cmd_Angular_Z);
}

void turn_on_robot::Fan_Cmd_Callback(const std_msgs::msg::Bool::SharedPtr msg)
{
  Fan_State = msg->data;

  Send_Control_Frame(Cmd_Linear_X, Cmd_Linear_Y, Cmd_Angular_Z);
}

void turn_on_robot::Publish_Odom()
{
     //Convert the Z-axis rotation Angle into a quaternion for expression 
//     //把Z轴转角转换为四元数进行表达
     tf2::Quaternion q;
     q.setRPY(0,0,Robot_Pos.Z);
     geometry_msgs::msg::Quaternion odom_quat=tf2::toMsg(q);
     geometry_msgs::msg::TransformStamped odom_tf;
    nav_msgs::msg::Odometry odom; //Instance the odometer topic data //实例化里程计话题数据
     odom.header.stamp = rclcpp::Node::now(); ; 
     odom.header.frame_id = "odom"; // Odometer TF parent coordinates //里程计TF父坐标
     odom.pose.pose.position.x = Robot_Pos.X; //Position //位置
     odom.pose.pose.position.y = Robot_Pos.Y;
     odom.pose.pose.position.z = 0.0; // 2D 机器人位置 z 固定为 0
     odom.pose.pose.orientation = odom_quat; //Posture, Quaternion converted by Z-axis rotation //姿态，通过Z轴转角转换的四元数

     odom.child_frame_id = "base_footprint"; // Odometer TF subcoordinates //里程计TF子坐标
     odom.twist.twist.linear.x =  Robot_Vel.X; //Speed in the X direction //X方向速度
     odom.twist.twist.linear.y =  Robot_Vel.Y; //Speed in the Y direction //Y方向速度
     odom.twist.twist.angular.z = Robot_Vel.Z; //Angular velocity around the Z axis //绕Z轴角速度        
     if(Robot_Vel.X== 0&&Robot_Vel.Y== 0&&Robot_Vel.Z== 0)
//     //If the velocity is zero, it means that the error of the encoder will be relatively small, and the data of the encoder will be considered more reliable
//     //如果velocity是零，说明编码器的误差会比较小，认为编码器数据更可靠
     {
         memcpy(&odom.pose.covariance, odom_pose_covariance2, sizeof(odom_pose_covariance2));
         memcpy(&odom.twist.covariance, odom_twist_covariance2, sizeof(odom_twist_covariance2));
    
     }
      else
     //If the velocity of the trolley is non-zero, considering the sliding error that may be brought by the encoder in motion, the data of IMU is considered to be more reliable
//     //如果小车velocity非零，考虑到运动中编码器可能带来的滑动误差，认为imu的数据更可靠
    {
     memcpy(&odom.pose.covariance, odom_pose_covariance, sizeof(odom_pose_covariance));
     memcpy(&odom.twist.covariance, odom_twist_covariance, sizeof(odom_twist_covariance));  
 
     }
     odom_publisher->publish(odom); //Pub odometer topic //发布里程计话题

     odom_tf.header.stamp = odom.header.stamp;
     odom_tf.header.frame_id = "odom";
     odom_tf.child_frame_id = "base_footprint";
     odom_tf.transform.translation.x = Robot_Pos.X;
     odom_tf.transform.translation.y = Robot_Pos.Y;
     odom_tf.transform.translation.z = 0.0; // 2D 机器人通常 z=0
     odom_tf.transform.rotation = odom_quat;
     
     // Only publish TF if pub_odom_tf is true (for EKF fusion compatibility)
     // 只有当 pub_odom_tf 为 true 时才发布 TF（兼容 EKF 融合）
     if (pub_odom_tf) {
         tf_broadcaster_->sendTransform(odom_tf);
     }
}
