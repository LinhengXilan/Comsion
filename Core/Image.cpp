#include <Core/Image.h>

Image::Image()
	: m_Name("")
{
	m_Image = CreateObjectRef<cv::Mat>();
}

Image::Image(const Image& image)
	: m_Name(image.m_Name)
{
	m_Image = CreateObjectRef<cv::Mat>();
	*m_Image = *image;
}

Image::Image(const cv::Mat& image)
	: m_Name("")
{
	m_Image = CreateObjectRef<cv::Mat>(image);
}

[[nodiscard]] cv::Mat Image::GetImage() const
{
	return *m_Image;
}

[[nodiscard]] cv::Mat* Image::GetImagePointer() const
{
	return m_Image.get();
}

[[nodiscard]] std::string Image::GetName() const
{
	return m_Name;
}

[[nodiscard]] int32_t Image::GetSize() const
{
	return m_Image->cols * m_Image->rows * m_Image->channels();
}

[[nodiscard]] int32_t Image::GetWidth() const
{
	return m_Image->cols;
}

[[nodiscard]] int32_t Image::GetHeight() const
{
	return m_Image->rows;
}

[[nodiscard]] int32_t Image::GetStride() const
{
	return m_Image->step;
}

void Image::SetImage(const cv::Mat& image)
{
	*m_Image = image;
}

void Image::SetImage(const Image& image)
{
	*m_Image = *image;
}

void Image::SetName(const std::string& name)
{
	m_Name = name;
}

Image& Image::operator=(const Image& image)
{
	m_Image->release();
	image.m_Image->copyTo(*m_Image);
	return *this;
}

cv::Mat& Image::operator*()
{
	return *m_Image;
}

const cv::Mat& Image::operator*() const
{
	return *m_Image;
}

void Image::LoadImage(const std::string& filepath)
{
	*m_Image = cv::imread(filepath, cv::IMREAD_COLOR_BGR);
	if (m_Image->empty())
	{
		std::cerr << "Failed to load image from: " << filepath << std::endl;
		return;
	}

	if (m_Name.empty())
	{
		m_Name = filepath.substr(filepath.find_last_of('/') + 1);
	}
}

void Image::CopyTo(Image& image) const
{
	image = *this;
}