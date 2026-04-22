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

void Processor::LoadImage(const Image& image)
{
	m_Image = image;
}

void Processor::Convert(Image& image, cv::ColorConversionCodes code)
{
	Image processingImage;
	cv::cvtColor(*image, *processingImage, code);
	image = processingImage;
}
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

Result Processor::Rect(const Setting& setting, double ROI[4])
{
	cv::Rect roi{static_cast<int>(ROI[0] * 2), static_cast<int>(ROI[1] * 2), static_cast<int>((ROI[2] - ROI[0]) * 2), static_cast<int>((ROI[3] - ROI[1]) * 2)};

	cv::Mat image;

	if (m_Image.GetImage().channels() == 1)
	{
		cv::cvtColor(m_Image.GetImage(), image, cv::COLOR_GRAY2BGR);
	}
	else if (m_Image.GetImage().channels() == 4)
	{
		cv::cvtColor(m_Image.GetImage(), image, cv::COLOR_RGBA2BGR);
	}
	else
	{
		image = m_Image.GetImage();
	}

	cv::Mat roiImage = image(roi);

	cv::Mat cannyImg;
	cv::Canny(roiImage, cannyImg, 80, 160, 3);

	// 图像膨胀
	int kernelParam = setting.GetData("Kernel") * 2 - 1;
	cv::Mat kernelImg{cv::getStructuringElement(0, cv::Size{kernelParam, kernelParam})};
	cv::dilate(cannyImg, cannyImg, kernelImg);

	// 外接矩形
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	std::vector<cv::Point2f> centers;
	cv::findContours(cannyImg, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point{});

	int minAreaParam = setting.GetData("AreaMin");
	int maxAreaParam = setting.GetData("AreaMax");
	for (int i = 0; i < contours.size(); ++i)
	{
		double area = cv::contourArea(contours[i]);
		if (area < minAreaParam || area > maxAreaParam)
		{
			contours.erase(contours.begin() + i);
			--i;
		}
	}

	if (contours.size() < setting.GetData("ContourMin") || contours.size() > setting.GetData("ContourMax"))
	{
		return {.image = m_Image, .origin = m_Result.front(), .current = m_Result.back(), .k = 0, .delta = 0, .contourCount = contours.size()};
	}
	// 画四条线
	Image img2 = m_Image;

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
		center.x += roi.x;
		center.y += roi.y;
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

	// 直线
	cv::Vec4f line;
	cv::fitLine(centers, line, cv::DIST_L1, 0.1, 0.01, 0.01);

	double k = line[1] / line[0];
	cv::Point2d point1{line[2], line[3]};
	cv::Point2d point2{line[2], line[3]};
	double deltaRoi = 0;
	// 存储结果
	if (m_Result.size() > 1)
	{
		deltaRoi = line[3] - m_Result.back();
		ROI[0] += deltaRoi * setting.GetData("ROIJustify") * 0.1;
		ROI[1] += deltaRoi * setting.GetData("ROIJustify") * 0.1;
		ROI[2] += deltaRoi * setting.GetData("ROIJustify") * 0.1;
		ROI[3] += deltaRoi * setting.GetData("ROIJustify") * 0.1;
	}
	m_Result.push_back(line[3]);

	int count = 0;

	while (point1.x <= m_Image.GetWidth() && point1.y <= m_Image.GetHeight() && point1.x >= 0 && point1.y >= 0 && count <= 1000)
	{
		point1.x += line[0];
		point1.y += line[1];
		++count;
	}
	count = 0;
	while (point2.x <= m_Image.GetWidth() && point2.y <= m_Image.GetHeight() && point2.x >= 0 && point2.y >= 0 && count <= 1000)
	{
		point2.x = point2.x - line[0];
		point2.y = point2.y - line[1];
		++count;
	}
	cv::line(*img2, point1, point2, cv::Scalar{0, 255, 0}, 2);
	return {.image = img2, .origin = m_Result.front(), .current = m_Result.back(), .k = k, .delta = deltaRoi, .contourCount = contours.size()};
}

Result Processor::Match(const Setting& setting, double ROI[4], double templateArea[4])
{
	cv::Rect roi{
		static_cast<int>(ROI[0] * 2),
		static_cast<int>(ROI[1] * 2),
		static_cast<int>((ROI[2] - ROI[0]) * 2),
		static_cast<int>((ROI[3] - ROI[1]) * 2)
	};
	cv::Rect templ{
		static_cast<int>(templateArea[0] * 2),
		static_cast<int>(templateArea[1] * 2),
		static_cast<int>((templateArea[2] - templateArea[0]) * 2),
		static_cast<int>((templateArea[3] - templateArea[1]) * 2)
	};

	cv::Mat image;

	if (m_Image.GetImage().channels() == 1)
	{
		cv::cvtColor(m_Image.GetImage(), image, cv::COLOR_GRAY2BGR);
	}
	else if (m_Image.GetImage().channels() == 4)
	{
		cv::cvtColor(m_Image.GetImage(), image, cv::COLOR_RGBA2BGR);
	}
	else
	{
		image = m_Image.GetImage();
	}

	Image resultImage = m_Image;
	cv::Mat roiImage = image(roi);
	cv::Mat templateImage = image(templ);
	cv::Mat result;
	cv::matchTemplate(roiImage, templateImage, result, 5);

	std::vector<cv::Point2f> centers;
	float threshold = setting.GetData("Threshold") * 0.01;
	for (int i = 0; i < result.cols; i++)
	{
		for (int j = 0; j < result.rows; j++)
		{
			int x1 = i + roi.x;
			int x2 = i + templ.width + roi.x;
			int y1 = j + roi.y;
			int y2 = j + templ.height + roi.y;
			if (result.at<float>(j, i) > threshold && result.at<float>(j, i) < 1.0f)
			{
				centers.push_back(cv::Point2f(i + roi.x + templ.width / 2, j + roi.y + templ.height / 2));
				cv::rectangle(*resultImage, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 0, 255), 1);
			}
		}
	}

// TODO: CSI，线阵，面阵；成本、性能
	// 直线
	cv::Vec4f line;
	cv::fitLine(centers, line, cv::DIST_L1, 0.1, 0.01, 0.01);

	double k = line[1] / line[0];
	cv::Point2d point1{line[2], line[3]};
	cv::Point2d point2{line[2], line[3]};
	double deltaRoi = 0;
	// 存储结果
	if (m_Result.size() > 1)
	{
		deltaRoi = line[3] - m_Result.back();
		ROI[0] += deltaRoi * setting.GetData("ROIJustifyParam") * 0.1;
		ROI[1] += deltaRoi * setting.GetData("ROIJustifyParam") * 0.1;
		ROI[2] += deltaRoi * setting.GetData("ROIJustify") * 0.1;
		ROI[3] += deltaRoi * setting.GetData("ROIJustify") * 0.1;
	}
	m_Result.push_back(line[3]);

	int count = 0;

	while (point1.x <= m_Image.GetWidth() && point1.y <= m_Image.GetHeight() && point1.x >= 0 && point1.y >= 0 && count <= 1000)
	{
		point1.x += line[0];
		point1.y += line[1];
		++count;
	}
	count = 0;
	while (point2.x <= m_Image.GetWidth() && point2.y <= m_Image.GetHeight() && point2.x >= 0 && point2.y >= 0 && count <= 1000)
	{
		point2.x = point2.x - line[0];
		point2.y = point2.y - line[1];
		++count;
	}
	cv::line(*resultImage, point1, point2, cv::Scalar{0, 255, 0}, 2);
	return {.image = resultImage, .origin = m_Result.front(), .current = m_Result.back(), .k = k, .delta = deltaRoi, .contourCount = centers.size()};
	// return {.image = resultImage, .origin = 0, .current = 0, .k = 0, .delta = 0, .contourCount = 0};
}

void Processor::ClearResult()
{
	m_Result.clear();
}