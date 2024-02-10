#include "confirm_dialog.hpp"

#include "ui_confirm_dialog.h"

ConfirmDialog::ConfirmDialog(QWidget* parent)
	: QDialog(parent), ui(std::make_unique<Ui::ConfirmDialog>())
{
	ui->setupUi(this);
}

ConfirmDialog::~ConfirmDialog() = default;

QDialog::DialogCode ConfirmDialog::display(QString const& title,
                                           QString const& question,
                                           QString const& cards)
{
	ConfirmDialog cd;
	cd.setWindowTitle(title);
	auto const cardCount = QString(" (%1)").arg(cards.count('\n') + 1);
	cd.ui->label->setText(question + cardCount);
	cd.ui->plainTextEdit->setPlainText(cards);
	return static_cast<DialogCode>(cd.exec());
}
