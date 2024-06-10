#!/usr/bin/env python3

import rospy
from ros_remote_pkg.msg import ImuData

def imu_callback(data):
    rospy.loginfo("Received IMU data: Roll=%.2f, Pitch=%.2f, Yaw=%.2f" % (data.rate_roll, data.rate_pitch, data.rate_yaw))

def imu_subscriber():
    rospy.init_node('imu_subscriber', anonymous=True)
    rospy.Subscriber('imu', ImuData, imu_callback)
    rospy.spin()

if __name__ == '__main__':
    try:
        imu_subscriber()
    except rospy.ROSInterruptException:
        pass