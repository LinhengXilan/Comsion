#ifndef PROCESSOR_PROCESSOR_H
#define PROCESSOR_PROCESSOR_H

#include <Core/Image.h>
#include <Core/Setting.h>

struct Result
{
	Image image;
	double origin;
	double current;
	double k;
	double delta;
};

class Processor
{
public:
	Processor() = default;
	Processor(const Image& image);
	~Processor() = default;

public:
	[[nodiscard]] Image GetOriginalImage() const;
	void LoadImage(const Image& image);

public:
	void ClearResult();

public:
	// Image& Process();
	// Image& Contours();
	Result Rect(const Setting& setting, double ROI[4]);

private:
	// void Convert(Image& image, cv::ColorConversionCodes type);
	// void PixelHistogram(Image& image, bool isDrawHist = true);
	// void DrawRectangle(Image& image, const cv::Point& pointLU, const cv::Point& pointRD, const Color::RGB& color, int32_t thickness = 200);
	// void DrawRectangle(Image& image, const cv::Point& point, const cv::Mat& mat, const Color::RGB& color, int32_t thickness = 200);

	/**
	 * @param templateImage 模板图片
	 * @param flag 模板匹配的方式
	 * @param color 矩形框的颜色
	 * @brief 模板匹配，并在处理后的图片上绘制匹配结果的矩形框。
	 * @note 需要保证图像为彩色图像，否则绘制矩形框时会出现问题。
	 */
	// void MatchTemplate(Image& image, const Image& templateImage, cv::TemplateMatchModes flag = cv::TM_CCOEFF_NORMED, const Color::RGB& color = { 0xFF, 0,0 });
	/**
	 * @param filepath 模板图片文件路径
	 * @param flag 模板匹配的方式
	 * @param color 矩形框的颜色
	 * @brief 模板匹配，并在处理后的图片上绘制匹配结果的矩形框。
	 * @note 需要保证图像为彩色图像，否则绘制矩形框时会出现问题。
	 */
	// void MatchTemplate(Image& image, const std::string& filepath, cv::TemplateMatchModes flag = cv::TM_CCOEFF_NORMED, const Color::RGB& color = { 0xFF, 0,0 });
	// void Filter(Image& image, const cv::Size& size, double sigma);
	// enum class EdgeDetectionMethod
	// {
	// 	Sobel,
	// 	Laplacian,
	// 	Canny
	// };
	// void EdgeDetection(Image& image, EdgeDetectionMethod method, int32_t arg0 = 1, int32_t arg1 = 1, int32_t arg3 = 1);
	// void EqualizeHist(Image& image);
	// void ConvertScaleAbs(Image& image, double alpha = 1.0f, double beta = 0.0f);


private:
	Image m_Image;
	std::vector<double> m_Result;
};

#endif