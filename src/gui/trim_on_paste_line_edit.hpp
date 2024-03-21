#ifndef GUI_PASSCODE_LINE_EDIT_HPP
#define GUI_PASSCODE_LINE_EDIT_HPP
#include <QApplication>
#include <QClipboard>
#include <QKeyEvent>
#include <QLineEdit>

class TrimOnPasteLineEdit final : public QLineEdit
{
	Q_OBJECT
public:
	TrimOnPasteLineEdit(QWidget* parent = nullptr) : QLineEdit(parent) {}

protected:
	void keyPressEvent(QKeyEvent* event) override
	{
		if(!event->matches(QKeySequence::Paste))
			return QLineEdit::keyPressEvent(event);
		event->accept();
		QClipboard* cb = QApplication::clipboard();
		auto clipboardText = cb->text().trimmed();
		if(clipboardText.size())
			insert(cb->text().trimmed());
	}
};

#endif // GUI_PASSCODE_LINE_EDIT_HPP
