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
	NewCardDialog ncd;
	ncd.ui->passLineEdit->setText("0");
	ncd.ui->copyDataCheckBox->setChecked(false);
	ncd.ui->copyDataCheckBox->setEnabled(canCopy);
	auto const dr = static_cast<QDialog::DialogCode>(ncd.exec());
	return {dr, ncd.ui->passLineEdit->text().toUInt(),
	        ncd.ui->copyDataCheckBox->isChecked()};
}
