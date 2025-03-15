#include <QApplication>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QTranslator>

#include "gui/main_window.hpp"

int main(int argc, char* argv[])
{
	// Qt6: These attributes are always enabled.
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
#endif
	QApplication app(argc, argv);
	QCoreApplication::setApplicationName("datacorn");
	QCoreApplication::setApplicationVersion("0.9");

	QCommandLineParser parser;
	// NOTE: We recognize the CLA on all platforms, but only do something
	//       on Windows. Later we can see if this is useful in other OSs.
	QCommandLineOption systemThemeOption(
		QStringList() << "st" << "system-theme",
		QCoreApplication::translate("main", "Use system's theme"));
	parser.addOption(systemThemeOption);

	parser.process(app);
#ifdef Q_OS_WIN
	if(!parser.isSet(systemThemeOption))
	{
		app.setStyle("fusion");
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
		p.setColor(QPalette::Link, QColor(0, 0, 0));
		p.setColor(QPalette::Highlight, QColor(0, 0, 0));
		p.setColor(QPalette::HighlightedText, Qt::black);
		app.setPalette(p);
	}
#endif // Q_OS_WIN
	MainWindow w;
	for(auto const& arg : parser.positionalArguments())
		w.openDatabaseWithFile(arg);
	w.show();
	return QApplication::exec();
}
