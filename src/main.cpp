#include <QApplication>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QSettings>
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

	QSettings settings("Datacorn.ini", QSettings::IniFormat);

	QCommandLineParser parser;
	QCommandLineOption systemThemeOption(
		QStringList() << "st" << "system-theme",
		QCoreApplication::translate("main", "Use system's theme"));
	parser.addOption(systemThemeOption);

	parser.process(app);
#ifdef Q_OS_WIN
	auto use_system_theme =
		settings.value("system-theme", false).toString().toLower() == "true";
	if(parser.isSet(systemThemeOption) && !use_system_theme) {
		use_system_theme = true;
		settings.setValue("system-theme", use_system_theme);
		settings.sync();
	}
	if(!use_system_theme)
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
		p.setColor(QPalette::Link, QColor(220, 206, 128));
		p.setColor(QPalette::Highlight, QColor(220, 206, 128));
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
