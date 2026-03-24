#include <Core/pch.h>
#include <Core/Processor.h>

Processor::Processor(const Image& image)
	: m_Image(image)
{

}

[[nodiscard]] Image Processor::GetOriginalImage() const
{
	return m_Image;
}

// Image& Processor::Process()
// {
// 	Image image{m_Image};
// 	Convert(image, cv::COLOR_BGR2GRAY);
// 	m_Image = image;
// 	/* 边缘检测 */
// 	// EqualizeHist();
// 	// Filter({ 3,3 }, 5);
// 	// EdgeDetection(EdgeDetectionMethod::Canny, 80, 180);
//
// 	// 模板匹配
// 	// MatchTemplate("Assets/1t.bmp");
// 	// ConvertScaleAbs(0.8f, 0.0f);
// 	return m_Image;
// }

// void Processor::Convert(Image& image, cv::ColorConversionCodes code)
// {
// 	Image processingImage;
// 	cv::cvtColor(*image, *processingImage, code);
// 	image = processingImage;
// }
//
// void Processor::PixelHistogram(Image& image, bool isDrawHist)
// {
// 	cv::Mat histogram;
// 	constexpr int channels[1] = { 0 };
// 	constexpr int maxBin[1] = { 256 };
// 	float range[2] = { 0, 255 };
// 	const float* ranges[1] = { range };
// 	cv::calcHist(image.GetImagePointer(), 1, channels, cv::Mat(), histogram, 1, maxBin, ranges);
//
// 	cv::Mat normalizedHistogram;
// 	cv::normalize(histogram, normalizedHistogram, 1, 0, cv::NORM_INF, -1, cv::Mat());
// 	int windowWidth = 800;
// 	int windowHeight = 450;
// 	int width = 2;
// 	cv::Mat histImage(windowHeight, windowWidth, CV_8UC3, cv::Scalar(0, 0, 0));
// 	for (int i = 0; i < histogram.rows; i++)
// 	{
// 		cv::rectangle(
// 			histImage,
// 			cv::Point(
// 				width * i,
// 				windowHeight - 1
// 			),
// 			cv::Point(
// 				width * (i + 1) - 1,
// 				windowHeight - cvRound(normalizedHistogram.at<float>(i) * windowHeight)
// 			),
// 			cv::Scalar(255, 255, 255),
// 			-1
// 		);
// 	}
// 	// cv::imshow(m_Image.GetName() + " | Pixel Histogram", histImage);
// }
//
// void Processor::DrawRectangle(Image& image, const cv::Point& pointLU, const cv::Point& pointRD, const Color::RGB& color, int32_t thickness)
// {
// 	cv::rectangle(*image, pointLU, pointRD, cv::Scalar{ color.B, color.G, color.R }, thickness);
// }
//
// void Processor::DrawRectangle(Image& image, const cv::Point& point, const cv::Mat& mat, const Color::RGB& color, int32_t thickness)
// {
// 	DrawRectangle(image, point, cv::Point{ point.x + mat.cols, point.y + mat.rows }, color, thickness);
// }
//
// void Processor::MatchTemplate(Image& image, const Image& templateImage, cv::TemplateMatchModes flag, const Color::RGB& color)
// {
// 	if (templateImage.GetImage().empty())
// 	{
// 		std::cerr << "Template image is empty!" << std::endl;
// 		return;
// 	}
//
// 	cv::Mat result;
// 	cv::matchTemplate(image.GetImage(), templateImage.GetImage(), result, cv::TM_CCOEFF_NORMED);
// 	double minVal, maxVal;
// 	cv::Point minLoc, maxLoc;
// 	cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
// 	DrawRectangle(image, maxLoc, templateImage.GetImage(), color, 2);
// }
//
// void Processor::MatchTemplate(Image& image, const std::string& filepath, cv::TemplateMatchModes flag, const Color::RGB& color)
// {
// 	Image templateImage;
// 	templateImage.LoadImage(filepath);
// 	MatchTemplate(image, templateImage, flag, color);
// }
//
// void Processor::Filter(Image& image, const cv::Size& size, double sigma)
// {
// 	Image processingImage;
// 	cv::GaussianBlur(image.GetImage(), *processingImage, size, sigma, sigma);
// 	processingImage.CopyTo(image);
// }
//
// void Processor::EdgeDetection(Image& image, EdgeDetectionMethod method, int32_t arg0, int32_t arg1, int32_t arg2)
// {
// 	cv::Mat edgeX, edgeY;
// 	cv::Mat edge;
// 	switch (method)
// 	{
// 	case EdgeDetectionMethod::Sobel:
// 		cv::Sobel(image.GetImage(), edgeX, CV_32F, arg0, 0);
// 		cv::Sobel(image.GetImage(), edgeY, CV_32F, 0, arg0);
// 		image.SetImage(edgeX + edgeY);
// 		break;
// 	case EdgeDetectionMethod::Laplacian:
// 		cv::Laplacian(image.GetImage(), edge, CV_32F, arg0, arg1);
// 		image.SetImage(edge);
// 		break;
// 	case EdgeDetectionMethod::Canny:
// 		cv::Canny(image.GetImage(), edge, arg0, arg1);
// 		image.SetImage(edge);
// 		break;
// 	}
// }
//
// void Processor::EqualizeHist(Image& image)
// {
// 	Image processingImage;
// 	cv::equalizeHist(*image, *processingImage);
// 	processingImage.CopyTo(image);
// }
//
// void Processor::ConvertScaleAbs(Image& image, double alpha, double beta)
// {
// 	Image processingImage;
// 	cv::convertScaleAbs(image.GetImage(), *processingImage, alpha, beta);
// 	processingImage.CopyTo(image);
// }
//
// Image& Processor::Contours()
// {
// 	Convert(m_Image, cv::COLOR_BGR2GRAY);
// 	cv::Mat binaryImage;
// 	cv::threshold(*m_Image, binaryImage, 170, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
//
// 	std::vector<std::vector<cv::Point>> contours;
// 	std::vector<cv::Vec4i> hierarchy;
// 	cv::findContours(binaryImage, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point{});
// 	for (int i = 0; i < contours.size(); ++i)
// 	{
// 		cv::drawContours(*m_Image, contours, i, {0, 0, 255}, 5);
// 	}
// 	return m_Image;
// }

Image Processor::Rect(int ROI[4])
{
	cv::Rect roi{ROI[0] * 2, ROI[1] * 2, (ROI[2] - ROI[0]) * 2, (ROI[3] - ROI[1]) * 2};
#ifdef COMSION_DEBUG
	std::cerr << "ROI: " << roi << std::endl;
#endif
	cv::Mat image{m_Image.GetImage()};
	cv::Mat roiImage = image(roi);

	cv::Mat cannyImg;
	cv::Canny(roiImage, cannyImg, 80, 160, 3);
#ifdef COMSION_DEBUG
	cv::imshow("11", roiImage);
	cv::imshow("", cannyImg);
	cv::waitKey();
#endif
	// 图像膨胀
	// cv::Mat kernelImg{cv::getStructuringElement(0, cv::Size{5, 5})};
	// cv::dilate(cannyImg, cannyImg, kernelImg);

	// 外接矩形
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	std::vector<cv::Point2f> centers;
	cv::findContours(cannyImg, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point{});
	std::cout << "contours.size(): " << contours.size() << std::endl;
	for (int i = 0; i < contours.size(); ++i)
	{
		double area = cv::contourArea(contours[i]);
		if (area < 100 || area > 50000)
		{
			contours.erase(contours.begin() + i);
			--i;
		}
	}
	std::cout << "contours.size(): " << contours.size() << std::endl;

	// 画四条线
	Image img2 = m_Image;;

	for (int i = 0; i < contours.size(); ++i)
	{
		cv::RotatedRect rotatedRect{cv::minAreaRect(contours[i])};
		cv::Point2f points[4];
		rotatedRect.points(points);
		for (int j = 0; j < 4; ++j)
		{
			points[j].x += roi.x;
			points[j].y += roi.y;
		}
		cv::Point2f center{rotatedRect.center};
		centers.push_back(center);
		for (int j = 0; j < 4; ++j)
		{
			if (j == 3)
			{
				cv::line(*img2, points[j], points[0], cv::Scalar{0, 0, 255}, 2);
				break;
			}
			cv::line(*img2, points[j], points[j + 1], cv::Scalar{0, 0, 255}, 2);
		}
		cv::circle(*img2, center, 4, cv::Scalar{0, 0, 255}, -1);
	}

	cv::Vec4f line;
	cv::fitLine(centers, line, cv::DIST_L1, 0, 0.01, 0.01);

	std::cout << line << std::endl;

	double k = line[1] / line[0];
	cv::Point2d point1{line[2] + roi.x, line[3] + roi.y};
	cv::Point2d point2{line[2] + roi.x, line[3] + roi.y};
	while (point1.x <= 1292 || point1.y <= 964)
	{
		point1.x += line[0];
		point1.y += line[1];

	}
	while (point2.x >= 0 || point2.y >= 0)
	{
		point2.x = point2.x - line[0];
		point2.y = point2.y - line[1];
	}
	cv::line(*img2, point1, point2, cv::Scalar{0, 255, 0}, 2);
	return img2;
}