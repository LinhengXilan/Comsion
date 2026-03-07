#include <pch.h>
#include <Processor/Processor.h>

Processor::Processor(const Image& image)
	: m_OriginalImage(image), m_ProcessedImage(image)
{

}

[[nodiscard]] Image Processor::GetOriginalImage() const
{
	return m_OriginalImage;
}

[[nodiscard]] Image Processor::GetProcessedImage() const
{
	return m_ProcessedImage;
}

[[nodiscard]] Image Processor::GetEdgeImage() const
{
	return m_EdgeImage;
}

void Processor::Process()
{
	Convert(cv::COLOR_BGR2GRAY);
	// EqualizeHist();
	Filter({ 3,3 }, 5);
	// MatchTemplate("Assets/1t.bmp");
	// ConvertScaleAbs(0.8f, 0.0f);
	EdgeDetection(EdgeDetectionMethod::Canny, 80, 180);
}

void Processor::Convert(cv::ColorConversionCodes flag)
{
	Image processingImage;
	cv::cvtColor(*m_ProcessedImage, *processingImage, flag);
	processingImage.CopyTo(m_ProcessedImage);
}

void Processor::PixelHistogram(bool isDrawHist)
{
	cv::Mat histogram;
	constexpr int channels[1] = { 0 };
	constexpr int maxBin[1] = { 256 };
	float range[2] = { 0, 255 };
	const float* ranges[1] = { range };
	cv::calcHist(&m_ProcessedImage, 1, channels, cv::Mat(), histogram, 1, maxBin, ranges);

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
	cv::imshow(m_OriginalImage.GetName() + " | Pixel Histogram", histImage);
}

void Processor::DrawRectangle(const cv::Point& pointLU, const cv::Point& pointRD, const Color::RGB& color, int32_t thickness)
{
	cv::rectangle(*m_ProcessedImage, pointLU, pointRD, cv::Scalar{ color.B, color.G, color.R }, thickness);
}

void Processor::DrawRectangle(const cv::Point& point, const cv::Mat& mat, const Color::RGB& color, int32_t thickness)
{
	DrawRectangle(point, cv::Point{ point.x + mat.cols, point.y + mat.rows }, color, thickness);
}

void Processor::MatchTemplate(const Image& templateImage, cv::TemplateMatchModes flag, const Color::RGB& color)
{
	if (templateImage.GetImage().empty())
	{
		std::cerr << "Template image is empty!" << std::endl;
		return;
	}

	cv::Mat result;
	cv::matchTemplate(m_ProcessedImage.GetImage(), templateImage.GetImage(), result, cv::TM_CCOEFF_NORMED);
	double minVal, maxVal;
	cv::Point minLoc, maxLoc;
	cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
	DrawRectangle(maxLoc, templateImage.GetImage(), color, 2);
}

void Processor::MatchTemplate(const std::string& filepath, cv::TemplateMatchModes flag, const Color::RGB& color)
{
	Image templateImage;
	templateImage.LoadImage(filepath);
	MatchTemplate(templateImage, flag, color);
}

void Processor::Filter(const cv::Size& size, double sigma)
{
	Image processingImage;
	cv::GaussianBlur(m_ProcessedImage.GetImage(), *processingImage, size, sigma, sigma);
	processingImage.CopyTo(m_ProcessedImage);
}

void Processor::EdgeDetection(EdgeDetectionMethod method, int32_t arg0, int32_t arg1, int32_t arg2)
{
	cv::Mat edgeX, edgeY;
	cv::Mat edge;
	switch (method)
	{
	case EdgeDetectionMethod::Sobel:
		cv::Sobel(m_ProcessedImage.GetImage(), edgeX, CV_32F, arg0, 0);
		cv::Sobel(m_ProcessedImage.GetImage(), edgeY, CV_32F, 0, arg0);
		m_EdgeImage.SetImage(edgeX + edgeY);
		break;
	case EdgeDetectionMethod::Laplacian:
		cv::Laplacian(m_ProcessedImage.GetImage(), edge, CV_32F, arg0, arg1);
		m_EdgeImage.SetImage(edge);
		break;
	case EdgeDetectionMethod::Canny:
		cv::Canny(m_ProcessedImage.GetImage(), edge, arg0, arg1);
		m_EdgeImage.SetImage(edge);
		break;
	}
}

void Processor::EqualizeHist()
{
	Image processingImage;
	cv::equalizeHist(*m_ProcessedImage, *processingImage);
	processingImage.CopyTo(m_ProcessedImage);
}

void Processor::ConvertScaleAbs(double alpha, double beta)
{
	Image processingImage;
	cv::convertScaleAbs(m_ProcessedImage.GetImage(), *processingImage, alpha, beta);
	processingImage.CopyTo(m_ProcessedImage);
}