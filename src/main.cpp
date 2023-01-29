#include <QApplication>
#include <QCoreApplication>
#include <QTranslator>

#include "gui/main_window.hpp"

int main(int argc, char* argv[])
{
	QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return QApplication::exec();
}
