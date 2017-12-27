/*
Thu Sep 28 14:35:08 CST 2017
利用感知哈希算法,
来计算两张图片的相似度。
*/
#include <opencv2/opencv.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

#define IMG01	argv[1]	
#define IMG02	argv[2]

int PerHash(string ImageName1, string ImageName2) {

	cv::Mat matSrc1, matSrc2;

	matSrc1 = cv::imread(ImageName1, CV_LOAD_IMAGE_COLOR);
	matSrc2 = cv::imread(ImageName2, CV_LOAD_IMAGE_COLOR);

	//1.缩小尺寸
	//将图片缩小到8x8的尺寸，总共64个像素。
	//这一步的作用是去除图片的细节，只保留结构、明暗等基本信息，摒弃不同尺寸、比例带来的图片差异。
	cv::Mat matDst1, matDst2;

	cv::resize(matSrc1, matDst1, cv::Size(8, 8), 0, 0, cv::INTER_CUBIC);
	cv::resize(matSrc2, matDst2, cv::Size(8, 8), 0, 0, cv::INTER_CUBIC);

	//2.简化色彩
	//将缩小后的图片，转为64级灰度。也就是说，所有像素点总共只有64种颜色。
	cv::cvtColor(matDst1, matDst1, CV_BGR2GRAY);
	cv::cvtColor(matDst2, matDst2, CV_BGR2GRAY);

	//3.计算平均值
	//计算所有64个像素的灰度平均值。
	int iAvg1 = 0, iAvg2 = 0;
	int arr1[64], arr2[64];

	for (int i = 0; i < 8; i++) {
		uchar* data1 = matDst1.ptr<uchar>(i);
		uchar* data2 = matDst2.ptr<uchar>(i);

		int tmp = i * 8;

		for (int j = 0; j < 8; j++) {
			int tmp1 = tmp + j;

			arr1[tmp1] = data1[j] / 4 * 4;
			arr2[tmp1] = data2[j] / 4 * 4;

			iAvg1 += arr1[tmp1];
			iAvg2 += arr2[tmp1];
		}
	}

	iAvg1 /= 64;
	iAvg2 /= 64;

	//4.比较像素的灰度
	//将每个像素的灰度，与平均值进行比较。大于或等于平均值，记为1；小于平均值，记为0。
	for (int i = 0; i < 64; i++) {
		arr1[i] = (arr1[i] >= iAvg1) ? 1 : 0;
		arr2[i] = (arr2[i] >= iAvg2) ? 1 : 0;
	}

	//5.计算哈希值
	//将上一步的比较结果，组合在一起，就构成了一个64位的整数，这就是这张图片的指纹。
	//组合的次序并不重要，只要保证所有图片都采用同样次序就行了。
	int iDiffNum = 0;

	for (int i = 0; i < 64; i++)
	if (arr1[i] != arr2[i])
		++iDiffNum;

	return iDiffNum;
}

void distance(int iDiffNum)
{
	//得到指纹以后，就可以对比不同的图片，看看64位中有多少位是不一样的。
	//在理论上，这等同于计算"汉明距离"（Hamming distance）。
	//如果不相同的数据位不超过5，就说明两张图片很相似；如果大于10，就说明这是两张不同的图片。
	if (iDiffNum <= 5)
		cout << "two images are very similar!" << endl;
	else if (iDiffNum > 10)
		cout << "they are two different images!" << endl;
	else
		cout << "two image are somewhat similar!" << endl;
}

int main(int argc, char **argv)
{
	cout << " 【测试方法三：感知哈希算法】 " << endl;

	//string ImageName1 = IMG01;
	//string ImageName2 = IMG02;

	//string ImageName1 = "1.bmp";
	//string ImageName2 = "2.bmp";
	//string ImageName3 = "9.bmp";

	//720
	//string ImageName1 = "12.jpg";
	//string ImageName2 = "13.jpg";
	//string ImageName3 = "14.jpg";

	//1080
	string ImageName1 = "1.jpeg";
	string ImageName2 = "2.jpeg";
	string ImageName3 = "3.jpeg";

	double start = static_cast<double>(getTickCount());
	int iDiffNum1 = PerHash(ImageName1, ImageName2);
	double start2 = static_cast<double>(getTickCount());

	cout << "iDiffNum = " << iDiffNum1 << endl;
	distance(iDiffNum1);
	cout << "pic1 pic2-------耗时 -------> " << (start2 - start) / getTickFrequency() * 1000 << " ms" << endl;

	cout << endl;
	
	int iDiffNum2 = PerHash(ImageName1, ImageName3);
	double end = ((double)getTickCount() - start2) / getTickFrequency() * 1000;

	cout << "iDiffNum = " << iDiffNum2 << endl;
	distance(iDiffNum2);
	cout << "pic1 pic3-------耗时-------> " << end << " ms" << endl;

	getchar();
	return 0;
}
