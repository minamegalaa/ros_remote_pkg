#!/usr/bin/env python3

import rospy
from std_msgs.msg import Int32
from std_msgs.msg import String

nodeName='messagesubs'

topicNameLeftEncoder='left_encoder_pulses'
topicNameRightEncoder='right_encoder_pulses'

topicNameLeftAngle='left_angle'
topicNameRightAngle='right_angle'

def callBackFunctionLeftEncoder(message1):
	print("Left encoder pulses: %s" %message1.data)
	
def callBackFunctionRightEncoder(message2):
	print("Right encoder pulses: %s" %message2.data)

def callBackFunctionLeftAngle(message3):
	print("Left angle: %s" %message3.data)
	
def callBackFunctionRightAngle(message4):
	print("Right angle: %s" %message4.data)
	
rospy.init_node(nodeName,anonymous=True)

rospy.Subscriber(topicNameLeftEncoder, Int32, callBackFunctionLeftEncoder)
rospy.Subscriber(topicNameRightEncoder, Int32, callBackFunctionRightEncoder)
rospy.Subscriber(topicNameLeftAngle, String, callBackFunctionLeftAngle)
rospy.Subscriber(topicNameRightAngle, String, callBackFunctionRightAngle)

rospy.spin()
