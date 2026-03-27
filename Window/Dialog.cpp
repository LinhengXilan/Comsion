#include <Window/Dialog.h>
#include <Core/Processor.h>

Dialog::Dialog(QWidget* parent)
{
	//
	m_ImageRect[0] = 0;
	m_ImageRect[1] = 0;
	m_ImageRect[2] = 0;
	m_ImageRect[3] = 0;

	QVBoxLayout* layout = new QVBoxLayout;

	// 图像显示
	QHBoxLayout* imageLayout = new QHBoxLayout;

	m_LabelLeft = new QLabel;
	m_LabelLeft->setFixedWidth(1292 / 2);
	m_LabelLeft->setFixedHeight(964 / 2);
	m_LabelLeft->installEventFilter(this);
	imageLayout->addWidget(m_LabelLeft);

	imageLayout->addStretch(10);

	m_LabelRight = new QLabel;
	m_LabelRight->setFixedWidth(1292 / 2);
	m_LabelRight->setFixedHeight(964 / 2);
	m_LabelRight->installEventFilter(this);
	imageLayout->addWidget(m_LabelRight);

	layout->addLayout(imageLayout);

	// 控制
	QHBoxLayout* controlLayout = new QHBoxLayout;
	m_FileSelectionButton = new QPushButton;
	m_FileSelectionButton->setText("选择文件");
	controlLayout->addWidget(m_FileSelectionButton);
	connect(m_FileSelectionButton, SIGNAL(clicked()), this, SLOT(FileSelectionButtonClicked()));

	m_RunButton = new QPushButton;
	m_RunButton->setText("运行");
	controlLayout->addWidget(m_RunButton);
	connect(m_RunButton, SIGNAL(clicked()), this, SLOT(RunButtonButtonClicked()));

	m_SelectModeComboBox = new QComboBox;
	controlLayout->addWidget(m_SelectModeComboBox);

	m_SelectTemplateButton = new QPushButton;
	m_SelectTemplateButton->setText("选择模板");
	m_SelectTemplateButton->setEnabled(false);
	controlLayout->addWidget(m_SelectTemplateButton);
	connect(m_SelectTemplateButton, SIGNAL(clicked()), this, SLOT(SelectTemplateButtonClicked()));

	m_SelectROIButton = new QPushButton;
	m_SelectROIButton->setText("选择ROI区域");
	// m_SelectROIButton->setEnabled(false);
	controlLayout->addWidget(m_SelectROIButton);
	connect(m_SelectROIButton, SIGNAL(clicked()), this, SLOT(SelectROIButtonClicked()));

	layout->addLayout(controlLayout);

	QHBoxLayout* paramLayout = new QHBoxLayout;

	// 膨胀系数
	m_KernelParamText = new QLabel;
	m_KernelParamText->setText("膨胀系数");
	paramLayout->addWidget(m_KernelParamText);

	m_KernelParam = new QSpinBox;
	m_KernelParam->setFixedHeight(30);
	paramLayout->addWidget(m_KernelParam);
	paramLayout->addStretch(1);

	// 筛选最小面积
	m_AreaMinParamText = new QLabel;
	m_AreaMinParamText->setText("最小面积");
	paramLayout->addWidget(m_AreaMinParamText);

	m_AreaMinParam = new QSpinBox;
	m_AreaMinParam->setFixedHeight(30);
	paramLayout->addWidget(m_AreaMinParam);
	paramLayout->addStretch(1);

	// 筛选最大面积
	m_AreaMaxParamText = new QLabel;
	m_AreaMaxParamText->setText("最大面积");
	paramLayout->addWidget(m_AreaMaxParamText);

	m_AreaMaxParam = new QSpinBox;
	m_AreaMaxParam->setFixedHeight(30);
	paramLayout->addWidget(m_AreaMaxParam);

	paramLayout->addStretch(10);

	// 配置
	m_ReadOptionButton = new QPushButton("读取配置");
	connect(m_ReadOptionButton, SIGNAL(clicked()), this, SLOT(ReadOptionButtonClicked()));
	paramLayout->addWidget(m_ReadOptionButton);

	m_SaveOptionButton = new QPushButton("保存配置");
	connect(m_SaveOptionButton, SIGNAL(clicked()), this, SLOT(SaveOptionButtonClicked()));
	paramLayout->addWidget(m_SaveOptionButton);

	layout->addLayout(paramLayout);

	// 软件控制
	QHBoxLayout* programControlLayout = new QHBoxLayout;

	m_QuitButton = new QPushButton("退出");



	setLayout(layout);
}

void Dialog::FileSelectionButtonClicked()
{
	m_ImagePath = QFileDialog::getOpenFileName(
		nullptr,
		"选择文件",
		".",
		"位图文件 (*.bmp *.jpg *.png)"
	);
	if (m_ImagePath.isEmpty())
	{
		QMessageBox::warning(this, "错误", "文件打开失败", QMessageBox::Ok);
	}
	m_Image.LoadImage(m_ImagePath.toStdString());
	m_ImageRect[0] = 0;
	m_ImageRect[1] = 0;
	m_ImageRect[2] = m_Image.GetWidth() - 1;
	m_ImageRect[3] = m_Image.GetHeight() - 1;
	is_Run = false;
	this->update();
}

void Dialog::RunButtonButtonClicked()
{
	Image image;
	image.LoadImage(m_ImagePath.toStdString());
	Processor processor{image};
	m_Image = processor.Rect(m_ImageRect);
	is_Run = true;
	this->update();
}

void Dialog::SelectTemplateButtonClicked()
{

}

void Dialog::SelectROIButtonClicked()
{
	is_SelectMode = true;
}

void Dialog::OnPaint()
{
	QPainter painter{this->m_LabelLeft};
	QPen pen;
	QPixmap pixmap{m_ImagePath};
	pen.setColor(Qt::blue);
	painter.setPen(pen);

	painter.drawPixmap(0, 0, pixmap.width() * m_AspectRatio, pixmap.height() * m_AspectRatio, QPixmap{m_ImagePath});
	painter.drawRect(
		m_ImageRect[0],
		m_ImageRect[1],
		m_ImageRect[2] - m_ImageRect[0],
		m_ImageRect[3] - m_ImageRect[1]
	);

#ifdef COMSION_DEBUG
	for (auto& i : m_ImageRect)
	{
		std::cout << i << ' ';
	}
	std::cout << std::endl;
#endif
}

void Dialog::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton && is_SelectMode)
	{
		is_MousePressed = true;
		m_ImageRect[0] = event->pos().x() - m_LabelLeft->pos().x();
		m_ImageRect[1] = event->pos().y();
		m_ImageRect[2] = 0;
		m_ImageRect[3] = 0;
	}
	this->update();
}

void Dialog::mouseMoveEvent(QMouseEvent* event)
{
	if (is_MousePressed && is_SelectMode)
	{
		m_ImageRect[2] = event->pos().x() - m_LabelLeft->pos().x();
		m_ImageRect[3] = event->pos().y();
	}
	this->update();
}

void Dialog::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton && is_MousePressed)
	{
		is_MousePressed = false;
		is_SelectMode = false;
		m_ImageRect[2] = event->pos().x() - m_LabelLeft->pos().x();
		m_ImageRect[3] = event->pos().y();
	}
	this->update();
}

bool Dialog::eventFilter(QObject* watched, QEvent* event)
{
	if (watched == this->m_LabelLeft && event->type() == QEvent::Paint)
	{
		OnPaint();
	}
	if (watched == this->m_LabelRight && event->type() == QEvent::Paint && is_Run)
	{
		OnOutPutPaint();
	}
	return QWidget::eventFilter(watched, event);
}

void Dialog::OnOutPutPaint()
{
	int width = m_Image.GetWidth();
	int height = m_Image.GetHeight();
	QImage qImage{m_Image.GetImage().data, width, height, QImage::Format_BGR888};
	m_LabelRight->setPixmap(
		QPixmap::fromImage(qImage).scaled(
			width * m_AspectRatio,
			height * m_AspectRatio,
			Qt::KeepAspectRatio,
			Qt::SmoothTransformation
		)
	);
}

void Dialog::ReadOptionButtonClicked()
{
	QString filepath = QFileDialog::getOpenFileName(
		nullptr,
		"选择文件",
		".",
		"配置文件 (*.ini)"
	);
	m_Setting.Load(filepath.toStdString());
	if (m_ImagePath.isEmpty())
	{
		QMessageBox::warning(this, "错误", "文件打开失败", QMessageBox::Ok);
	}

	is_Run = false;
}

void Dialog::SaveOptionButtonClicked()
{

}