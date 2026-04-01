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

public:
	explicit Dialog(QWidget* parent = nullptr);
	~Dialog() override = default;

private slots:
	void FileSelectionButtonClicked();
	void RunButtonClicked();
	void StopButtonClicked();
	void DirSelectionButtonClicked();
	void SelectROIButtonClicked();

	void PauseButtonClicked();

	void ClearCmpButtonClicked();

	void ReadOptionButtonClicked();
	void SaveOptionButtonClicked();
	void KernelParamChanged(int value);
	void AreaMinParamChanged(int value);
	void AreaMaxParamChanged(int value);

	void OnImageReady(const QString& path);

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
	QPushButton* m_RunButton;
	QPushButton* m_PauseButton;
	std::atomic<bool> is_Paused = false;
	QPushButton* m_SelectROIButton;

	QPushButton* m_ClearCmpButton;
	QLabel* m_BaseText;
	QLabel* m_BaseDataText;
	QLabel* m_CmpOriginText;
	QLabel* m_CmpOriginDataText;
	QLabel* m_CmpPreText;
	QLabel* m_CmpPreDataText;
	QLabel* m_KText;
	QLabel* m_KDataText;

	QLabel* m_KernelParamText;
	QSpinBox* m_KernelParam;
	int m_KernelParamData = 0;

	QLabel* m_AreaMinParamText;
	QSpinBox* m_AreaMinParam;
	int m_AreaMinParamData = 0;

	QLabel* m_AreaMaxParamText;
	QSpinBox* m_AreaMaxParam;
	int m_AreaMaxParamData = 0;

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
	// 鼠标绘制
	double m_ImageRect[4];
	bool is_MousePressed = false;
	bool is_SelectMode = false;
	// 程序控制
	bool is_Run = false;
	InputType m_InputType = InputType::None;
};

#endif