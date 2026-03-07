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

private:
	void SetImage(const Image& image);

private slots:
	void FileSelectionButtonClicked();

private:
	QLabel* m_Label;
	QPushButton* m_FileSelectionButton;
};

#endif