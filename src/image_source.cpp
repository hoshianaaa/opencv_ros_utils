#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "std_msgs/String.h"
#include "std_msgs/Empty.h"
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace enc = sensor_msgs::image_encodings;
using namespace cv;
using namespace std;

cv::VideoCapture camera;

class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  ros::Subscriber capture_sub_;
  image_transport::Publisher image_pub_;
public:
  ImageConverter()
    : it_(nh_)
  {
    // Subscrive to input video feed and publish output video feed
    image_pub_ = it_.advertise("/image_source/output_image", 1);
    capture_sub_ = nh_.subscribe("/image_source/capture", 1000, &ImageConverter::captureCb, this);

    camera.open("/dev/video0");
    if (!camera.isOpened()) {
      ROS_INFO("failed to open camera.");
      return;
    }

  }
  ~ImageConverter()
  {
  }
  void captureCb(const std_msgs::EmptyConstPtr& msg)
  {
    cv::Mat image;
    camera >> image;
    cv_bridge::CvImagePtr cv_ptr;

    cv_ptr = cv_bridge::CvImagePtr(new cv_bridge::CvImage(std_msgs::Header(), "bgr8", image));

    image_pub_.publish(cv_ptr->toImageMsg());
  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  ros::spin();
  return 0;
}
