#include "new_card_dialog.hpp"

#include <QRegularExpressionValidator>

#include "ui_new_card_dialog.h"

NewCardDialog::NewCardDialog(QWidget* parent)
	: QDialog(parent), ui(std::make_unique<Ui::NewCardDialog>())
{
	ui->setupUi(this);
	ui->passLineEdit->setValidator(new QRegularExpressionValidator(
		QRegularExpression("[0-9]+"), ui->passLineEdit));
}

NewCardDialog::~NewCardDialog() = default;

NewCardDialog::Result NewCardDialog::display(QSqlDatabase& db, bool canCopy)
{
	ui->passLineEdit->setText("0");
	ui->copyDataCheckBox->setChecked(false);
	ui->copyDataCheckBox->setEnabled(canCopy);
	int const dr = exec();
	return {dr, ui->passLineEdit->text().toUInt(),
	        ui->copyDataCheckBox->isChecked()};
}
