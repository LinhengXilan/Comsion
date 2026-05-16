#include <Core/pch.h>
#include <Core/Processor.h>

void ShowImage(const std::string& winName, const cv::Mat& sourceImage)
{
	cv::Mat image;
	cv::resize(sourceImage, image, cv::Size(sourceImage.cols / 2, sourceImage.rows / 2));
	cv::imshow(winName, image);
}

float GetDistance(const cv::Point2f& point1, const cv::Point2f& point2)
{
	return sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2));
}

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

Result Processor::Rect(const Setting& setting, double ROI[4])
{
	cv::Rect roi{static_cast<int>(ROI[0]), static_cast<int>(ROI[1]), static_cast<int>(ROI[2] - ROI[0]), static_cast<int>(ROI[3] - ROI[1])};

	cv::Mat image = m_Image.GetImage().clone();
	cv::Mat resultImage = m_Image.GetImage().clone();

	cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);

	image = image(roi);

#ifdef COMSION_DEBUG
	cv::rectangle(resultImage, roi, cv::Scalar{0., 255., 0.}, 2);
#endif

	//cv::GaussianBlur(image, image, cv::Size(5, 5), 0);
	cv::threshold(image, image, 250., 255., cv::THRESH_BINARY_INV);

	cv::Canny(image, image, 100, 180);

	// 图像膨胀
	int kernelParam = setting.GetData("Kernel") * 2 - 1;
	cv::Mat kernelImg{cv::getStructuringElement(cv::MORPH_RECT, cv::Size{kernelParam, kernelParam})};
	cv::dilate(image, image, kernelImg);

	// 外接矩形
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	std::vector<cv::Point2f> centers;

	cv::findContours(image, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point{});

	for (auto& contour : contours)
	{
		for (auto& point :  contour)
		{
			point.x += roi.x;
			point.y += roi.y;
		}
	}

	int minAreaParam = setting.GetData("AreaMin");
	int maxAreaParam = setting.GetData("AreaMax");
	for (int i = 0; i < contours.size(); ++i)
	{
		if (double area = cv::contourArea(contours[i]); area < minAreaParam || area > maxAreaParam)
		{
			contours.erase(contours.begin() + i);
			--i;
		}
	}

	cv::drawContours(resultImage, contours, -1, cv::Scalar(255, 0, 0), 2);


	// 画四条线
	Image img2 = m_Image;

	for (const auto& contour : contours)
	{
		cv::RotatedRect rotatedRect{cv::minAreaRect(contour)};

		cv::Point2f points[4];
		rotatedRect.points(points);
		cv::Point2f center{rotatedRect.center};

		bool flag = false;
		if (!centers.empty())
		{
			for (const auto& centerInVec : centers)
			{
				if (GetDistance(centerInVec, center) < 5.f)
				{
					flag = true;
				}
			}
		}
		if (flag)
		{
			continue;
		}


		centers.push_back(center);

		for (int j = 0; j < 4; ++j)
		{
			if (j == 3)
			{
				cv::line(resultImage, points[j], points[0], cv::Scalar{0, 0, 255}, 2);
				break;
			}
			cv::line(resultImage, points[j], points[j + 1], cv::Scalar{0, 0, 255}, 2);
		}
		cv::circle(resultImage, center, 4, cv::Scalar{0, 0, 255}, -1);
	}

	if (contours.size() < setting.GetData("ContourMin"))
	{
		return {.image = m_Image, .origin = 0., .current = 0., .k = 0, .delta = 0, .contourCount = static_cast<int>(contours.size())};
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
	cv::line(resultImage, point1, point2, cv::Scalar{0, 255, 0}, 2);

	#ifdef COMSION_DEBUG
	ShowImage("Rect", resultImage);
	#endif






	return {.image = resultImage, .origin = m_Result.front(), .current = m_Result.back(), .k = k, .delta = deltaRoi, .contourCount = contours.size()};
}

Result Processor::Match(const Setting& setting, double ROI[4], double templateArea[4])
{
	cv::Rect roi{
		static_cast<int>(ROI[0]),
		static_cast<int>(ROI[1]),
		static_cast<int>(ROI[2] - ROI[0]),
		static_cast<int>(ROI[3] - ROI[1])
	};

	cv::Rect templ{
		static_cast<int>(templateArea[0]),
		static_cast<int>(templateArea[1]),
		static_cast<int>(templateArea[2] - templateArea[0]),
		static_cast<int>(templateArea[3] - templateArea[1])
	};

	cv::Mat image = m_Image.GetImage().clone();
	cv::Mat resultImage = m_Image.GetImage().clone();

	cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
	cv::Mat templateImage = image.clone()(templ);

	image = image(roi);


	cv::GaussianBlur(image, image, cv::Size(5, 5), 0);

	cv::Mat result;
	cv::matchTemplate(image, templateImage, result, cv::TM_CCOEFF_NORMED);

	std::vector<cv::Point> points;
	float threshold = setting.GetData("Threshold") * 0.01;
	for (int i = 0; i < result.rows; ++i)
	{
		for (int j = 0; j < result.cols; ++j)
		{
			if (result.at<float>(i, j) > threshold)
			{
				points.emplace_back(j + roi.x, i + roi.y);
			}
		}
	}

	std::vector<cv::Point> centers;
	for (int i = 0; i < points.size(); ++i)
	{
		for (int j = 0; j < i; ++j)
		{
			if (GetDistance(points[i], points[j]) < 5.f)
			{
				points.erase(points.begin() + i);
				--i;
				break;
			}
		}

		cv::rectangle(
			resultImage,
			cv::Point(points[i].x, points[i].y),
			cv::Point(points[i].x + templateImage.cols, points[i].y + templateImage.rows),
			cv::Scalar{255., 0., 0.},
			2
		);
		centers.emplace_back(points[i].x + templateImage.cols / 2, points[i].y + templateImage.rows / 2);
	}


	if (centers.size() < setting.GetData("ContourMin"))
	{
		return {.image = m_Image, .origin = 0., .current = 0., .k = 0, .delta = 0, .contourCount = static_cast<int>(centers.size())};
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
	cv::line(resultImage, point1, point2, cv::Scalar{0, 255, 0}, 2);


	#ifdef COMSION_DEBUG
	ShowImage("Match", resultImage);
	#endif


	return {.image = resultImage, .origin = m_Result.front(), .current = m_Result.back(), .k = k, .delta = deltaRoi, .contourCount = centers.size()};
	// return {.image = resultImage, .origin = 0, .current = 0, .k = 0, .delta = 0, .contourCount = 0};
}

void Processor::ClearResult()
{
	m_Result.clear();
}