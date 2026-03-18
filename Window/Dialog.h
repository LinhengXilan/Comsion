#ifndef WINDOW_DIALOG_H
#define WINDOW_DIALOG_H

#include <QtWidgets/QtWidgets>
#include <QtGui/QImage>
#include <Core/Image.h>

class Dialog : public QDialog
{
	Q_OBJECT
public:
	Dialog(QWidget* parent = nullptr);
	~Dialog() override;

public:
	void SetImage(const Image& image, QImage::Format format = QImage::Format_BGR888);

private:
	void SetImage(QLabel& label, const Image& image, QImage::Format format = QImage::Format_BGR888);

private slots:
	void FileSelectionButtonClicked();
	void RunButtonButtonClicked();

private:
	QLabel* m_LabelLeft;
	QLabel* m_LabelRight;
	QPushButton* m_FileSelectionButton;
	QPushButton* m_RunButton;
	float m_AspectRatio = 0.5;

	Image m_Image;
	bool Is_Run;
};

#endif