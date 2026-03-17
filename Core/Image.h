#ifndef CORE_IMAGE_H
#define CORE_IMAGE_H

#include <Core/Type.h>

class Image
{
public:
	Image();
	Image(const Image& image);
	Image(const cv::Mat& image);
	~Image() = default;

public:
	[[nodiscard]] cv::Mat GetImage() const;
	[[nodiscard]] cv::Mat* GetImagePointer() const;
	[[nodiscard]] std::string GetName() const;
	[[nodiscard]] int32_t GetSize() const;
	[[nodiscard]] int32_t GetWidth() const;
	[[nodiscard]] int32_t GetHeight() const;
	[[nodiscard]] int32_t GetStride() const;

public:
	void SetImage(const cv::Mat& image);
	void SetImage(const Image& image);
	void SetName(const std::string& name);

public:
	Image& operator=(const Image& image);
	cv::Mat& operator*();
	const cv::Mat& operator*() const;

public:
	void LoadImage(const std::string& filepath);
	void CopyTo(Image& image) const;

private:
	std::string m_Name;
	ObjectRef<cv::Mat> m_Image;
};

#endif