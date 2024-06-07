#include <ros.h>
#include <std_msgs/Int32.h>
#include "CytronMotorDriver.h"
#include <Servo.h>

// Configure the motor driver.
CytronMD leftmotor(PWM_DIR, 5, 4);  
CytronMD rightmotor(PWM_DIR, 6, 7);  
//encoder interrupt pins
int encoderPinLeft=2;  
int encoderPinRight=3;  
int ENA=8;
int ENB=9;
//left encoder pulses
volatile unsigned long totalPulsesLeft = 0;
//right encoder pulses
volatile unsigned long totalPulsesRight = 0;
//left servo angle
volatile unsigned long AngleLeft = 0;
//right servo angle
volatile unsigned long AngleRight = 0;

// motor velocities - these variables are set by ROS
// left motor
int motorVelocityLeft=0;
// right motor
int motorVelocityRight=0;

// configure the servo
Servo leftservo;
Servo rightservo;

int posleft=0;
int posright=0;

ros::NodeHandle nh;

void callBackFunctionServoLeft(const std_msgs::Int32 &servoAngleLeftROS){
  posleft=servoAngleLeftROS.data;
}
void callBackFunctionServoRight(const std_msgs::Int32 &servoAngleRightROS){
  posright=servoAngleRightROS.data;
}

void callBackFunctionMotorLeft(const std_msgs::Int32 &motorVelocityLeftROS){
  motorVelocityLeft=motorVelocityLeftROS.data;
}
void callBackFunctionMotorRight(const std_msgs::Int32 &motorVelocityRightROS){
  motorVelocityRight=motorVelocityRightROS.data;
}


//publishers for the encoder pulses
//left encoder publisher
std_msgs::Int32 leftEncoderROS;
ros::Publisher leftEncoderROSPublisher("left_encoder_pulses", &leftEncoderROS);
//right encoder publisher
std_msgs::Int32 rightEncoderROS;
ros::Publisher rightEncoderROSPublisher("right_encoder_pulses", &rightEncoderROS);

//left angle publisher
std_msgs::Int32 leftAngleROS;
ros::Publisher leftAngleROSPublisher("left_angle", &leftAngleROS);
//right angle publisher
std_msgs::Int32 rightAngleROS;
ros::Publisher rightAngleROSPublisher("right_angle", &rightAngleROS);


//subscribers for left and right motor velocities
//left motor
ros::Subscriber<std_msgs::Int32>leftMotorROSSubsciber("left_motor_velocity",&callBackFunctionMotorLeft);
//right motor
ros::Subscriber<std_msgs::Int32>rightMotorROSSubsciber("right_motor_velocity",&callBackFunctionMotorRight);

//left servo angle
ros::Subscriber<std_msgs::Int32>leftServoROSSubsciber("left_servo_angle",&callBackFunctionServoLeft);
//right servo angle
ros::Subscriber<std_msgs::Int32>rightServoROSSubsciber("right_servo_angle",&callBackFunctionServoRight);


// The setup routine runs once when you press reset.
void setup() {
  //set the encoder pins
  pinMode(encoderPinLeft, INPUT);
  pinMode(encoderPinRight, INPUT);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  //servo attach pins
  leftservo.attach(10);
  rightservo.attach(11);
  //attach the interrupts for tracking the pulses
  attachInterrupt(digitalPinToInterrupt(encoderPinLeft), interruptFunctionLeft, RISING);
  attachInterrupt(digitalPinToInterrupt(encoderPinRight), interruptFunctionRight, RISING);
  //left motor
 leftmotor.setSpeed(0);     
 //right motor
 rightmotor.setSpeed(0);
 //left servo
 leftservo.write(0);
 //right servo
 rightservo.write(0);

//Setup ROS Stuff
 nh.getHardware()->setBaud(9600);
 nh.initNode();
 //Publishers
 nh.advertise(leftEncoderROSPublisher);
 nh.advertise(rightEncoderROSPublisher);
 nh.advertise(leftAngleROSPublisher);
 nh.advertise(rightAngleROSPublisher);


 //Subscribers
 nh.subscribe(leftMotorROSSubsciber);
 nh.subscribe(rightMotorROSSubsciber);
 nh.subscribe(leftServoROSSubsciber);
 nh.subscribe(rightServoROSSubsciber);
  
}


// The loop routine runs over and over again forever.
void loop() {
nh.spinOnce();
//analogWrite(ENA, motorVelocityLeft);
//Set the direction and turn ON
//left motor
leftmotor.setSpeed(motorVelocityLeft);
//right motor
rightmotor.setSpeed(motorVelocityRight);
//left servo
leftservo.write(posleft);
//right servo
rightservo.write(posright);
//send back the encoder readings
leftEncoderROS.data = totalPulsesLeft;
leftEncoderROSPublisher.publish(&leftEncoderROS);

rightEncoderROS.data = totalPulsesRight;
rightEncoderROSPublisher.publish(&rightEncoderROS);

//send back the servo readings
leftAngleROS.data = AngleLeft;
leftAngleROSPublisher.publish(&leftAngleROS);

rightAngleROS.data = AngleRight;
rightAngleROSPublisher.publish(&rightAngleROS);
Serial.println(motorVelocityLeft);
Serial.println(motorVelocityRight);


delay(5);
}

//interrupt function left encoder
void interruptFunctionLeft(){
        if(motorVelocityLeft < 0){
          totalPulsesLeft--;
        }
        if(motorVelocityLeft > 0){
          totalPulsesLeft++;
        }
}
void interruptFunctionRight(){
        if(motorVelocityRight < 0){
          totalPulsesRight--;
        }
        if(motorVelocityRight > 0){
          totalPulsesRight++;
        }
}
void ServoLeft(){
        if(posleft < 0){
          AngleLeft--;
        }
        if(posleft > 0){
          AngleLeft++;
        }
        
}
void ServoRight(){
        if(posright < 0){
          AngleRight--;
        }
        if(posright> 0){
          AngleRight++;
        }
}