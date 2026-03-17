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

Image& Processor::Process()
{
	Image image{m_Image};
	Convert(image, cv::COLOR_BGR2GRAY);
	m_Image = image;
	/* 边缘检测 */
	// EqualizeHist();
	// Filter({ 3,3 }, 5);
	// MatchTemplate("Assets/1t.bmp");
	// ConvertScaleAbs(0.8f, 0.0f);
	// EdgeDetection(EdgeDetectionMethod::Canny, 80, 180);
	return m_Image;
}

void Processor::Convert(Image& image, cv::ColorConversionCodes code)
{
	Image processingImage;
	cv::cvtColor(*image, *processingImage, code);
	image = processingImage;
}

void Processor::PixelHistogram(Image& image, bool isDrawHist)
{
	cv::Mat histogram;
	constexpr int channels[1] = { 0 };
	constexpr int maxBin[1] = { 256 };
	float range[2] = { 0, 255 };
	const float* ranges[1] = { range };
	cv::calcHist(image.GetImagePointer(), 1, channels, cv::Mat(), histogram, 1, maxBin, ranges);

	cv::Mat normalizedHistogram;
	cv::normalize(histogram, normalizedHistogram, 1, 0, cv::NORM_INF, -1, cv::Mat());
	int windowWidth = 800;
	int windowHeight = 450;
	int width = 2;
	cv::Mat histImage(windowHeight, windowWidth, CV_8UC3, cv::Scalar(0, 0, 0));
	for (int i = 0; i < histogram.rows; i++)
	{
		cv::rectangle(
			histImage,
			cv::Point(
				width * i,
				windowHeight - 1
			),
			cv::Point(
				width * (i + 1) - 1,
				windowHeight - cvRound(normalizedHistogram.at<float>(i) * windowHeight)
			),
			cv::Scalar(255, 255, 255),
			-1
		);
	}
	cv::imshow(m_Image.GetName() + " | Pixel Histogram", histImage);
}

void Processor::DrawRectangle(Image& image, const cv::Point& pointLU, const cv::Point& pointRD, const Color::RGB& color, int32_t thickness)
{
	cv::rectangle(*image, pointLU, pointRD, cv::Scalar{ color.B, color.G, color.R }, thickness);
}

void Processor::DrawRectangle(Image& image, const cv::Point& point, const cv::Mat& mat, const Color::RGB& color, int32_t thickness)
{
	DrawRectangle(image, point, cv::Point{ point.x + mat.cols, point.y + mat.rows }, color, thickness);
}

void Processor::MatchTemplate(Image& image, const Image& templateImage, cv::TemplateMatchModes flag, const Color::RGB& color)
{
	if (templateImage.GetImage().empty())
	{
		std::cerr << "Template image is empty!" << std::endl;
		return;
	}

	cv::Mat result;
	cv::matchTemplate(image.GetImage(), templateImage.GetImage(), result, cv::TM_CCOEFF_NORMED);
	double minVal, maxVal;
	cv::Point minLoc, maxLoc;
	cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
	DrawRectangle(image, maxLoc, templateImage.GetImage(), color, 2);
}

void Processor::MatchTemplate(Image& image, const std::string& filepath, cv::TemplateMatchModes flag, const Color::RGB& color)
{
	Image templateImage;
	templateImage.LoadImage(filepath);
	MatchTemplate(image, templateImage, flag, color);
}

void Processor::Filter(Image& image, const cv::Size& size, double sigma)
{
	Image processingImage;
	cv::GaussianBlur(image.GetImage(), *processingImage, size, sigma, sigma);
	processingImage.CopyTo(image);
}

void Processor::EdgeDetection(Image& image, EdgeDetectionMethod method, int32_t arg0, int32_t arg1, int32_t arg2)
{
	cv::Mat edgeX, edgeY;
	cv::Mat edge;
	switch (method)
	{
	case EdgeDetectionMethod::Sobel:
		cv::Sobel(image.GetImage(), edgeX, CV_32F, arg0, 0);
		cv::Sobel(image.GetImage(), edgeY, CV_32F, 0, arg0);
		image.SetImage(edgeX + edgeY);
		break;
	case EdgeDetectionMethod::Laplacian:
		cv::Laplacian(image.GetImage(), edge, CV_32F, arg0, arg1);
		image.SetImage(edge);
		break;
	case EdgeDetectionMethod::Canny:
		cv::Canny(image.GetImage(), edge, arg0, arg1);
		image.SetImage(edge);
		break;
	}
}

void Processor::EqualizeHist(Image& image)
{
	Image processingImage;
	cv::equalizeHist(*image, *processingImage);
	processingImage.CopyTo(image);
}

void Processor::ConvertScaleAbs(Image& image, double alpha, double beta)
{
	Image processingImage;
	cv::convertScaleAbs(image.GetImage(), *processingImage, alpha, beta);
	processingImage.CopyTo(image);
}

Image& Processor::Contours()
{
	Convert(m_Image, cv::COLOR_BGR2GRAY);
	cv::Mat binaryImage;
	cv::threshold(*m_Image, binaryImage, 170, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(binaryImage, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point{});
	for (int i = 0; i < contours.size(); ++i)
	{
		cv::drawContours(*m_Image, contours, i, {0, 0, 255}, 5);
	}
	return m_Image;
}

std::pair<Image, Image> Processor::Rect()
{
	// Convert(m_Image, cv::COLOR_BGR2GRAY);
	cv::Mat cannyImg;
	cv::Mat img1{m_Image.GetImage()};
	cv::Mat img2{m_Image.GetImage()};
	// 二值化
	cv::Canny(m_Image.GetImage(), cannyImg, 80, 160, 3);
	// 图像膨胀
	cv::Mat kernelImg{cv::getStructuringElement(0, cv::Size{15, 15})};
	cv::dilate(cannyImg, cannyImg, kernelImg);

	// 外接矩形
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(cannyImg, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point{});
	for (int i = 0; i < contours.size(); ++i)
	{
		// 绘制矩形
		cv::Rect rect{cv::boundingRect(contours[i])};
		cv::rectangle(img1, rect, cv::Scalar{0, 0, 255}, 2);

		// 画四条线
		cv::RotatedRect rotatedRect{cv::minAreaRect(contours[i])};
		cv::Point2f points[4];
		rotatedRect.points(points);
		cv::Point2f center{rotatedRect.center};
		for (int i = 0; i < 4; ++i)
		{
			if (i == 3)
			{
				cv::line(img2, points[i], points[0], cv::Scalar{0, 0, 255}, 2);
				break;
			}
			cv::line(img2, points[i], points[i + 1], cv::Scalar{0, 0, 255}, 2);
		}
		cv::circle(img2, center, 4, cv::Scalar{0, 0, 255}, -1);

	}

	return {img1, img2};
}