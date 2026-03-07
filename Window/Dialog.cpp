#include <Window/Dialog.h>

Dialog::Dialog(QWidget* parent)
{
	Image image{"Assets/25mmS/MER-132-43GM(192.168.1.163[00-21-49-00-6E-9C])_2021-09-24_18_13_56_644-0.jpg"};

	m_Image = new QImage(image.GetImage().data, image.GetWidth(), image.GetHeight(), image.GetStride(), QImage::Format_BGR888);
	m_Label = new QLabel;
	m_Label->setPixmap(QPixmap::fromImage(m_Image->scaled(m_Label->size(), Qt::KeepAspectRatio)));
	QVBoxLayout* layout{new QVBoxLayout};
	layout->addWidget(m_Label);
	setLayout(layout);
}

Dialog::~Dialog()
{

}

void Dialog::SetImage(const Image& image)
{
	m_Image->loadFromData(image.GetImage().data, image.GetSize());
}