/*=================================================================
 * Calculate Background Model of a list of Frames(Normally a video stream) in the
 * method of Background Difference Method & OTSU Algorithm By OpenCV.
 *
 * Copyright (C) 2017 Chandler Geng. All rights reserved.
 *
 *     This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 *     This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 *     You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc., 59
 * Temple Place, Suite 330, Boston, MA 02111-1307 USA
 ===================================================================
 */

/*=================================================
 * Version:
 * v1.0: 原版程序由IplImage转换为Mat；
 * v1.1: 背景差分法封装成类: BGDiff；
 * v1.2: 补充注释；
 * v1.3: 该方法与高斯混合背景模型不同，命名有误，改为背景差分法；
 ===================================================
 */

#include "BGDifference.h"
#include "opencv2/opencv.hpp"

BGDiff BGDif;

// 原图像
Mat pFrame;
// 原始OTSU算法输出图像
Mat pFroundImg;
// 背景图像
Mat pBackgroundImg;
// 改进的OTSU算法输出图像
Mat pFroundImg_c;
Mat pBackgroundImg_c;

int nFrmNum = 0;

void releaseFrame()
{
	pFrame.release();
	pFroundImg.release();
	pBackgroundImg.release();
	pFroundImg_c.release();
	pBackgroundImg_c.release();
	nFrmNum = 0;
}
int otsu(string filename)
{
	//// 原图像
	//Mat pFrame;
	//// 原始OTSU算法输出图像
	//Mat pFroundImg;
	//// 背景图像
	//Mat pBackgroundImg;
	//// 改进的OTSU算法输出图像
	//Mat pFroundImg_c;
	//Mat pBackgroundImg_c;

	//视频控制全局变量,
	// 's' 画面stop
	// 'q' 退出播放
	// 'p' 打印OTSU算法中找到的阈值
	char ctrl = NULL;

	//BGDiff BGDif;

	//for video 
	//VideoCapture capture;
	//string inputPath = "D:\\videos\\Record\\20170808_A2.avi";
	//capture = VideoCapture(inputPath);
	//if (!capture.isOpened())
	//{
	//	capture = VideoCapture(inputPath);
	//	if (!capture.isOpened())
	//	{
	//		capture = VideoCapture(inputPath);
	//		if (!capture.isOpened())
	//		{
	//			cout << "ERROR: Did't find this video!" << endl;
	//			return 0;
	//		}
	//	}
	//}

	//for picture
	pFrame = imread(filename);
	double time1, time2;
	double start, start2;
	

	//// 逐帧读取视频
	//capture >> pFrame;
	//while (!pFrame.empty())
	//{
	//	capture >> pFrame;
		nFrmNum++;
		// 图像尺寸缩小
		cv::resize(pFrame, pFrame, cv::Size(), 0.5, 0.5);
		//// 视频控制
		//if ((ctrl = cvWaitKey(1000 / 25)) == 's')
		//	waitKey(0);
		//else if (ctrl == 'p')
		//	cout << "Current Frame = " << nFrmNum << endl;
		//else if (ctrl == 'q')
		//	break;

		start = static_cast<double>(getTickCount());

		// OpenCV自带OTSU
		BGDif.BackgroundDiff(pFrame, pFroundImg, pBackgroundImg, nFrmNum, CV_THRESH_OTSU);
		time1 = ((double)getTickCount() - start) / getTickFrequency() * 1000;

		start2 = static_cast<double>(getTickCount());
		// 阈值筛选后的OTSU
		BGDif.BackgroundDiff(pFrame, pFroundImg_c, pBackgroundImg_c, nFrmNum, CV_THRESH_BINARY);
		time2 = ((double)getTickCount() - start2) / getTickFrequency() * 1000;

		//cout << endl;
		cout << "Time of Update OTSU Background: " << time1 << "ms" << endl;
		cout << "Time of Update Advanced OTSU Background: " << time2 << "ms" << endl;
		cout << endl;

		// 显示图像
		//imshow("Source Video", pFrame);
		//imshow("Background", pBackgroundImg);
		//imshow("OTSU ForeGround", pFroundImg);

		//imshow("Background", pBackgroundImg_c);
		//imshow("Advanced OTSU ForeGround", pFroundImg_c);

		if (waitKey(25) == 27)
			return -1;
	//}
		return 0;
}

int main()
{
	cout << "img_576 : " << endl;
	otsu("../3rdParty/pics/img_576/1.png");
	otsu("../3rdParty/pics/img_576/2.png");
	otsu("../3rdParty/pics/img_576/3.png");
	releaseFrame();
	
	cout << "img_720 : " << endl;
	otsu("../3rdParty/pics/img_720/1.jpg");
	otsu("../3rdParty/pics/img_720/2.jpg");
	otsu("../3rdParty/pics/img_720/3.jpg");
	releaseFrame();

	cout << "img_1080 : " << endl;
	otsu("../3rdParty/pics/img_1080/1.jpeg");
	otsu("../3rdParty/pics/img_1080/2.jpeg");
	otsu("../3rdParty/pics/img_1080/3.jpeg");
	releaseFrame();

	getchar();
	return 0;
}

int main2(int argc, char** argv)
{
	// 原图像
	Mat pFrame;
	// 原始OTSU算法输出图像
	Mat pFroundImg;
	// 背景图像
	Mat pBackgroundImg;
	// 改进的OTSU算法输出图像
	Mat pFroundImg_c;
	Mat pBackgroundImg_c;

	//视频控制全局变量,
	// 's' 画面stop
	// 'q' 退出播放
	// 'p' 打印OTSU算法中找到的阈值
	char ctrl = NULL;

	BGDiff BGDif;

	//for video 
	VideoCapture capture;
	string inputPath = "D:\\videos\\Record\\20170808_A2.avi";
	capture = VideoCapture(inputPath);
	if (!capture.isOpened())
	{
		capture = VideoCapture(inputPath);
		if (!capture.isOpened())
		{
			capture = VideoCapture(inputPath);
			if (!capture.isOpened())
			{
				cout << "ERROR: Did't find this video!" << endl;
				return 0;
			}
		}
	}

	double time1,time2;
	double start;
	int nFrmNum = 0;

	// 逐帧读取视频
	capture >> pFrame;
	while (!pFrame.empty())
	{
		capture >> pFrame;
		nFrmNum++;
		// 图像尺寸缩小
		cv::resize(pFrame, pFrame, cv::Size(), 0.5, 0.5);
		// 视频控制
		if ((ctrl = cvWaitKey(1000 / 25)) == 's')
			waitKey(0);
		else if (ctrl == 'p')
			cout << "Current Frame = " << nFrmNum << endl;
		else if (ctrl == 'q')
			break;

		start = static_cast<double>(getTickCount());

		// OpenCV自带OTSU
		BGDif.BackgroundDiff(pFrame, pFroundImg, pBackgroundImg, nFrmNum, CV_THRESH_OTSU);
		time1 = ((double)getTickCount() - start) / getTickFrequency() * 1000;

		// 阈值筛选后的OTSU
		BGDif.BackgroundDiff(pFrame, pFroundImg_c, pBackgroundImg_c, nFrmNum, CV_THRESH_BINARY);
		time2 = ((double)getTickCount() - start) / getTickFrequency() * 1000;

		cout << endl;
		cout << "Time of Update OTSU Background: " << time1 << "ms" << endl;
		cout << "Time of Update Advanced OTSU Background: " << time2 << "ms" << endl;
		cout << endl;

		// 显示图像
		//imshow("Source Video", pFrame);
		//imshow("Background", pBackgroundImg);
		imshow("OTSU ForeGround", pFroundImg);

		//imshow("Background", pBackgroundImg_c);
		imshow("Advanced OTSU ForeGround", pFroundImg_c);

		if (waitKey(25) == 27)
			break;
	}
	return 0;
}

