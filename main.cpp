#include <QtWidgets/QApplication>
#include <Window/Dialog.h>

int main(int argc, char* argv[])
{
	Image image{"Assets/25mmS/MER-132-43GM(192.168.1.163[00-21-49-00-6E-9C])_2021-09-24_18_13_56_644-0.jpg"};
	QApplication app(argc, argv);
	Dialog dialog;
	dialog.SetImage(image);
	dialog.show();
	return app.exec();
}