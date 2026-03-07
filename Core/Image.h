#ifndef CORE_IMAGE_H
#define CORE_IMAGE_H

class Image
{
public:
	Image() = default;
	explicit Image(const std::string& filePath);
	~Image() = default;

	[[nodiscard]] cv::Mat GetImage() const;
	[[nodiscard]] std::string GetName() const;
	[[nodiscard]] int32_t GetSize() const;
	[[nodiscard]] int32_t GetWidth() const;
	[[nodiscard]] int32_t GetHeight() const;
	[[nodiscard]] int32_t GetStride() const;

	void CopyImage(const cv::Mat& image);
	void SetName(const std::string& name);

	Image& operator=(const Image& image);

	void LoadImage(const std::string& filepath);
	void ShowImage();
	void ShowImage(const std::string& windowName);
	void CopyTo(Image& image);

private:
	cv::Mat m_Image;
	std::string m_Name;
};

#endif