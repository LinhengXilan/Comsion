#include <Window/Dialog.h>
#include <Core/Processor.h>

Dialog::Dialog(QWidget* parent)
{
	QVBoxLayout* layout = new QVBoxLayout;

	// 图像显示
	QHBoxLayout* imageLayout = new QHBoxLayout;

	m_LabelLeft = new QLabel;
	m_LabelLeft->setFixedWidth(1292 / 2);
	m_LabelLeft->setFixedHeight(964 / 2);
	m_LabelLeft->installEventFilter(this);
	imageLayout->addWidget(m_LabelLeft);

	imageLayout->addStretch(1);

	m_LabelRight = new QLabel;
	m_LabelRight->setFixedWidth(1292 / 2);
	m_LabelRight->setFixedHeight(964 / 2);
	imageLayout->addWidget(m_LabelRight);

	layout->addLayout(imageLayout);


	// 控制
	QHBoxLayout* controlLayout = new QHBoxLayout;

	m_FileSelectionButton = new QPushButton("选择文件");
	connect(m_FileSelectionButton, SIGNAL(clicked()), this, SLOT(FileSelectionButtonClicked()));
	controlLayout->addWidget(m_FileSelectionButton);

	m_DirSelectionButton = new QPushButton("选择文件夹");
	connect(m_DirSelectionButton, SIGNAL(clicked()), this, SLOT(DirSelectionButtonClicked()));
	controlLayout->addWidget(m_DirSelectionButton);

	m_RunButton = new QPushButton("运行");
	connect(m_RunButton, SIGNAL(clicked()), this, SLOT(RunButtonClicked()));
	controlLayout->addWidget(m_RunButton);

	m_PauseButton = new QPushButton("暂停");
	m_PauseButton->setEnabled(false);
	connect(m_PauseButton, SIGNAL(clicked()), this, SLOT(PauseButtonClicked()));
	controlLayout->addWidget(m_PauseButton);
	
	layout->addLayout(controlLayout);
	
	// ROI
	QHBoxLayout* roiLayout = new QHBoxLayout;
	
	m_SelectROIButton = new QPushButton("选择ROI区域");
	connect(m_SelectROIButton, SIGNAL(clicked()), this, SLOT(SelectROIButtonClicked()));
	roiLayout->addWidget(m_SelectROIButton);

	roiLayout->addStretch(1);

	m_ROIText = new QLabel("ROI区域：");
	roiLayout->addWidget(m_ROIText);
	
	m_ROIDataText = new QLabel("0 ，0 ，0 ，0");
	roiLayout->addWidget(m_ROIDataText);

	layout->addLayout(roiLayout);
	
	// 输出
	QHBoxLayout* outputLayout = new QHBoxLayout;

	m_ClearCmpButton = new QPushButton("清除结果");
	connect(m_ClearCmpButton, SIGNAL(clicked()), this, SLOT(ClearCmpButtonClicked()));
	outputLayout->addWidget(m_ClearCmpButton);

	m_BaseText = new QLabel("基准：");
	outputLayout->addWidget(m_BaseText);

	m_BaseDataText = new QLabel("0");
	outputLayout->addWidget(m_BaseDataText);

	outputLayout->addStretch(1);

	m_CmpOriginText = new QLabel("相较于基准：");
	outputLayout->addWidget(m_CmpOriginText);

	m_CmpOriginDataText = new QLabel("0");
	outputLayout->addWidget(m_CmpOriginDataText);

	outputLayout->addStretch(1);

	m_CmpPreText = new QLabel("相较于上一个：");
	outputLayout->addWidget(m_CmpPreText);

	m_CmpPreDataText = new QLabel("0");
	outputLayout->addWidget(m_CmpPreDataText);

	outputLayout->addStretch(1);

	m_KText = new QLabel("k：");
	outputLayout->addWidget(m_KText);

	m_KDataText = new QLabel("0");
	outputLayout->addWidget(m_KDataText);

	outputLayout->addStretch(1);

	m_ContourCountText = new QLabel("轮廓数量：");
	outputLayout->addWidget(m_ContourCountText);

	m_ContourCountDataText = new QLabel("0");
	outputLayout->addWidget(m_ContourCountDataText);

	layout->addLayout(outputLayout);

	// 参数
	QHBoxLayout* paramLayout = new QHBoxLayout;

	// 膨胀系数
	m_KernelParamText = new QLabel("膨胀系数");
	paramLayout->addWidget(m_KernelParamText);

	m_KernelParam = new QSpinBox;
	m_KernelParam->setFixedHeight(30);
	connect(m_KernelParam, SIGNAL(valueChanged(int)), this, SLOT(KernelParamChanged(int)));
	paramLayout->addWidget(m_KernelParam);

	paramLayout->addStretch(1);

	m_AreaMinParamText = new QLabel("最小面积");
	paramLayout->addWidget(m_AreaMinParamText);

	m_AreaMinParam = new QSpinBox;
	m_AreaMinParam->setFixedHeight(30);
	m_AreaMinParam->setMaximum(10000);
	connect(m_AreaMinParam, SIGNAL(valueChanged(int)), this, SLOT(AreaMinParamChanged(int)));
	paramLayout->addWidget(m_AreaMinParam);

	paramLayout->addStretch(1);

	m_AreaMaxParamText = new QLabel("最大面积");
	paramLayout->addWidget(m_AreaMaxParamText);

	m_AreaMaxParam = new QSpinBox;
	m_AreaMaxParam->setFixedHeight(30);
	m_AreaMaxParam->setMaximum(100000);
	connect(m_AreaMaxParam, SIGNAL(valueChanged(int)), this, SLOT(AreaMaxParamChanged(int)));
	paramLayout->addWidget(m_AreaMaxParam);

	paramLayout->addStretch(1);

	m_ROIjustifyParamText = new QLabel("ROI调整系数");
	paramLayout->addWidget(m_ROIjustifyParamText);

	m_ROIjustifyParam = new QSpinBox;
	m_ROIjustifyParam->setFixedHeight(30);
	m_ROIjustifyParam->setMaximum(10);
	connect(m_ROIjustifyParam, SIGNAL(valueChanged(int)), this, SLOT(ROIjustifyParamChanged(int)));
	paramLayout->addWidget(m_ROIjustifyParam);

	paramLayout->addStretch(1);

	m_ContourMinParamText = new QLabel("最小轮廓数");
	paramLayout->addWidget(m_ContourMinParamText);

	m_ContourMinParam = new QSpinBox;
	m_ContourMinParam->setFixedHeight(30);
	m_ContourMinParam->setMaximum(10);
	connect(m_ContourMinParam, SIGNAL(valueChanged(int)), this, SLOT(ContourMinParamChanged(int)));
	paramLayout->addWidget(m_ContourMinParam);

	paramLayout->addStretch(1);

	m_ContourMaxParamText = new QLabel("最大轮廓数");
	paramLayout->addWidget(m_ContourMaxParamText);

	m_ContourMaxParam = new QSpinBox;
	m_ContourMaxParam->setFixedHeight(30);
	m_ContourMaxParam->setMaximum(10);
	connect(m_ContourMaxParam, SIGNAL(valueChanged(int)), this, SLOT(ContourMaxParamChanged(int)));
	paramLayout->addWidget(m_ContourMaxParam);

	// 配置
	m_Setting.Load("Settings.ini");
	LoadSetting();

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
	connect(m_QuitButton, SIGNAL(clicked()), this, SLOT(QuitButtonClicked()));
	programControlLayout->addWidget(m_QuitButton);

	layout->addLayout(programControlLayout);

	setLayout(layout);
	connect(this, SIGNAL(ImageReady(const QString&)), this, SLOT(OnImageReady(const QString&)));
}

/*****************************************************************************/

void Dialog::FileSelectionButtonClicked()
{
	m_InputType = InputType::File;
	is_Run = false;
	m_ImagePath = QFileDialog::getOpenFileName(
		this,
		"选择文件",
		".",
		"图像文件 (*)"
	);
	if (m_ImagePath.isEmpty())
	{
		QMessageBox::warning(this, "错误", "文件不存在", QMessageBox::Ok);
		return;
	}
	m_Image.LoadImage(m_ImagePath.toStdString());
	m_ROIData[0] = 0;
	m_ROIData[1] = 0;
	m_ROIData[2] = m_Image.GetWidth() / 2 - 1;
	m_ROIData[3] = m_Image.GetHeight() / 2 - 1;

	this->update();
}

void Dialog::DirSelectionButtonClicked()
{
	m_InputType = InputType::Directory;
	is_Run = false;
	QString path = QFileDialog::getExistingDirectory(this, "选择文件夹", ".");
	if (path.isEmpty())
	{
		return;
	}
	QDir dir(path);
	if (!dir.exists())
	{
		QMessageBox::warning(this, "错误", "文件夹为空", QMessageBox::Ok);
		return;
	}
	dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
	QStringList imageList = dir.entryList();
	for (auto& it : imageList)
	{
		QString imagePath = dir.absolutePath() + "/" + it;
		m_ImageVec.push_back(imagePath.toStdString());
	}
	std::ranges::sort(
		m_ImageVec,
		[](const std::string &a, const std::string& b) {
			std::string n1 = a.substr(a.find_last_of('/') + 1);
			std::string n2 = b.substr(b.find_last_of('/') + 1);
			return std::stoi(n1) < std::stoi(n2);
		}
	);

	m_ImagePath = m_ImageVec.front().c_str();
	m_Image.LoadImage(m_ImagePath.toStdString());
	m_ROIData[0] = 0;
	m_ROIData[1] = 0;
	m_ROIData[2] = m_Image.GetWidth() / 2 - 1;
	m_ROIData[3] = m_Image.GetHeight() / 2 - 1;
	this->update();
}

void Dialog::RunButtonClicked()
{
	is_Run = true;
	m_PauseButton->setEnabled(true);

	switch (m_InputType)
	{
	case InputType::File:
	{
		Image image;
		image.LoadImage(m_ImagePath.toStdString());
		m_Image = image;

		m_Processor.LoadImage(image);
		m_Processor.ClearResult();
		m_Result = m_Processor.Rect(m_Setting, m_ROIData);

		m_Image = m_Result.image;
		m_KDataText->setText(std::to_string(m_Result.k).c_str());
		m_BaseDataText->setText(std::to_string(m_Result.origin).c_str());
		m_CmpOriginDataText->setText(std::to_string(m_Result.current - m_Result.origin).c_str());
		m_CmpPreDataText->setText(std::to_string(m_Result.delta).c_str());
		m_ContourCountDataText->setText(std::to_string(m_Result.contourCount).c_str());
		emit ImageReady(m_ImagePath);
		break;
	}
	case InputType::Directory:
	{
		m_Processor.ClearResult();
		m_Future = QtConcurrent::run(
			[this] {
				Image image;
				for (auto& it : m_ImageVec)
				{
					while (is_Paused.load())
					{
						if (!is_Paused.load())
						{
							break;
						}
					}
					image.LoadImage(it);
					m_Image = image;
					m_Processor.LoadImage(image);
					m_Result = m_Processor.Rect(m_Setting, m_ROIData);
					QThread::msleep(500);
					emit ImageReady(it.c_str());
					if (m_Future.isFinished())
					{
						m_PauseButton->setEnabled(false);
					}
				}
			}
		);
		break;
	}
	default:
		break;
	}
	this->update();
}

void Dialog::PauseButtonClicked()
{
	if (is_Paused.load())
	{
		is_Paused = false;
		m_PauseButton->setText("暂停");
	}
	else
	{
		is_Paused = true;
		m_PauseButton->setText("继续运行");
	}
}

void Dialog::SelectROIButtonClicked()
{
	is_SelectMode = true;
}

/*****************************************************************************/

void Dialog::ClearCmpButtonClicked()
{
	m_KDataText->setText("0");
	m_BaseDataText->setText("0");
	m_CmpOriginDataText->setText("0");
	m_CmpPreDataText->setText("0");
	m_KDataText->setText("0");
	m_ContourCountDataText->setText("0");
}

/*****************************************************************************/

void Dialog::StopButtonClicked()
{

}

/*****************************************************************************/

void Dialog::ReadOptionButtonClicked()
{
	QString filepath = QFileDialog::getOpenFileName(
		nullptr,
		"选择文件",
		".",
		"配置文件 (*.ini)"
	);
	std::cout << filepath.toStdString() << std::endl;
	m_Setting.Load(filepath.toStdString());

	LoadSetting();

	is_Run = false;
}

void Dialog::LoadSetting()
{
	m_KernelParam->setValue(m_Setting.GetData("KernelParam"));
	m_AreaMinParam->setValue(m_Setting.GetData("AreaMinParam"));
	m_AreaMaxParam->setValue(m_Setting.GetData("AreaMaxParam"));
	m_ROIjustifyParam->setValue(m_Setting.GetData("ROIJustifyParam"));
	m_ContourMinParam->setValue(m_Setting.GetData("ContourMinParam"));
	m_ContourMaxParam->setValue(m_Setting.GetData("ContourMaxParam"));
}

void Dialog::SaveOptionButtonClicked()
{
	QString filepath = QFileDialog::getSaveFileName(
		nullptr,
		"选择文件",
		".",
		"配置文件 (*.ini)"
	);
	m_Setting.Save(filepath.toStdString());
}

void Dialog::KernelParamChanged(int value)
{
	m_Setting.SetData("KernelParam", std::to_string(value));
}

void Dialog::AreaMinParamChanged(int value)
{
	m_Setting.SetData("AreaMinParam", std::to_string(value));
}

void Dialog::AreaMaxParamChanged(int value)
{
	m_Setting.SetData("AreaMaxParam", std::to_string(value));
}

void Dialog::ROIjustifyParamChanged(int value)
{
	m_Setting.SetData("ROIJustifyParam", std::to_string(value));
}

void Dialog::ContourMinParamChanged(int value)
{
	m_Setting.SetData("ContourMinParam", std::to_string(value));
}

void Dialog::ContourMaxParamChanged(int value)
{
	m_Setting.SetData("ContourMaxParam", std::to_string(value));
}

/*****************************************************************************/

void Dialog::QuitButtonClicked()
{
	this->close();
}

/*****************************************************************************/

void Dialog::OnPaint()
{
	QPainter painter{this->m_LabelLeft};
	QPen pen;
	QPixmap pixmap{m_ImagePath};
	pen.setColor(Qt::blue);
	painter.setPen(pen);

	painter.drawPixmap(0, 0, pixmap.width() * m_AspectRatio, pixmap.height() * m_AspectRatio, QPixmap{m_ImagePath});
	painter.drawRect(
		m_ROIData[0],
		m_ROIData[1],
		m_ROIData[2] - m_ROIData[0],
		m_ROIData[3] - m_ROIData[1]
	);
	std::string roi = std::to_string(m_ROIData[0]);
	roi.append(" ，");
	roi.append(std::to_string(m_ROIData[1]));
	roi.append(" ，");
	roi.append(std::to_string(m_ROIData[2]));
	roi.append(" ，");
	roi.append(std::to_string(m_ROIData[3]));
	m_ROIDataText->setText(roi.c_str());
}

void Dialog::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton && is_SelectMode)
	{
		is_MousePressed = true;
		m_ROIData[0] = event->position().x() - m_LabelLeft->pos().x();
		m_ROIData[1] = event->position().y();
		m_ROIData[2] = 0;
		m_ROIData[3] = 0;
	}
	this->update();
}

void Dialog::mouseMoveEvent(QMouseEvent* event)
{
	if (is_MousePressed && is_SelectMode)
	{
		m_ROIData[2] = event->position().x() - m_LabelLeft->pos().x();
		m_ROIData[3] = event->position().y();
	}
	this->update();
}

void Dialog::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton && is_MousePressed)
	{
		is_MousePressed = false;
		is_SelectMode = false;
		m_ROIData[2] = event->position().x() - m_LabelLeft->pos().x();
		m_ROIData[3] = event->position().y();
	}
	this->update();
}

bool Dialog::eventFilter(QObject* watched, QEvent* event)
{
	if (watched == this->m_LabelLeft && event->type() == QEvent::Paint)
	{
		OnPaint();
	}
	return QWidget::eventFilter(watched, event);
}

void Dialog::OnImageReady(const QString& path)
{
	m_Image = m_Result.image;
	m_ImagePath = path;

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

	m_KDataText->setText(std::to_string(m_Result.k).c_str());
	m_BaseDataText->setText(std::to_string(m_Result.origin).c_str());
	m_CmpOriginDataText->setText(std::to_string(m_Result.current - m_Result.origin).c_str());
	m_CmpPreDataText->setText(std::to_string(m_Result.delta).c_str());
	m_ContourCountText->setText(std::to_string(m_Result.contourCount).c_str());
}