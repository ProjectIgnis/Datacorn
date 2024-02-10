#ifndef GUI_NEW_CARD_DIALOG_HPP
#define GUI_NEW_CARD_DIALOG_HPP
#include <QDialog>
#include <memory>

QT_BEGIN_NAMESPACE
class QSqlDatabase;
namespace Ui
{
class NewCardDialog;
}
QT_END_NAMESPACE

class NewCardDialog : public QDialog
{
	Q_OBJECT
public:
	struct Result
	{
		QDialog::DialogCode dialogResult;
		quint32 newCode;
		bool copy;
	};

	explicit NewCardDialog(QWidget* parent = nullptr);
	~NewCardDialog() override;

	static Result display(QSqlDatabase& db, bool canCopy);

private:
	std::unique_ptr<Ui::NewCardDialog> ui;
};

#endif // GUI_NEW_CARD_DIALOG_HPP
