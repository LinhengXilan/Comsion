#ifndef WINDOW_DIALOG_H
#define WINDOW_DIALOG_H

#include <QtWidgets/QtWidgets>
#include <Core/Image.h>

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

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

	bool eventFilter(QObject* watched, QEvent* event);
	void OnPaint();
	void OnOutPutPaint();

private:
	// 界面布局
	QLabel* m_LabelLeft{nullptr};
	QLabel* m_LabelRight{nullptr};
	QPushButton* m_FileSelectionButton{nullptr};
	QPushButton* m_RunButton{nullptr};
	QComboBox* m_SelectModeComboBox{nullptr};
	QPushButton* m_SelectTemplateButton{nullptr};
	QPushButton* m_SelectROIButton{nullptr};
	float m_AspectRatio{0.5};

	QString m_ImagePath;
	Image m_Image;
	// 鼠标绘制
	int m_ImageRect[4];
	bool is_MousePressed{false};
	bool is_SelectMode{false};

	bool is_Run{false};
};

#endif