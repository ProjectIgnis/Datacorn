#include <QApplication>
#include <QCoreApplication>
#include <QTranslator>

#ifdef Q_OS_WIN
// Needed to ensure that the platform plugin is statically linked.
#include <QtPlugin>
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);
#endif // Q_OS_WIN

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
