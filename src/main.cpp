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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	// Qt6: High-DPI pixmaps are always enabled. This attribute no longer has any effect.
	QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
	// Qt6: High-DPI scaling is always enabled. This attribute no longer has any effect.
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
#endif
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return QApplication::exec();
}
