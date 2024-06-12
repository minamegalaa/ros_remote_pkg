#!/usr/bin/env python3

import rospy
from std_msgs.msg import Int32

nodeName='messagepublisher'

topicNameLeftMotor='left_motor_velocity'
topicNameRightMotor='right_motor_velocity'

topicNameLeftServo='left_servo_angle'
topicNameRightServo='right_servo_angle'

rospy.init_node(nodeName,anonymous=True)

publisherLeftMotor=rospy.Publisher(topicNameLeftMotor, Int32, queue_size=5)
publisherRightMotor=rospy.Publisher(topicNameRightMotor, Int32, queue_size=5)

publisherLeftServo=rospy.Publisher(topicNameLeftServo, Int32, queue_size=5)
publisherRightServo=rospy.Publisher(topicNameRightServo, Int32, queue_size=5)

ratePublisher=rospy.Rate(1)

leftMotor=0
rightMotor=0

leftServo=0
rightServo=0

def parseInput(input:str) :
	input = input.lower()
	if input == "n":
		leftmotor = -255
		rightmotor = -255
		leftservo = 0
		rightservo = 0
	elif input == "s":
		leftmotor = 255
		rightmotor = -255
		leftservo = 0
		rightservo = 0
	elif input == "e":
		leftmotor = -255
		rightmotor = -255
		leftservo = 90
		rightservo = 90
	elif input == "w":
		leftmotor = 255
		rightmotor = 255
		leftservo = 90
		rightservo = 90
	elif input == "ne":
		leftmotor = 255
		rightmotor = 255
		leftservo = 45
		rightservo = 45
	elif input == "sw":
		leftmotor = -255
		rightmotor = -255
		leftservo = 45
		rightservo = 45
	elif input == "nw":
		leftmotor = 255
		rightmotor = 255
		leftservo = 135
		rightservo = 135
	elif input == "se":
		leftmotor = -255
		rightmotor = -255
		leftservo = 135
		rightservo = 135
	elif input == "off":
		leftmotor = 0
		rightmotor = 0
		leftservo = 0
		rightservo = 0

	return leftmotor, rightmotor, leftservo, rightservo


while not rospy.is_shutdown():
  rospy.loginfo(leftMotor)
  rospy.loginfo(rightMotor)
  rospy.loginfo(leftServo)
  rospy.loginfo(rightServo)

  userinput = input("Enter Direction : \n ")
  leftMotor, rightMotor, leftServo, rightServo = parseInput(userinput)

  #userinput = input("Enter left motor velocity (lowf,midf,highf,lowb,midb,highb,off) : \n ")
  #leftMotor = parseInput(userinput)
  #userinput = input("Enter right motor velocity (lowf,midf,highf,lowb,midb,highb,off) : \n ")
  #rightMotor = parseInput(userinput)

  #leftServo = int(input("Enter left servo angle (0-360) : \n "))
  #rightServo = int(input("Enter right servo angle (0-360) : \n "))
  for i in range(10):
    publisherLeftMotor.publish(leftMotor)
    publisherRightMotor.publish(rightMotor)
    publisherLeftServo.publish(leftServo)
    publisherRightServo.publish(rightServo)
  
  ratePublisher.sleep()

  #leftMotor = int(input("Enter left motor velocity (-255-255) : \n "))
  #rightMotor = int(input("Enter right motor velocity (-255-255) : \n "))

  #leftServo = int(input("Enter left servo angle (0-360) : \n "))
  #rightServo = int(input("Enter right servo angle (0-360) : \n "))
  #for i in range(10):
    #publisherLeftMotor.publish(leftMotor)
    #publisherRightMotor.publish(rightMotor)
    #publisherLeftServo.publish(leftServo)
    #publisherRightServo.publish(rightServo)
  
  #ratePublisher.sleep()
