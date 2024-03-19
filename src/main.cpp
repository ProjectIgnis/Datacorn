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
#ifdef Q_OS_WIN
	if(!a.arguments().contains("--system-theme"))
	{
		a.setStyle("fusion");
		QPalette p;
		p.setColor(QPalette::Window, QColor(52, 52, 52));
		p.setColor(QPalette::WindowText, Qt::white);
		p.setColor(QPalette::Base, QColor(43, 43, 43));
		p.setColor(QPalette::AlternateBase, QColor(52, 52, 52));
		p.setColor(QPalette::ToolTipBase, Qt::black);
		p.setColor(QPalette::ToolTipText, Qt::white);
		p.setColor(QPalette::Text, Qt::white);
		p.setColor(QPalette::Button, QColor(52, 52, 52));
		p.setColor(QPalette::ButtonText, Qt::white);
		p.setColor(QPalette::BrightText, Qt::red);
		p.setColor(QPalette::Link, QColor(220, 206, 128));
		p.setColor(QPalette::Highlight, QColor(220, 206, 128));
		p.setColor(QPalette::HighlightedText, Qt::black);
		a.setPalette(p);
	}
#endif // Q_OS_WIN
	MainWindow w;
	w.show();
	return QApplication::exec();
}
