#include <Window/Dialog.h>
#include <Core/Processor.h>

Dialog::Dialog(QWidget* parent)
{
	QVBoxLayout* layout = new QVBoxLayout;

	// 图像显示
	QGroupBox* imageGroupBox = new QGroupBox;
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
	imageGroupBox->setLayout(imageLayout);
	layout->addWidget(imageGroupBox);


	QGroupBox* generalGroupBox = new QGroupBox;
	QVBoxLayout* generalLayout = new QVBoxLayout;
	// 控制
	QHBoxLayout* controlLayout = new QHBoxLayout;

	m_FileSelectionButton = new QPushButton("选择文件");
	connect(m_FileSelectionButton, SIGNAL(clicked()), this, SLOT(FileSelectionButtonClicked()));
	controlLayout->addWidget(m_FileSelectionButton);

	m_DirSelectionButton = new QPushButton("选择文件夹");
	connect(m_DirSelectionButton, SIGNAL(clicked()), this, SLOT(DirSelectionButtonClicked()));
	controlLayout->addWidget(m_DirSelectionButton);

	m_ProcessModeComboBox = new QComboBox;
	m_ProcessModeComboBox->addItem("外接矩形");
	m_ProcessModeComboBox->addItem("模板匹配");
	connect(m_ProcessModeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(ProcessModeComboBoxChanged(int)));
	controlLayout->addWidget(m_ProcessModeComboBox);

	m_RunButton = new QPushButton("运行");
	connect(m_RunButton, SIGNAL(clicked()), this, SLOT(RunButtonClicked()));
	controlLayout->addWidget(m_RunButton);

	m_PauseButton = new QPushButton("暂停");
	m_PauseButton->setEnabled(false);
	connect(m_PauseButton, SIGNAL(clicked()), this, SLOT(PauseButtonClicked()));
	controlLayout->addWidget(m_PauseButton);

	generalLayout->addLayout(controlLayout);
	
	// ROI
	QGroupBox* roiGroupBox = new QGroupBox("区域选择");

	QHBoxLayout* roiLayout = new QHBoxLayout();

	m_SelectROIButton = new QPushButton("选择ROI区域");
	connect(m_SelectROIButton, SIGNAL(clicked()), this, SLOT(SelectROIButtonClicked()));
	roiLayout->addWidget(m_SelectROIButton);

	m_ROIText = new QLabel("ROI区域：");
	roiLayout->addWidget(m_ROIText);

	m_ROIDataText = new QLabel("0 ，0 ，0 ，0");
	roiLayout->addWidget(m_ROIDataText);

	roiLayout->addStretch(1);

	m_SelectTemplateButton = new QPushButton("选择模板区域");
	//m_SelectTemplateButton->setEnabled(true);
	connect(m_SelectTemplateButton, SIGNAL(clicked()), this, SLOT(SelectTemplateButtonClicked()));
	roiLayout->addWidget(m_SelectTemplateButton);

	m_TemplateText = new QLabel("模板区域");
	roiLayout->addWidget(m_TemplateText);

	m_TemplateDataText = new QLabel("0, 0, 0, 0");
	roiLayout->addWidget(m_TemplateDataText);

	roiGroupBox->setLayout(roiLayout);
	generalLayout->addWidget(roiGroupBox);
	
	// 输出
	QGroupBox* outputGroupBox = new QGroupBox("结果显示");
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

	m_KText = new QLabel("斜率：");
	outputLayout->addWidget(m_KText);

	m_KDataText = new QLabel("0");
	outputLayout->addWidget(m_KDataText);

	outputLayout->addStretch(1);

	m_ContourCountText = new QLabel("轮廓数量：");
	outputLayout->addWidget(m_ContourCountText);

	m_ContourCountDataText = new QLabel("0");
	outputLayout->addWidget(m_ContourCountDataText);

	outputGroupBox->setLayout(outputLayout);
	generalLayout->addWidget(outputGroupBox);



	// 参数
	QGroupBox* paramGroupBox = new QGroupBox("参数控制");
	QVBoxLayout* paramLayout = new QVBoxLayout;

	// 外接矩形参数
	QHBoxLayout* rectParamLayout = new QHBoxLayout;

	// 膨胀系数
	m_KernelParamText = new QLabel("膨胀系数");
	rectParamLayout->addWidget(m_KernelParamText);

	m_KernelParam = new QSpinBox;
	m_KernelParam->setFixedHeight(30);
	connect(m_KernelParam, SIGNAL(valueChanged(int)), this, SLOT(KernelParamChanged(int)));
	rectParamLayout->addWidget(m_KernelParam);

	rectParamLayout->addStretch(1);

	m_AreaMinParamText = new QLabel("最小面积");
	rectParamLayout->addWidget(m_AreaMinParamText);

	m_AreaMinParam = new QSpinBox;
	m_AreaMinParam->setFixedHeight(30);
	m_AreaMinParam->setMaximum(10000);
	connect(m_AreaMinParam, SIGNAL(valueChanged(int)), this, SLOT(AreaMinParamChanged(int)));
	rectParamLayout->addWidget(m_AreaMinParam);

	rectParamLayout->addStretch(1);

	m_AreaMaxParamText = new QLabel("最大面积");
	rectParamLayout->addWidget(m_AreaMaxParamText);

	m_AreaMaxParam = new QSpinBox;
	m_AreaMaxParam->setFixedHeight(30);
	m_AreaMaxParam->setMaximum(100000);
	connect(m_AreaMaxParam, SIGNAL(valueChanged(int)), this, SLOT(AreaMaxParamChanged(int)));
	rectParamLayout->addWidget(m_AreaMaxParam);

	rectParamLayout->addStretch(1);

	m_ContourMinParamText = new QLabel("最小轮廓数");
	rectParamLayout->addWidget(m_ContourMinParamText);

	m_ContourMinParam = new QSpinBox;
	m_ContourMinParam->setFixedHeight(30);
	m_ContourMinParam->setMaximum(10);
	connect(m_ContourMinParam, SIGNAL(valueChanged(int)), this, SLOT(ContourMinParamChanged(int)));
	rectParamLayout->addWidget(m_ContourMinParam);

	rectParamLayout->addStretch(1);

	m_ContourMaxParamText = new QLabel("最大轮廓数");
	rectParamLayout->addWidget(m_ContourMaxParamText);

	m_ContourMaxParam = new QSpinBox;
	m_ContourMaxParam->setFixedHeight(30);
	m_ContourMaxParam->setMaximum(10);
	connect(m_ContourMaxParam, SIGNAL(valueChanged(int)), this, SLOT(ContourMaxParamChanged(int)));
	rectParamLayout->addWidget(m_ContourMaxParam);

	paramLayout->addLayout(rectParamLayout);

	// 模板匹配参数
	QHBoxLayout* matchParamLayout = new QHBoxLayout;
	m_ThresholdParamText = new QLabel("模板匹配筛选阈值");
	matchParamLayout->addWidget(m_ThresholdParamText);

	m_ThresholdParam = new QSpinBox;
	m_ThresholdParam->setFixedHeight(30);
	m_ThresholdParam->setMaximum(100);
	connect(m_ThresholdParam, SIGNAL(valueChanged(int)), this, SLOT(ThresholdParamChanged(int)));
	matchParamLayout->addWidget(m_ThresholdParam);

	paramLayout->addLayout(matchParamLayout);

	// 调整配置
	QHBoxLayout* rejustParamLayout = new QHBoxLayout;
	m_RoiJustifyParamText = new QLabel("ROI调整系数");
	rejustParamLayout->addWidget(m_RoiJustifyParamText);

	m_RoiJustifyParam = new QSpinBox;
	m_RoiJustifyParam->setFixedHeight(30);
	m_RoiJustifyParam->setMaximum(10);
	connect(m_RoiJustifyParam, SIGNAL(valueChanged(int)), this, SLOT(ROIjustifyParamChanged(int)));
	rejustParamLayout->addWidget(m_RoiJustifyParam);

	paramLayout->addLayout(rejustParamLayout);

	// 配置
	QHBoxLayout* settingParamLayout = new QHBoxLayout;

	m_Setting.Load("Settings.ini");
	LoadSetting();

	m_ReadOptionButton = new QPushButton("读取配置");
	connect(m_ReadOptionButton, SIGNAL(clicked()), this, SLOT(ReadOptionButtonClicked()));
	settingParamLayout->addWidget(m_ReadOptionButton);

	m_SaveOptionButton = new QPushButton("保存配置");
	connect(m_SaveOptionButton, SIGNAL(clicked()), this, SLOT(SaveOptionButtonClicked()));
	settingParamLayout->addWidget(m_SaveOptionButton);

	paramLayout->addLayout(settingParamLayout);

	paramGroupBox->setLayout(paramLayout);
	generalLayout->addWidget(paramGroupBox);

	// 软件控制
	QHBoxLayout* programControlLayout = new QHBoxLayout;

	m_QuitButton = new QPushButton("退出");
	connect(m_QuitButton, SIGNAL(clicked()), this, SLOT(QuitButtonClicked()));
	programControlLayout->addWidget(m_QuitButton);

	generalLayout->addLayout(programControlLayout);

	generalGroupBox->setLayout(generalLayout);
	layout->addWidget(generalGroupBox);


	setLayout(layout);
	connect(this, SIGNAL(ImageReady(const QString&, Result)), this, SLOT(OnImageReady(const QString&, Result)));
}

/*****************************************************************************/

void Dialog::FileSelectionButtonClicked()
{
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
	m_InputType = InputType::File;
	m_Image.LoadImage(m_ImagePath.toStdString());
	m_ROIData[0] = 0;
	m_ROIData[1] = 0;
	m_ROIData[2] = m_Image.GetWidth() / 2 - 1;
	m_ROIData[3] = m_Image.GetHeight() / 2 - 1;

	this->update();
}

void Dialog::DirSelectionButtonClicked()
{
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

	m_InputType = InputType::Directory;
	m_ImagePath = m_ImageVec.front().c_str();
	m_Image.LoadImage(m_ImagePath.toStdString());
	m_ROIData[0] = 0;
	m_ROIData[1] = 0;
	m_ROIData[2] = m_Image.GetWidth() / 2 - 1;
	m_ROIData[3] = m_Image.GetHeight() / 2 - 1;
	this->update();
}

void Dialog::ProcessModeComboBoxChanged(int index)
{
	switch (index)
	{
	case 0:
		m_ProcessType = ProcessType::Rect;
		break;
	case 1:
		m_ProcessType = ProcessType::Match;
		break;
	}
}

void Dialog::RunButtonClicked()
{
	if (is_Run) return;
	is_Run = true;
	m_PauseButton->setEnabled(true);

	switch (m_InputType)
	{
	case InputType::File:
	{
		Image image;
		Result result;
		image.LoadImage(m_ImagePath.toStdString());
		m_Image = image;

		m_Processor.LoadImage(image);
		m_Processor.ClearResult();
		switch (m_ProcessType)
		{
		case ProcessType::Rect:
			result = m_Processor.Rect(m_Setting, m_ROIData);
			break;
		case ProcessType::Match:
			result = m_Processor.Match(m_Setting, m_ROIData, m_TemplateData);
			break;
		default:
			break;
		}

		emit ImageReady(m_ImagePath, result);
		break;
	}
	case InputType::Directory:
	{
		m_Processor.ClearResult();
		m_Future = QtConcurrent::run(
			[this] {
				Image image;
				Result result;
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
					switch (m_ProcessType)
					{
					case ProcessType::Rect:
						result = m_Processor.Rect(m_Setting, m_ROIData);
						break;
					case ProcessType::Match:
						result = m_Processor.Match(m_Setting, m_ROIData, m_TemplateData);
						break;
					default:
						break;
					}
					QThread::msleep(33);
					emit ImageReady(it.c_str(), result);
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
	m_SelectMode = SelectMode::RoiArea;
	is_SelectMode = true;
}

void Dialog::SelectTemplateButtonClicked()
{
	m_SelectMode = SelectMode::TemplateImage;
	is_SelectMode = true;
}

/*****************************************************************************/

void Dialog::ClearCmpButtonClicked()
{
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
	m_KernelParam->setValue(m_Setting.GetData("Kernel"));
	m_AreaMinParam->setValue(m_Setting.GetData("AreaMin"));
	m_AreaMaxParam->setValue(m_Setting.GetData("AreaMax"));
	m_RoiJustifyParam->setValue(m_Setting.GetData("ROIJustify"));
	m_ContourMinParam->setValue(m_Setting.GetData("ContourMin"));
	m_ContourMaxParam->setValue(m_Setting.GetData("ContourMax"));
	m_ThresholdParam->setValue(m_Setting.GetData("Threshold"));
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
	m_Setting.SetData("Kernel", std::to_string(value));
}

void Dialog::AreaMinParamChanged(int value)
{
	m_Setting.SetData("AreaMin", std::to_string(value));
}

void Dialog::AreaMaxParamChanged(int value)
{
	m_Setting.SetData("AreaMax", std::to_string(value));
}

void Dialog::ContourMinParamChanged(int value)
{
	m_Setting.SetData("ContourMin", std::to_string(value));
}

void Dialog::ContourMaxParamChanged(int value)
{
	m_Setting.SetData("ContourMax", std::to_string(value));
}

void Dialog::ThresholdParamChanged(int value)
{
	m_Setting.SetData("Threshold", std::to_string(value));
}

void Dialog::ROIjustifyParamChanged(int value)
{
	m_Setting.SetData("ROIJustify", std::to_string(value));
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

	pen.setColor(Qt::green);
	painter.drawRect(
		m_TemplateData[0],
		m_TemplateData[1],
		m_TemplateData[2] - m_TemplateData[0],
		m_TemplateData[3] - m_TemplateData[1]
	);

	std::string roi = std::to_string(m_ROIData[0]);
	roi.append(", ");
	roi.append(std::to_string(m_ROIData[1]));
	roi.append(", ");
	roi.append(std::to_string(m_ROIData[2]));
	roi.append(", ");
	roi.append(std::to_string(m_ROIData[3]));
	m_ROIDataText->setText(roi.c_str());

	std::string templateText = std::to_string(m_TemplateData[0]);
	templateText.append(", ");
	templateText.append(std::to_string(m_TemplateData[1]));
	templateText.append(", ");
	templateText.append(std::to_string(m_TemplateData[2]));
	templateText.append(", ");
	templateText.append(std::to_string(m_TemplateData[3]));
	m_TemplateDataText->setText(templateText.c_str());
}

void Dialog::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton && is_SelectMode)
	{
		is_MousePressed = true;
		double* data;
		switch (m_SelectMode)
		{
		case SelectMode::RoiArea:
			data = m_ROIData;
			break;
		case SelectMode::TemplateImage:
			data = m_TemplateData;
			break;
		}
		data[0] = event->position().x() - m_LabelLeft->pos().x();
		data[1] = event->position().y();
		data[2] = 0;
		data[3] = 0;
	}
	this->update();
}

void Dialog::mouseMoveEvent(QMouseEvent* event)
{
	if (is_MousePressed && is_SelectMode)
	{
		double* data;
		switch (m_SelectMode)
		{
		case SelectMode::RoiArea:
			data = m_ROIData;
			break;
		case SelectMode::TemplateImage:
			data = m_TemplateData;
			break;
		}
		data[2] = event->position().x() - m_LabelLeft->pos().x();
		data[3] = event->position().y();
	}
	this->update();
}

void Dialog::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton && is_MousePressed)
	{
		double* data;
		is_MousePressed = false;
		is_SelectMode = false;
		switch (m_SelectMode)
		{
		case SelectMode::RoiArea:
			data = m_ROIData;
			break;
		case SelectMode::TemplateImage:
			data = m_TemplateData;
			break;
		}
		data[2] = event->position().x() - m_LabelLeft->pos().x();
		data[3] = event->position().y();
		if (data[0] > data[2])
		{
			std::swap(data[0], data[2]);
		}
		if (data[1] > data[3])
		{
			std::swap(data[1], data[3]);
		}
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

void Dialog::OnImageReady(const QString& path, const Result& result)
{
	m_Image = result.image;
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
	m_KDataText->setText(std::to_string(result.k).c_str());
	m_BaseDataText->setText(std::to_string(result.origin).c_str());
	m_CmpOriginDataText->setText(std::to_string(result.current - result.origin).c_str());
	m_CmpPreDataText->setText(std::to_string(result.delta).c_str());
	m_ContourCountDataText->setText(std::to_string(result.contourCount).c_str());
}