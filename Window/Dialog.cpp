#include <Window/Dialog.h>
#include <Core/Processor.h>

Dialog::Dialog(QWidget* parent)
{
	QVBoxLayout* layout = new QVBoxLayout;

	// 按钮
	QHBoxLayout* buttonLayout = new QHBoxLayout;
	m_FileSelectionButton = new QPushButton;
	m_FileSelectionButton->setText("选择文件");
	buttonLayout->addWidget(m_FileSelectionButton);
	connect(m_FileSelectionButton, SIGNAL(clicked()), this, SLOT(FileSelectionButtonClicked()));

	m_RunButton = new QPushButton;
	m_RunButton->setText("运行");
	buttonLayout->addWidget(m_RunButton);
	connect(m_RunButton, SIGNAL(clicked()), this, SLOT(RunButtonButtonClicked()));
	layout->addLayout(buttonLayout);

	// 图像显示
	QHBoxLayout* imageLayout = new QHBoxLayout;

	m_LabelLeft = new QLabel;
	m_LabelLeft->setFixedWidth(1292 / 2);
	m_LabelLeft->setFixedHeight(964 / 2);
	imageLayout->addWidget(m_LabelLeft);

	imageLayout->addStretch(10);

	m_LabelRight = new QLabel;
	m_LabelRight->setFixedWidth(1292 / 2);
	m_LabelRight->setFixedHeight(964 / 2);
	imageLayout->addWidget(m_LabelRight);

	layout->addLayout(imageLayout);

	setLayout(layout);
}

Dialog::~Dialog()
{

}

void Dialog::SetImage(const Image& image, QImage::Format format)
{
	SetImage(*m_LabelRight, image, format);
}

void Dialog::SetImage(QLabel& label, const Image& image, QImage::Format format)
{
	int width = image.GetWidth();
	int height = image.GetHeight();
#ifdef DEBUG
	QMessageBox::information(
		this,
		"Image Info",
		"Width: " + QString::number(width) + "Height: " + QString::number(height),
		QMessageBox::Ok
	);
#endif
	QImage qImage{image.GetImage().data, width, height, format};
	label.setPixmap(
		QPixmap::fromImage(qImage).scaled(
			width * m_AspectRatio,
			height * m_AspectRatio,
			Qt::KeepAspectRatio,
			Qt::SmoothTransformation
		)
	);
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
		QMessageBox::warning(this, "错误", "文件打开失败", QMessageBox::Ok);
	}
	m_Image.LoadImage(filePath.toStdString());
	SetImage(*m_LabelLeft, m_Image);
}

void Dialog::RunButtonButtonClicked()
{
	Processor processor{m_Image};
	SetImage(*m_LabelRight, processor.Rect().second, QImage::Format_BGR888);
}