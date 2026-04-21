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
	int contourCount;
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
	Result Rect(const Setting& setting, double ROI[4]);
	Result Match(const Setting& setting, double ROI[4], double templateArea[4]);

private:
	void Convert(Image& image, cv::ColorConversionCodes type);
	// enum class EdgeDetectionMethod
	// {
	// 	Sobel,
	// 	Laplacian,
	//	Canny
	// };.
	// void EdgeDetection(Image& image, EdgeDetectionMethod method, int32_t arg0 = 1, int32_t arg1 = 1, int32_t arg3 = 1);

private:
	Image m_Image;
	std::vector<double> m_Result;
};

#endif