#!/usr/bin/env python3

import rospy
from std_msgs.msg import Int32

nodeName='messagepublisher'

topicNameVacuumMotor='vacuum_motor_velocity'


rospy.init_node(nodeName,anonymous=True)

publisherVacuumMotor=rospy.Publisher(topicNameVacuumMotor, Int32, queue_size=5)


ratePublisher=rospy.Rate(1)

VacuumMotor=0

def parseInput(input:str) -> Int32:
  input = input.lower()
  if input == "off":
    return 0
  if input == "low":
    return 85
  if input == "mid":
    return 170
  if input == "high":
    return 255


while not rospy.is_shutdown():
  rospy.loginfo(VacuumMotor)

  userinput = input("Enter vacuum motor velocity (OFF, LOW, MID, HIGH) : \n ")
  VacuumMotor = parseInput(userinput)
  publisherVacuumMotor.publish(VacuumMotor)

  ratePublisher.sleep()
