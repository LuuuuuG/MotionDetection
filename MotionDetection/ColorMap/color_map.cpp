/*
Thu Oct 26 17:57:33 CST 2017
使用颜色分布法计算图像相似度

基于比较灰度直方图的方法计算两幅图像相似度误判较多，
一个原因是直方图自身局限性：
仅反映图像像素各灰度值的数量，
不能反映图像纹理结构；
另一个原因就是在使用该方法时，
对于彩色图像，
一般都是将其转为灰度图像，
然后在计算其灰度直方图，
最后再参与运算比较，
很明显在彩色转灰度的转换过程中将损失图像颜色信息，
所以在计算时存在大量误判。
由第一个原因产生的误判很难找到解决方案，
除非不用这个方法；
而第二原因，
对于彩色图像，
我们可以换一种方式计算直方图，
阮一峰的博客《相似图片搜索的原理（二）》提到了这种彩色图像直方图计算方法，
这里不再赘述。
通过实验发现，这种方式很好的克服了第二个原因产生的误判，
相似度计算结果准确性有所提高。
下面是算法实现，
程序中将颜色分成8个区间做映射，
如果分成16个区间或更高的区间做映射，
相信准确度会更高。
当然，该算法对图像的明暗还是特别敏感，
不太适用内容相近、但光线明暗频繁变化的图像。
*/
#include <iostream>                                                      
#include <opencv2/opencv.hpp>
#include <math.h>
using namespace std;
using namespace cv;

// bgr颜色分区间映射
uchar ColorValMapping(uchar &val)
{
	uchar mapVal = 0;
	if (val > 223)
	{
		// [224 ~ 255]
		mapVal = 7;
	}
	else if (val > 191)
	{
		// [192 ~ 223]
		mapVal = 6;
	}
	else if (val > 159)
	{
		// [160 ~ 191]
		mapVal = 5;
	}
	else if (val > 127)
	{
		// [128 ~ 159]
		mapVal = 4;
	}
	else if (val > 95)
	{
		// [96 ~ 127]
		mapVal = 3;
	}
	else if (val > 63)
	{
		// [64 ~ 95]
		mapVal = 2;
	}
	else if (val > 31)
	{
		// [32 ~ 63]
		mapVal = 1;
	}
	else
	{
		// [0 ~ 31]
		mapVal = 0;
	}

	return mapVal;
}

// 计算颜色直方图向量
void CompImageFeature(Mat &inputImg, int *pImgFeature)
{
	int index = 0;
	int row = inputImg.rows;
	int col = inputImg.cols;
	uchar map_b = 0, map_g = 0, map_r = 0;
	uchar* pImg = inputImg.data;

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			// 颜色映射
			map_b = ColorValMapping(pImg[3 * j]);
			map_g = ColorValMapping(pImg[3 * j + 1]);
			map_r = ColorValMapping(pImg[3 * j + 2]);
			index = map_b * 64 + map_g * 8 + map_r;
			pImgFeature[index]++;
		}

		pImg += inputImg.step;
	}
}

// 计算两幅图像相似度
double CompImageSimilarity(Mat &img0, Mat &img1)
{
	// 计算颜色直方图向量>>分8个区间,共8*8*8 = 512种组合
	int imgFeature0[512] = { 0 };
	int imgFeature1[512] = { 0 };
	CompImageFeature(img0, imgFeature0);
	CompImageFeature(img1, imgFeature1);

	// 计算余弦相似度>>余弦值越接近1,表明夹角越接近0度,两个向量越相似
	double sum_square0 = 0.0, sum_square1 = 0.0, sum_multiply = 0.0;
	for (int i = 0; i < 512; i++)
	{
		sum_square0 += imgFeature0[i] * imgFeature0[i];
		sum_square1 += imgFeature1[i] * imgFeature1[i];
		sum_multiply += imgFeature0[i] * imgFeature1[i];
	}

	return sum_multiply / (sqrt(sum_square0) * sqrt(sum_square1));
}

int main(int argc, char **argv)
{
	double similarity = 0.0;

	// 计算两幅图像相似度
	//Mat img0 = imread(argv[1]);
	//Mat img1 = imread(argv[2]);

	Mat img0 = imread("1.bmp");
	Mat img1 = imread("2.bmp");
	Mat img2 = imread("9.bmp");

	//720P
	//Mat img0 = imread("12.jpg");
	//Mat img1 = imread("13.jpg");
	//Mat img2 = imread("14.jpg");

	//1080P
	//Mat img0 = imread("1.jpeg");
	//Mat img1 = imread("2.jpeg");
	//Mat img2 = imread("3.jpeg");
	
	cout << " 【测试方法二：颜色分布法(直方图改进版)】 " << endl ;
	double start = static_cast<double>(getTickCount());

	similarity = CompImageSimilarity(img0, img1);
	printf("pic1 pic2-------相似度-------> %f \n", similarity);

	double start2 = static_cast<double>(getTickCount());
	cout << "pic1 pic2-------耗时 -------> " << (start2 - start) / getTickFrequency() * 1000 << " ms" << endl;
	cout << endl;
	similarity = CompImageSimilarity(img0, img2);
	printf("pic1 pic3-------相似度-------> %f \n", similarity);

	double end = ((double)getTickCount() - start2) / getTickFrequency() * 1000;
	cout << "pic1 pic3-------耗时-------> " << end << " ms" << endl;

	getchar();
	return 0;
}
