#include <Window/Dialog.h>

Dialog::Dialog(QWidget* parent)
{
	QVBoxLayout* layout = new QVBoxLayout;

	m_FileSelectionButton = new QPushButton;
	m_FileSelectionButton->setText("选择文件");
	layout->addWidget(m_FileSelectionButton);

	m_Label = new QLabel;
	m_Label->setFixedWidth(1292);
	m_Label->setFixedHeight(964);
	layout->addWidget(m_Label);

	setLayout(layout);

	connect(m_FileSelectionButton, SIGNAL(clicked()), this, SLOT(FileSelectionButtonClicked()));
}

Dialog::~Dialog()
{

}

void Dialog::SetImage(const Image& image)
{
	int width = image.GetWidth();
	int height = image.GetHeight();
	QImage qImage{image.GetImage().data, width, height, QImage::Format_BGR888};
	m_Label->setPixmap(QPixmap::fromImage(qImage));
}

void Dialog::FileSelectionButtonClicked()
{
	QString filePath = QFileDialog::getOpenFileName(
		nullptr,
		"选择文件",
		".",
		"位图文件 (*.bmp *.jpg)"
	);
	if (filePath.isEmpty())
	{
		return;
	}
	Image image{filePath.toStdString()};
	SetImage(image);
}