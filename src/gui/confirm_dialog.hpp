#ifndef GUI_CONFIRM_DIALOG_HPP
#define GUI_CONFIRM_DIALOG_HPP
#include <QDialog>
#include <memory>

QT_BEGIN_NAMESPACE
class QString;
namespace Ui
{
class ConfirmDialog;
}
QT_END_NAMESPACE

class ConfirmDialog : public QDialog
{
	Q_OBJECT
public:
	explicit ConfirmDialog(QWidget* parent = nullptr);
	~ConfirmDialog() override;

	static DialogCode display(QString const& title, QString const& question,
							  QString const& cards);

private:
	std::unique_ptr<Ui::ConfirmDialog> ui;
};

#endif // GUI_CONFIRM_DIALOG_HPP
