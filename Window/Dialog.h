#ifndef WINDOW_DIALOG_H
#define WINDOW_DIALOG_H

#include <QtWidgets/QtWidgets>
#include <Core/Image.h>
#include <Core/Setting.h>

class Dialog : public QDialog
{
	Q_OBJECT
public:
	Dialog(QWidget* parent = nullptr);
	~Dialog() override = default ;

private slots:
	void FileSelectionButtonClicked();
	void RunButtonButtonClicked();
	void SelectTemplateButtonClicked();
	void SelectROIButtonClicked();
	void ReadOptionButtonClicked();
	void SaveOptionButtonClicked();

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

	bool eventFilter(QObject* watched, QEvent* event);
	void OnPaint();
	void OnOutPutPaint();

private:
	// 界面布局
	QLabel* m_LabelLeft;
	QLabel* m_LabelRight;
	QPushButton* m_FileSelectionButton;
	QPushButton* m_RunButton;
	QComboBox* m_SelectModeComboBox;
	QPushButton* m_SelectTemplateButton;
	QPushButton* m_SelectROIButton;

	QLabel* m_KernelParamText;
	QSpinBox* m_KernelParam;
	int m_KernelParamData{0};

	QLabel* m_AreaMinParamText;
	QSpinBox* m_AreaMinParam;
	int m_AreaMinParamData{0};

	QLabel* m_AreaMaxParamText;
	QSpinBox* m_AreaMaxParam;
	int m_AreaMaxParamData{0};

	QPushButton* m_ReadOptionButton;
	QPushButton* m_SaveOptionButton;

	QPushButton* m_QuitButton;

	Setting m_Setting;

	// 图像
	QString m_ImagePath;
	Image m_Image;
	float m_AspectRatio{0.5};
	// 鼠标绘制
	int m_ImageRect[4];
	bool is_MousePressed{false};
	bool is_SelectMode{false};
	// 程序控制
	bool is_Run{false};
};

#endif