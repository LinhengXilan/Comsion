#include <QtWidgets/QApplication>
#include <Window/Dialog.h>

int main(int argc, char* argv[])
{
	 QApplication app(argc, argv);
	 QFont font("Microsoft YaHei UI");
	 app.setFont(font);

	 Dialog dialog;
	 dialog.show();

	return app.exec();
}