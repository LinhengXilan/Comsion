#include <Core/Image.h>

Image::Image(const std::string& filePath)
{
	m_Name = filePath.substr(filePath.find_last_of('/') + 1);
	m_Image = cv::imread(filePath);
	if (m_Image.empty())
	{
		std::cerr << "Failed to load image from: " << filePath << std::endl;
	}
}

[[nodiscard]] cv::Mat Image::GetImage() const
{
	return m_Image;
}

[[nodiscard]] std::string Image::GetName() const
{
	return m_Name;
}

[[nodiscard]] int32_t Image::GetSize() const
{
	return m_Image.cols * m_Image.rows * m_Image.channels();
}

[[nodiscard]] int32_t Image::GetWidth() const
{
	return m_Image.cols;
}

[[nodiscard]] int32_t Image::GetHeight() const
{
	return m_Image.rows;
}

[[nodiscard]] int32_t Image::GetStride() const
{
	return m_Image.cols * m_Image.channels();
}

void Image::CopyImage(const cv::Mat& image)
{
	image.copyTo(m_Image);
}

void Image::SetName(const std::string& name)
{
	m_Name = name;
}

Image& Image::operator=(const Image& image)
{
	m_Image.release();
	image.m_Image.copyTo(this->m_Image);
	return *this;
}

void Image::LoadImage(const std::string& filepath)
{
	m_Image = cv::imread(filepath);
	if (m_Image.empty())
	{
		std::cerr << "Failed to load image from: " << filepath << std::endl;
		return;
	}

	if (m_Name.empty())
	{
		m_Name = filepath.substr(filepath.find_last_of('/') + 1);
	}
}

void Image::ShowImage()
{
	if (m_Image.empty() || m_Name.empty()) return;
	cv::imshow(m_Name, m_Image);
}

void Image::ShowImage(const std::string& windowName)
{
	if (m_Image.empty()) return;
	cv::imshow(windowName, m_Image);
}

void Image::CopyTo(Image& image)
{
	image = *this;
}