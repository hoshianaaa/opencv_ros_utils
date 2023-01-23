#!/usr/bin/env python

import os
import rospy
from sensor_msgs.msg import Image
import cv2
from cv_bridge import CvBridge
import sys
import math
from std_msgs.msg import *
import numpy as np


image = None
msg = None
bridge = None

def callback(cb_msg):
  global msg, image, bridge

  angle = cb_msg.data
  print(image)
  if image is not None:
    a = np.radians(angle)
    h, w = image.shape[:2]
    w_rot = int(np.round(w*abs(np.cos(a)) + h*abs(np.sin(a))))
    h_rot = int(np.round(w*abs(np.sin(a)) + h*abs(np.cos(a))))

    M = cv2.getRotationMatrix2D((w/2,h/2), angle, 1)
    M[0][2] += -w/2 + w_rot/2
    M[1][2] += -h/2 + h_rot/2

    img_rot  = cv2.warpAffine(image, M, (w_rot,h_rot))

    msg = bridge.cv2_to_imgmsg(img_rot, encoding="bgr8")

filepath = sys.argv[1]

rospy.init_node('img_pub_from_file', anonymous=True)
sub = rospy.Subscriber('img_publisher/rotate', Float64, callback)
pub = rospy.Publisher('usb_cam/image_raw', Image, queue_size=10)
# read image
#filepath = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'lena.png')
image = cv2.imread(filepath, cv2.IMREAD_COLOR)
# make bridge
bridge = CvBridge()
msg = bridge.cv2_to_imgmsg(image, encoding="bgr8")
rate = rospy.Rate(10) # 1hz
while not rospy.is_shutdown():
    pub.publish(msg)
    rate.sleep()
