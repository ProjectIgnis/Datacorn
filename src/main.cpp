#include <QApplication>
#include <QTranslator>

#include "gui/main_window.hpp"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
// 	a.setStyle("fusion");
// 	QPalette p;
// 	p.setColor(QPalette::Window, QColor(52, 52, 52));
// 	p.setColor(QPalette::WindowText, Qt::white);
// 	p.setColor(QPalette::Base, QColor(43, 43, 43));
// 	p.setColor(QPalette::AlternateBase, QColor(52, 52, 52));
// 	p.setColor(QPalette::ToolTipBase, Qt::black);
// 	p.setColor(QPalette::ToolTipText, Qt::white);
// 	p.setColor(QPalette::Text, Qt::white);
// 	p.setColor(QPalette::Button, QColor(52, 52, 52));
// 	p.setColor(QPalette::ButtonText, Qt::white);
// 	p.setColor(QPalette::BrightText, Qt::red);
// 	p.setColor(QPalette::Link, QColor(220, 206, 128));
// 	p.setColor(QPalette::Highlight, QColor(220, 206, 128));
// 	p.setColor(QPalette::HighlightedText, Qt::black);
// 	a.setPalette(p);
	MainWindow w;
	w.show();
	return a.exec();
}
