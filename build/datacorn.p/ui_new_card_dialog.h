/********************************************************************************
** Form generated from reading UI file 'new_card_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.16
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEW_CARD_DIALOG_H
#define UI_NEW_CARD_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include "trim_on_paste_line_edit.hpp"

QT_BEGIN_NAMESPACE

class Ui_NewCardDialog
{
public:
    QFormLayout *formLayout;
    QLabel *passLabel;
    TrimOnPasteLineEdit *passLineEdit;
    QCheckBox *copyDataCheckBox;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *NewCardDialog)
    {
        if (NewCardDialog->objectName().isEmpty())
            NewCardDialog->setObjectName(QString::fromUtf8("NewCardDialog"));
        NewCardDialog->resize(362, 118);
        formLayout = new QFormLayout(NewCardDialog);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        passLabel = new QLabel(NewCardDialog);
        passLabel->setObjectName(QString::fromUtf8("passLabel"));
        QFont font;
        font.setPointSize(12);
        passLabel->setFont(font);

        formLayout->setWidget(0, QFormLayout::LabelRole, passLabel);

        passLineEdit = new TrimOnPasteLineEdit(NewCardDialog);
        passLineEdit->setObjectName(QString::fromUtf8("passLineEdit"));
        passLineEdit->setFont(font);
        passLineEdit->setText(QString::fromUtf8("0"));
        passLineEdit->setMaxLength(10);

        formLayout->setWidget(0, QFormLayout::FieldRole, passLineEdit);

        copyDataCheckBox = new QCheckBox(NewCardDialog);
        copyDataCheckBox->setObjectName(QString::fromUtf8("copyDataCheckBox"));
        copyDataCheckBox->setEnabled(false);

        formLayout->setWidget(1, QFormLayout::SpanningRole, copyDataCheckBox);

        buttonBox = new QDialogButtonBox(NewCardDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        formLayout->setWidget(2, QFormLayout::SpanningRole, buttonBox);


        retranslateUi(NewCardDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), NewCardDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), NewCardDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(NewCardDialog);
    } // setupUi

    void retranslateUi(QDialog *NewCardDialog)
    {
        NewCardDialog->setWindowTitle(QCoreApplication::translate("NewCardDialog", "Insert New Card", nullptr));
        passLabel->setText(QCoreApplication::translate("NewCardDialog", "passcode", nullptr));
        passLineEdit->setInputMask(QString());
        copyDataCheckBox->setText(QCoreApplication::translate("NewCardDialog", "copy currently selected data?", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NewCardDialog: public Ui_NewCardDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEW_CARD_DIALOG_H
