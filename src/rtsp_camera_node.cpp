




#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
 
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


int main(int argc, char** argv)
{
    ros::init(argc, argv, "rtsp_camera_node");
   
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    image_transport::Publisher pub = it.advertise("rtsp_camera/image", 1);
    nh.param("/rtsp_camera/image/compressed/jpeg_quality", 20); // asaf 19/07/21

    cv::VideoCapture cap("rtsp://192.168.0.203:554/video", cv::CAP_GSTREAMER);
    if(!cap.isOpened()) {
        std::cerr<<"Failed to open url"<<std::endl;
        return (-1);
    }

     cv::Mat img;

    ros::Rate rate(10); // ROS Rate at 5Hz
    while (ros::ok()) {
       
       if (!cap.read(img)) {
            std::cerr<<"Capture read error"<<std::endl;
            continue;
        }
        sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", img).toImageMsg();

        pub.publish(msg);

        rate.sleep();
    }
 
    cerr <<" error with camera "<<endl;
    cap.release();

 return 0;
}
