#!/usr/bin/env python

import rospy
from std_msgs.msg import *
from geometry_msgs.msg import *

# start diff
def sd():
  global start
  return now() - start # ex: 0.1 (0.1s)

# last diff
def ld():
  global last
  n = now()
  diff = n - last
  last = n
  return diff

def now():
  return rospy.get_time()

def callback(msg):
  print(msg.data)

rospy.init_node("aaa")
start= now()
last = now()

pub_topic_name = rospy.get_param("~pub", "data")
sub_topic_name = rospy.get_param("~sub", "data")

pub = rospy.Publisher(pub_topic_name, Float64, queue_size=10)
rospy.Subscriber(sub_topic_name, Float64, callback)

r = rospy.Rate(10)

while not rospy.is_shutdown():
  pub.publish(Float64(0))
  print("sd:",sd())
  print("ld:",ld())
  r.sleep()

'''
- example run command
rosrun opencv_ros_utils aaa.py _pub:=aaa _sub:=aaa
'''

