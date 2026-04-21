#ifndef WINDOW_DIALOG_H
#define WINDOW_DIALOG_H

#include <QtWidgets/QtWidgets>
#include <Core/Image.h>
#include <Core/Setting.h>
#include <QtConcurrent/QtConcurrent>
#include <Core/Processor.h>

class Dialog : public QDialog
{
	Q_OBJECT

private:
	enum class InputType : uint8_t
	{
		None,
		File,
		Directory,
	};

	enum class ProcessType : uint8_t
	{
		Rect,
		Match
	};

	enum class SelectMode : uint8_t
	{
		RoiArea,
		TemplateImage
	};

public:
	explicit Dialog(QWidget* parent = nullptr);
	~Dialog() override = default;

private slots:
	void FileSelectionButtonClicked();
	void DirSelectionButtonClicked();
	void ProcessModeComboBoxChanged(int value);
	void RunButtonClicked();
	void StopButtonClicked();

	void SelectROIButtonClicked();
	void SelectTemplateButtonClicked();

	void PauseButtonClicked();

	void ClearCmpButtonClicked();

	void KernelParamChanged(int value);
	void AreaMinParamChanged(int value);
	void AreaMaxParamChanged(int value);
	void ContourMinParamChanged(int value);
	void ContourMaxParamChanged(int value);
	void ROIjustifyParamChanged(int value);
	void ReadOptionButtonClicked();
	void SaveOptionButtonClicked();

	void QuitButtonClicked();

	void OnImageReady(const QString& path);
signals:
	void ImageReady(const QString& path);

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

	bool eventFilter(QObject* watched, QEvent* event);
	void OnPaint();

private:
	void LoadSetting();

private:
	// 界面布局
	QLabel* m_LabelLeft;
	QLabel* m_LabelRight;
	QPushButton* m_FileSelectionButton;
	QPushButton* m_DirSelectionButton;
	QComboBox* m_ProcessModeComboBox;
	ProcessType m_ProcessType = ProcessType::Rect;
	Image m_Template;
	QPushButton* m_RunButton;
	QPushButton* m_PauseButton;
	std::atomic<bool> is_Paused = false;

	QPushButton* m_SelectROIButton;
	QLabel* m_ROIText;
	QLabel* m_ROIDataText;
	double m_ROIData[4];
	QPushButton* m_SelectTemplateButton;
	QLabel* m_TemplateText;
	QLabel* m_TemplateDataText;
	double m_TemplateData[4];

	QPushButton* m_ClearCmpButton;
	QLabel* m_BaseText;
	QLabel* m_BaseDataText;
	QLabel* m_CmpOriginText;
	QLabel* m_CmpOriginDataText;
	QLabel* m_CmpPreText;
	QLabel* m_CmpPreDataText;
	QLabel* m_KText;
	QLabel* m_KDataText;
	QLabel* m_ContourCountText;
	QLabel* m_ContourCountDataText;

	QLabel* m_KernelParamText;
	QSpinBox* m_KernelParam;
	QLabel* m_AreaMinParamText;
	QSpinBox* m_AreaMinParam;
	QLabel* m_AreaMaxParamText;
	QSpinBox* m_AreaMaxParam;
	QLabel* m_RoiJustifyParamText;
	QSpinBox* m_RoiJustifyParam;
	QLabel* m_ContourMinParamText;
	QSpinBox* m_ContourMinParam;
	QLabel* m_ContourMaxParamText;
	QSpinBox* m_ContourMaxParam;

	QPushButton* m_ReadOptionButton;
	QPushButton* m_SaveOptionButton;

	QPushButton* m_QuitButton;

	//
	Setting m_Setting;
	QFuture<void> m_Future;
	Processor m_Processor;
	Result m_Result;

	// 图像
	QString m_ImagePath;
	std::vector<std::string> m_ImageVec;
	Image m_Image;
	float m_AspectRatio = 0.5;
	SelectMode m_SelectMode;
	// 鼠标绘制
	bool is_MousePressed = false;
	bool is_SelectMode = false;
	// 程序控制
	bool is_Run = false;
	InputType m_InputType = InputType::None;
};

#endif