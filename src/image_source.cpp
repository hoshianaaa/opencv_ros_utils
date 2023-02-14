#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "std_msgs/String.h"
#include "std_msgs/Empty.h"

cv::VideoCapture camera;

void callback(const std_msgs::Empty::ConstPtr& msg)
{
}

int main(int argc, char** argv) {
  ros::init (argc, argv, "image_source");
  ros::NodeHandle nh("~"); 
  image_transport::ImageTransport it(nh);
  image_transport::Publisher image_pub = it.advertise("image", 10);
  ros::Subscriber sub = nh.subscribe("capture", 1000, callback);
  cv::Mat image;
  camera.open("/dev/video0");
  if (!camera.isOpened()) {
    ROS_INFO("failed to open camera.");
    return -1;
  }
  ros::Rate looprate (10);
  while(ros::ok()) {
    camera >> image;
    sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", image).toImageMsg();
    image_pub.publish(msg);
    ros::spinOnce();
    looprate.sleep();
  }
  return 0;
}
