#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "std_msgs/String.h"
#include "std_msgs/Empty.h"
#include "std_msgs/Bool.h"
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
  ros::Subscriber capture_sub_, capture_io_sub_, fname_sub_;
  image_transport::Publisher image_pub_;
public:
  ImageConverter()
    : it_(nh_)
  {
    // Subscrive to input video feed and publish output video feed
    image_pub_ = it_.advertise("/usb_cam/image_raw", 1);
    capture_sub_ = nh_.subscribe("/image_source/capture", 1000, &ImageConverter::captureCb, this);
    capture_io_sub_ = nh_.subscribe("/hoshina_io/change_7", 1000, &ImageConverter::capture_io_Cb, this);
    fname_sub_ = nh_.subscribe("/image_source/file_name", 1000, &ImageConverter::fnameCb, this);

    ros::Rate loop_rate(100);

    camera.open("/dev/video0");
    if (!camera.isOpened()) {
          ROS_INFO("failed to open camera.");
      return;
    }
    
    while (ros::ok())
    {

      cv::Mat image;
      camera >> image;

      loop_rate.sleep();
      ros::spinOnce();

    }

  }
  ~ImageConverter()
  {
  }
  void imagePub(cv::Mat image)
  {
    cv_bridge::CvImagePtr cv_ptr;

    cv_ptr = cv_bridge::CvImagePtr(new cv_bridge::CvImage(std_msgs::Header(), "bgr8", image));

    image_pub_.publish(cv_ptr->toImageMsg());
  }

  void capture_io_Cb(const std_msgs::BoolConstPtr& msg)
  {
    if (msg->data == false)
    {
      cv::Mat image;
      camera >> image;
      imagePub(image);
    }
  }
  void captureCb(const std_msgs::EmptyConstPtr& msg)
  {
    cv::Mat image;
    camera >> image;
    imagePub(image);
  }
  void fnameCb(const std_msgs::StringConstPtr& msg)
  {
    cv::Mat image = cv::imread(msg->data, -1);
    imagePub(image);
  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  ros::spin();
  return 0;
}
