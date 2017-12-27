#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/opencv.hpp>  
#include <iostream>
#include <sstream>


using namespace cv;
using namespace std;
Mat frame;
Mat fgMaskMOG2;
Ptr<BackgroundSubtractor> pMOG2;
int keyboard;

int GMM(string videoFilename)
{
	//std::string videoFile = "../test.avi";

	cv::VideoCapture capture(videoFilename);
	//capture.open(videoFile);

	if (!capture.isOpened())
	{
		std::cout << "read video failure" << std::endl;
		return -1;
	}


	//cv::BackgroundSubtractorMOG2 mog;

	cv::Mat foreground;
	cv::Mat background;

	cv::Mat frame;

	double time = 0;
	double start;


	long frameNo = 0;
	while (capture.read(frame))
	{
		++frameNo;

		std::cout << frameNo << std::endl;

		start = static_cast<double>(getTickCount());

		// 图像尺寸缩小
		cv::resize(frame, frame, cv::Size(), 0.5, 0.5);

		// 运动前景检测，并更新背景  
		pMOG2->apply(frame, foreground/*, 0.001*/);

		// 腐蚀  
		cv::erode(foreground, foreground, cv::Mat());

		// 膨胀  
		cv::dilate(foreground, foreground, cv::Mat());

		pMOG2->getBackgroundImage(background);   // 返回当前背景图像  

		time += ((double)getTickCount() - start) / getTickFrequency() * 1000;
		cout << "Time of Update GMM Background: " << time / frameNo << "ms" << endl;


		cv::imshow("video", foreground);
		cv::imshow("background", frame);


		if (cv::waitKey(25) > 0)
		{
			break;
		}
	}

}

void processVideo(string videoFilename)
{
	// 视频获取
	VideoCapture capture(videoFilename);
	if (!capture.isOpened())
	{
		// 输出视频文件打开错误信息
		cerr << "Unable to open video file: " << videoFilename << endl;
		exit(EXIT_FAILURE);
	}
	double time = 0;
	double start;

	// 按下q键和esc退出
	while ((char)keyboard != 'q' && (char)keyboard != 27)
	{
		// 读取当前帧
		if (!capture.read(frame))
		{
			cerr << "Unable to read next frame." << endl;
			cerr << "Exiting..." << endl;
			exit(EXIT_FAILURE);
		}

		start = static_cast<double>(getTickCount());

		// 图像尺寸缩小
		cv::resize(frame, frame, cv::Size(), 0.5, 0.5);
		//  背景模型生成
		pMOG2->apply(frame, fgMaskMOG2);
		
		time = ((double)getTickCount() - start) / getTickFrequency() * 1000;
		cout << "Time of Update BGDiff Background: " << time << "ms" << endl;

		// 输出当前帧号
		stringstream ss;
		rectangle(frame, cv::Point(10, 2), cv::Point(100, 20),
			cv::Scalar(255, 255, 255), -1);
		ss << capture.get(CAP_PROP_POS_FRAMES);
		string frameNumberString = ss.str();
		// 左上角显示帧号
		putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
			FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
		// 输出结果
		imshow("Frame", frame);
		imshow("FG Mask MOG 2", fgMaskMOG2);
		keyboard = waitKey(30);
	}
	capture.release();
}
int main(int argc, char* argv[])
{
	// 创建背景建模类
	pMOG2 = createBackgroundSubtractorMOG2();
	string inputPath = "D:\\videos\\Record\\20170808_A2.avi";
	//processVideo(inputPath);
	GMM(inputPath);
	return 0;
}