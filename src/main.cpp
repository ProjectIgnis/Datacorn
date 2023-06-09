#include <QApplication>
#include <QCoreApplication>
#include <QTranslator>

#ifdef Q_OS_WIN
// Needed to ensure that the platform plugin is statically linked.
#include <QtPlugin>
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);
Q_IMPORT_PLUGIN(QICOPlugin);
#endif // Q_OS_WIN

#include "gui/main_window.hpp"

int main(int argc, char* argv[])
{
	// Qt6: These attributes are always enabled.
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
#endif
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return QApplication::exec();
}
