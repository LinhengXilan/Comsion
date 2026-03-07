#ifndef WINDOW_DIALOG_H
#define WINDOW_DIALOG_H

#include <QtWidgets/QtWidgets>
#include <QtGui/QImage>
#include <Core/Image.h>

class Dialog : public QDialog
{
public:
	Dialog(QWidget* parent = nullptr);
	~Dialog();

	void SetImage(const Image& image);

private:
	QImage* m_Image;
	QLabel* m_Label;
};

#endif