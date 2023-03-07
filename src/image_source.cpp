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
#include "./cpp_ros_utils/psp.h"

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
  ros::Publisher io_set_pub_;
  image_transport::Publisher image_pub_;
  image_transport::Publisher last_image_pub_;
  bool pub_;
public:
  ImageConverter()
    : it_(nh_)
  {

    // Subscrive to input video feed and publish output video feed
    image_pub_ = it_.advertise("/usb_cam/image_raw", 1);

    last_image_pub_ = it_.advertise("/usb_cam/last_image_raw", 1);

    capture_sub_ = nh_.subscribe("/image_source/capture", 1000, &ImageConverter::captureCb, this);
    capture_io_sub_ = nh_.subscribe("/hoshina_io/change_7", 1000, &ImageConverter::capture_io_Cb, this);
    fname_sub_ = nh_.subscribe("/image_source/file_name", 1000, &ImageConverter::fnameCb, this);
    io_set_pub_ = nh_.advertise<std_msgs::String>("hoshina_io/set", 1000);

    PSP_num capture_loop_psp = PSP_num(&nh_, "image_source/capture_loop");

    std::vector<std::string> source_list = {"video_device","image"};
    PSP_mode source_select_psp = PSP_mode(&nh_, "image_source/source_select", source_list);

    ros::Rate loop_rate(100);

    pub_ = false;

    cv::Mat pub_image;
    cv::Mat image;

    std::string source_state;
    
    while (ros::ok())
    {

      source_state = source_select_psp.get_value();

      if (source_state == source_list[0])
      {
        if (!camera.isOpened())
        {
          //camera.open("/dev/video2");
          camera.open("/dev/video0");
        } 
        else 
        {
          camera >> image;
        }
      }
      else if (source_state == source_list[1])
      {

      }

      if (pub_==true || capture_loop_psp.get_value()!=0)
      {
        pub_image = image;
        imagePub(pub_image);
        std_msgs::String msg;
        msg.data = "12,0";
        io_set_pub_.publish(msg);
        msg.data = "13,0";
        io_set_pub_.publish(msg);
        pub_ = false;
      }

      lastImagePub(pub_image);

      loop_rate.sleep();
      ros::spinOnce();

    }

  }
  ~ImageConverter()
  {
  }

  void lastImagePub(cv::Mat image)
  {
    cv_bridge::CvImagePtr cv_ptr;

    cv_ptr = cv_bridge::CvImagePtr(new cv_bridge::CvImage(std_msgs::Header(), "bgr8", image));

    last_image_pub_.publish(cv_ptr->toImageMsg());
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
      pub_ = true;
    }
  }
  void captureCb(const std_msgs::EmptyConstPtr& msg)
  {
    pub_ = true;
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
