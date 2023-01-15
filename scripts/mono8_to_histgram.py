#!/usr/bin/env python
import rospy
from sensor_msgs.msg import Image 
from cv_bridge import CvBridge
import cv2

import matplotlib.pyplot as plt
import numpy as np
import io

def now():
    return rospy.get_time()

def img_to_hist(img):
# ヒストグラムの取得
#  img_hist_cv = cv2.calcHist([img], [0], None, [256], [0, 256])

# ヒストグラムの表示
#  plt.plot(img_hist_cv)

  plt.hist(img.ravel(),255,[0,256])

  buf = io.BytesIO()
  plt.savefig(buf, format='png')
  enc = np.frombuffer(buf.getvalue(), dtype=np.uint8)
  dst = cv2.imdecode(enc, 1)
  dst = dst[:,:,::-1]
  dst = cv2.cvtColor(dst, cv2.COLOR_RGB2BGR)

#  plt.pause(.01)

  plt.close()

  return dst
  #plt.show()

def process_image(msg):
    global pub
    print("callback")
    start = now()
    try:
        bridge = CvBridge()
        orig = bridge.imgmsg_to_cv2(msg, "bgr8")
        img = cv2.cvtColor(orig, cv2.COLOR_BGR2GRAY)

        img = img_to_hist(img)

        imgMsg = bridge.cv2_to_imgmsg(img, "bgr8")
        pub.publish(imgMsg)
        print(now() - start)
    except Exception as err:
        print(err)

rospy.init_node('mono8_to_histgram')
rospy.loginfo('img_proc node started')
pub = rospy.Publisher('histgram', Image, queue_size=10)
rospy.Subscriber("image_raw", Image, process_image)
rospy.spin()
