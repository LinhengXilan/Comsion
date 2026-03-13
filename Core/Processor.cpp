#include <pch.h>
#include <Core/Processor.h>

Processor::Processor(const Image& image)
	: m_OriginalImage(image)
{

}

[[nodiscard]] Image Processor::GetOriginalImage() const
{
	return m_OriginalImage;
}

Image Processor::Process()
{
	Image image{m_OriginalImage};
	Convert(image, cv::COLOR_BGR2GRAY);
	// EqualizeHist();
	// Filter({ 3,3 }, 5);
	// MatchTemplate("Assets/1t.bmp");
	// ConvertScaleAbs(0.8f, 0.0f);
	//EdgeDetection(EdgeDetectionMethod::Canny, 80, 180);
	return image;
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
	cv::imshow(m_OriginalImage.GetName() + " | Pixel Histogram", histImage);
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