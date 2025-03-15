/********************************************************************************
** Form generated from reading UI file 'database_editor_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.16
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DATABASE_EDITOR_WIDGET_H
#define UI_DATABASE_EDITOR_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "trim_on_paste_line_edit.hpp"

QT_BEGIN_NAMESPACE

class Ui_DatabaseEditorWidget
{
public:
    QHBoxLayout *horizontalLayout_7;
    QTableView *cardCodeNameList;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_5;
    TrimOnPasteLineEdit *nameLineEdit;
    QLabel *passLabel;
    TrimOnPasteLineEdit *passLineEdit;
    QLabel *aliasLabel;
    TrimOnPasteLineEdit *aliasLineEdit;
    QPlainTextEdit *descPlainTextEdit;
    QHBoxLayout *horizontalLayout_8;
    QGroupBox *scopesGroup;
    QVBoxLayout *verticalLayout;
    QListWidget *scopesWidget;
    QGroupBox *typesGroup;
    QVBoxLayout *verticalLayout_2;
    QListWidget *typesWidget;
    QGroupBox *racesGroup;
    QVBoxLayout *verticalLayout_3;
    QListWidget *racesWidget;
    QGroupBox *attributesGroup;
    QVBoxLayout *verticalLayout_4;
    QListWidget *attributesWidget;
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout_6;
    QFrame *frame1;
    QHBoxLayout *horizontalLayout_2;
    QLabel *levelLabel;
    QSpinBox *levelSpinBox;
    QFrame *frame2;
    QHBoxLayout *horizontalLayout_3;
    QSpinBox *atkSpinBox;
    QVBoxLayout *verticalLayout_7;
    QFrame *frame3;
    QHBoxLayout *horizontalLayout;
    QLabel *lScaleLabel;
    QSpinBox *lScaleSpinBox;
    QSpinBox *rScaleSpinBox;
    QLabel *rScaleLabel;
    QFrame *frame4;
    QHBoxLayout *horizontalLayout_4;
    QSpinBox *defSpinBox;
    QFrame *frame;
    QGridLayout *gridLayout_5;
    QToolButton *markerTopLeftButton;
    QToolButton *markerTopButton;
    QToolButton *markerTopRightButton;
    QToolButton *markerLeftButton;
    QToolButton *markerRightButton;
    QToolButton *markerBottomLeftButton;
    QToolButton *markerBottomButton;
    QToolButton *markerBottomRightButton;
    QHBoxLayout *horizontalLayout_6;
    QTableWidget *stringsTableWidget;
    QGroupBox *archeGroup;
    QGridLayout *gridLayout;
    QPushButton *addArcheButton;
    QComboBox *archeComboBox;
    QVBoxLayout *verticalLayout_13;
    QPushButton *removeArcheButton;
    QSpacerItem *verticalSpacer;
    QListWidget *archeList;
    QListWidget *categoriesWidget;

    void setupUi(QWidget *DatabaseEditorWidget)
    {
        if (DatabaseEditorWidget->objectName().isEmpty())
            DatabaseEditorWidget->setObjectName(QString::fromUtf8("DatabaseEditorWidget"));
        DatabaseEditorWidget->resize(1050, 878);
        DatabaseEditorWidget->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 14pt \"Sans Serif\"\n"
""));
        horizontalLayout_7 = new QHBoxLayout(DatabaseEditorWidget);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        cardCodeNameList = new QTableView(DatabaseEditorWidget);
        cardCodeNameList->setObjectName(QString::fromUtf8("cardCodeNameList"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(cardCodeNameList->sizePolicy().hasHeightForWidth());
        cardCodeNameList->setSizePolicy(sizePolicy);
        cardCodeNameList->setMinimumSize(QSize(0, 50));
        cardCodeNameList->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 61, 61);\n"
"font: 12pt \"Sans Serif\";"));
        cardCodeNameList->setAutoScroll(false);
        cardCodeNameList->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        cardCodeNameList->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        cardCodeNameList->verticalHeader()->setVisible(false);

        horizontalLayout_7->addWidget(cardCodeNameList);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        nameLineEdit = new TrimOnPasteLineEdit(DatabaseEditorWidget);
        nameLineEdit->setObjectName(QString::fromUtf8("nameLineEdit"));
        QFont font;
        font.setFamily(QString::fromUtf8("Sans Serif"));
        font.setPointSize(12);
        font.setBold(false);
        font.setItalic(false);
        nameLineEdit->setFont(font);
        nameLineEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 61, 61);\n"
"font: 12pt \"Sans Serif\";"));

        horizontalLayout_5->addWidget(nameLineEdit);

        passLabel = new QLabel(DatabaseEditorWidget);
        passLabel->setObjectName(QString::fromUtf8("passLabel"));
        passLabel->setFont(font);
        passLabel->setStyleSheet(QString::fromUtf8("background-color: rgb(81, 81, 83);\n"
"font: 12pt \"Sans Serif\";"));

        horizontalLayout_5->addWidget(passLabel);

        passLineEdit = new TrimOnPasteLineEdit(DatabaseEditorWidget);
        passLineEdit->setObjectName(QString::fromUtf8("passLineEdit"));
        passLineEdit->setFont(font);
        passLineEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 61, 61);\n"
"font: 12pt \"Sans Serif\";"));
        passLineEdit->setText(QString::fromUtf8("0"));
        passLineEdit->setMaxLength(10);

        horizontalLayout_5->addWidget(passLineEdit);

        aliasLabel = new QLabel(DatabaseEditorWidget);
        aliasLabel->setObjectName(QString::fromUtf8("aliasLabel"));
        aliasLabel->setFont(font);
        aliasLabel->setStyleSheet(QString::fromUtf8("background-color: rgb(81, 81, 83);\n"
"font: 12pt \"Sans Serif\";"));

        horizontalLayout_5->addWidget(aliasLabel);

        aliasLineEdit = new TrimOnPasteLineEdit(DatabaseEditorWidget);
        aliasLineEdit->setObjectName(QString::fromUtf8("aliasLineEdit"));
        aliasLineEdit->setFont(font);
        aliasLineEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 61, 61);\n"
"font: 12pt \"Sans Serif\";"));
        aliasLineEdit->setText(QString::fromUtf8("0"));
        aliasLineEdit->setMaxLength(10);

        horizontalLayout_5->addWidget(aliasLineEdit);


        verticalLayout_5->addLayout(horizontalLayout_5);

        descPlainTextEdit = new QPlainTextEdit(DatabaseEditorWidget);
        descPlainTextEdit->setObjectName(QString::fromUtf8("descPlainTextEdit"));
        descPlainTextEdit->setMinimumSize(QSize(0, 400));
        descPlainTextEdit->setFont(font);
        descPlainTextEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 61, 61);\n"
"font: 12pt \"Sans Serif\";"));

        verticalLayout_5->addWidget(descPlainTextEdit);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        scopesGroup = new QGroupBox(DatabaseEditorWidget);
        scopesGroup->setObjectName(QString::fromUtf8("scopesGroup"));
        scopesGroup->setMinimumSize(QSize(100, 100));
        scopesGroup->setStyleSheet(QString::fromUtf8("background-color: rgb(81, 81, 83);\n"
"font: 12pt \"Sans Serif\";"));
        verticalLayout = new QVBoxLayout(scopesGroup);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        scopesWidget = new QListWidget(scopesGroup);
        scopesWidget->setObjectName(QString::fromUtf8("scopesWidget"));
        scopesWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 61, 61);\n"
"font: 12pt \"Sans Serif\";"));

        verticalLayout->addWidget(scopesWidget);


        horizontalLayout_8->addWidget(scopesGroup);

        typesGroup = new QGroupBox(DatabaseEditorWidget);
        typesGroup->setObjectName(QString::fromUtf8("typesGroup"));
        typesGroup->setMinimumSize(QSize(100, 100));
        typesGroup->setStyleSheet(QString::fromUtf8("background-color: rgb(81, 81, 83);\n"
"font: 12pt \"Sans Serif\";"));
        verticalLayout_2 = new QVBoxLayout(typesGroup);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        typesWidget = new QListWidget(typesGroup);
        typesWidget->setObjectName(QString::fromUtf8("typesWidget"));
        typesWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 61, 61);\n"
"font: 12pt \"Sans Serif\";"));

        verticalLayout_2->addWidget(typesWidget);


        horizontalLayout_8->addWidget(typesGroup);

        racesGroup = new QGroupBox(DatabaseEditorWidget);
        racesGroup->setObjectName(QString::fromUtf8("racesGroup"));
        racesGroup->setMinimumSize(QSize(100, 100));
        racesGroup->setStyleSheet(QString::fromUtf8("background-color: rgb(81, 81, 83);\n"
"font: 12pt \"Sans Serif\";"));
        verticalLayout_3 = new QVBoxLayout(racesGroup);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        racesWidget = new QListWidget(racesGroup);
        racesWidget->setObjectName(QString::fromUtf8("racesWidget"));
        racesWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 61, 61);\n"
"font: 12pt \"Sans Serif\";"));

        verticalLayout_3->addWidget(racesWidget);


        horizontalLayout_8->addWidget(racesGroup);

        attributesGroup = new QGroupBox(DatabaseEditorWidget);
        attributesGroup->setObjectName(QString::fromUtf8("attributesGroup"));
        attributesGroup->setMinimumSize(QSize(100, 100));
        attributesGroup->setStyleSheet(QString::fromUtf8("background-color: rgb(81, 81, 83);\n"
"font: 12pt \"Sans Serif\";"));
        verticalLayout_4 = new QVBoxLayout(attributesGroup);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        attributesWidget = new QListWidget(attributesGroup);
        attributesWidget->setObjectName(QString::fromUtf8("attributesWidget"));
        attributesWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 61, 61);\n"
"font: 12pt \"Sans Serif\";"));

        verticalLayout_4->addWidget(attributesWidget);


        horizontalLayout_8->addWidget(attributesGroup);


        verticalLayout_5->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(0);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        frame1 = new QFrame(DatabaseEditorWidget);
        frame1->setObjectName(QString::fromUtf8("frame1"));
        frame1->setStyleSheet(QString::fromUtf8("background-color: rgb(81, 81, 83);\n"
"font: 12pt \"Sans Serif\";"));
        horizontalLayout_2 = new QHBoxLayout(frame1);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        levelLabel = new QLabel(frame1);
        levelLabel->setObjectName(QString::fromUtf8("levelLabel"));
        levelLabel->setStyleSheet(QString::fromUtf8("background-color: rgb(81, 81, 83);\n"
"font: 12pt \"Sans Serif\";"));

        horizontalLayout_2->addWidget(levelLabel);

        levelSpinBox = new QSpinBox(frame1);
        levelSpinBox->setObjectName(QString::fromUtf8("levelSpinBox"));
        levelSpinBox->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 61, 61);\n"
"font: 12pt \"Sans Serif\";"));
        levelSpinBox->setMaximum(12);

        horizontalLayout_2->addWidget(levelSpinBox);


        verticalLayout_6->addWidget(frame1);

        frame2 = new QFrame(DatabaseEditorWidget);
        frame2->setObjectName(QString::fromUtf8("frame2"));
        frame2->setStyleSheet(QString::fromUtf8("background-color: rgb(81, 81, 83);\n"
"font: 12pt \"Sans Serif\";"));
        horizontalLayout_3 = new QHBoxLayout(frame2);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        atkSpinBox = new QSpinBox(frame2);
        atkSpinBox->setObjectName(QString::fromUtf8("atkSpinBox"));
        atkSpinBox->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 61, 61);\n"
"font: 12pt \"Sans Serif\";"));
        atkSpinBox->setMinimum(-1);
        atkSpinBox->setMaximum(9999);
        atkSpinBox->setSingleStep(10);
        atkSpinBox->setValue(-1);

        horizontalLayout_3->addWidget(atkSpinBox);


        verticalLayout_6->addWidget(frame2);


        horizontalLayout_9->addLayout(verticalLayout_6);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(0);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        frame3 = new QFrame(DatabaseEditorWidget);
        frame3->setObjectName(QString::fromUtf8("frame3"));
        frame3->setStyleSheet(QString::fromUtf8("background-color: rgb(81, 81, 83);\n"
"font: 12pt \"Sans Serif\";"));
        horizontalLayout = new QHBoxLayout(frame3);
        horizontalLayout->setSpacing(2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lScaleLabel = new QLabel(frame3);
        lScaleLabel->setObjectName(QString::fromUtf8("lScaleLabel"));
        lScaleLabel->setStyleSheet(QString::fromUtf8("background-color: rgb(81, 81, 83);\n"
"font: 12pt \"Sans Serif\";"));
        lScaleLabel->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout->addWidget(lScaleLabel);

        lScaleSpinBox = new QSpinBox(frame3);
        lScaleSpinBox->setObjectName(QString::fromUtf8("lScaleSpinBox"));
        lScaleSpinBox->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 61, 61);\n"
"font: 12pt \"Sans Serif\";"));

        horizontalLayout->addWidget(lScaleSpinBox);

        rScaleSpinBox = new QSpinBox(frame3);
        rScaleSpinBox->setObjectName(QString::fromUtf8("rScaleSpinBox"));
        rScaleSpinBox->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 61, 61);\n"
"font: 12pt \"Sans Serif\";"));

        horizontalLayout->addWidget(rScaleSpinBox);

        rScaleLabel = new QLabel(frame3);
        rScaleLabel->setObjectName(QString::fromUtf8("rScaleLabel"));
        rScaleLabel->setStyleSheet(QString::fromUtf8("background-color: rgb(81, 81, 83);\n"
"font: 12pt \"Sans Serif\";"));

        horizontalLayout->addWidget(rScaleLabel);


        verticalLayout_7->addWidget(frame3);

        frame4 = new QFrame(DatabaseEditorWidget);
        frame4->setObjectName(QString::fromUtf8("frame4"));
        frame4->setStyleSheet(QString::fromUtf8("background-color: rgb(81, 81, 83);\n"
"font: 12pt \"Sans Serif\";"));
        horizontalLayout_4 = new QHBoxLayout(frame4);
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        defSpinBox = new QSpinBox(frame4);
        defSpinBox->setObjectName(QString::fromUtf8("defSpinBox"));
        defSpinBox->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 61, 61);\n"
"font: 12pt \"Sans Serif\";"));
        defSpinBox->setMinimum(-1);
        defSpinBox->setMaximum(9999);
        defSpinBox->setSingleStep(10);
        defSpinBox->setValue(-1);

        horizontalLayout_4->addWidget(defSpinBox);


        verticalLayout_7->addWidget(frame4);


        horizontalLayout_9->addLayout(verticalLayout_7);

        frame = new QFrame(DatabaseEditorWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setStyleSheet(QString::fromUtf8("background-color: rgb(81, 81, 83);\n"
"font: 12pt \"Sans Serif\";"));
        gridLayout_5 = new QGridLayout(frame);
        gridLayout_5->setSpacing(0);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        markerTopLeftButton = new QToolButton(frame);
        markerTopLeftButton->setObjectName(QString::fromUtf8("markerTopLeftButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(markerTopLeftButton->sizePolicy().hasHeightForWidth());
        markerTopLeftButton->setSizePolicy(sizePolicy1);
        markerTopLeftButton->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 61, 61);\n"
"font: 12pt \"Sans Serif\";"));
        markerTopLeftButton->setText(QString::fromUtf8("\342\206\226"));
        markerTopLeftButton->setCheckable(true);
        markerTopLeftButton->setChecked(false);

        gridLayout_5->addWidget(markerTopLeftButton, 0, 0, 1, 1);

        markerTopButton = new QToolButton(frame);
        markerTopButton->setObjectName(QString::fromUtf8("markerTopButton"));
        sizePolicy1.setHeightForWidth(markerTopButton->sizePolicy().hasHeightForWidth());
        markerTopButton->setSizePolicy(sizePolicy1);
        markerTopButton->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 61, 61);\n"
"font: 12pt \"Sans Serif\";"));
        markerTopButton->setText(QString::fromUtf8("\342\206\221"));
        markerTopButton->setCheckable(true);
        markerTopButton->setChecked(false);

        gridLayout_5->addWidget(markerTopButton, 0, 1, 1, 1);

        markerTopRightButton = new QToolButton(frame);
        markerTopRightButton->setObjectName(QString::fromUtf8("markerTopRightButton"));
        sizePolicy1.setHeightForWidth(markerTopRightButton->sizePolicy().hasHeightForWidth());
        markerTopRightButton->setSizePolicy(sizePolicy1);
        markerTopRightButton->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 61, 61);\n"
"font: 12pt \"Sans Serif\";"));
        markerTopRightButton->setText(QString::fromUtf8("\342\206\227"));
        markerTopRightButton->setCheckable(true);
        markerTopRightButton->setChecked(false);

        gridLayout_5->addWidget(markerTopRightButton, 0, 2, 1, 1);

        markerLeftButton = new QToolButton(frame);
        markerLeftButton->setObjectName(QString::fromUtf8("markerLeftButton"));
        sizePolicy1.setHeightForWidth(markerLeftButton->sizePolicy().hasHeightForWidth());
        markerLeftButton->setSizePolicy(sizePolicy1);
        markerLeftButton->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 61, 61);\n"
"font: 12pt \"Sans Serif\";"));
        markerLeftButton->setText(QString::fromUtf8("\342\206\220"));
        markerLeftButton->setCheckable(true);
        markerLeftButton->setChecked(false);

        gridLayout_5->addWidget(markerLeftButton, 1, 0, 1, 1);

        markerRightButton = new QToolButton(frame);
        markerRightButton->setObjectName(QString::fromUtf8("markerRightButton"));
        sizePolicy1.setHeightForWidth(markerRightButton->sizePolicy().hasHeightForWidth());
        markerRightButton->setSizePolicy(sizePolicy1);
        markerRightButton->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 61, 61);\n"
"font: 12pt \"Sans Serif\";"));
        markerRightButton->setText(QString::fromUtf8("\342\206\222"));
        markerRightButton->setCheckable(true);
        markerRightButton->setChecked(false);

        gridLayout_5->addWidget(markerRightButton, 1, 2, 1, 1);

        markerBottomLeftButton = new QToolButton(frame);
        markerBottomLeftButton->setObjectName(QString::fromUtf8("markerBottomLeftButton"));
        sizePolicy1.setHeightForWidth(markerBottomLeftButton->sizePolicy().hasHeightForWidth());
        markerBottomLeftButton->setSizePolicy(sizePolicy1);
        markerBottomLeftButton->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 61, 61);\n"
"font: 12pt \"Sans Serif\";"));
        markerBottomLeftButton->setText(QString::fromUtf8("\342\206\231"));
        markerBottomLeftButton->setCheckable(true);
        markerBottomLeftButton->setChecked(false);

        gridLayout_5->addWidget(markerBottomLeftButton, 2, 0, 1, 1);

        markerBottomButton = new QToolButton(frame);
        markerBottomButton->setObjectName(QString::fromUtf8("markerBottomButton"));
        sizePolicy1.setHeightForWidth(markerBottomButton->sizePolicy().hasHeightForWidth());
        markerBottomButton->setSizePolicy(sizePolicy1);
        markerBottomButton->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 61, 61);\n"
"font: 12pt \"Sans Serif\";"));
        markerBottomButton->setText(QString::fromUtf8("\342\206\223"));
        markerBottomButton->setCheckable(true);
        markerBottomButton->setChecked(false);

        gridLayout_5->addWidget(markerBottomButton, 2, 1, 1, 1);

        markerBottomRightButton = new QToolButton(frame);
        markerBottomRightButton->setObjectName(QString::fromUtf8("markerBottomRightButton"));
        sizePolicy1.setHeightForWidth(markerBottomRightButton->sizePolicy().hasHeightForWidth());
        markerBottomRightButton->setSizePolicy(sizePolicy1);
        markerBottomRightButton->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 61, 61);\n"
"font: 12pt \"Sans Serif\";"));
        markerBottomRightButton->setText(QString::fromUtf8("\342\206\230"));
        markerBottomRightButton->setCheckable(true);
        markerBottomRightButton->setChecked(false);

        gridLayout_5->addWidget(markerBottomRightButton, 2, 2, 1, 1);


        horizontalLayout_9->addWidget(frame);


        verticalLayout_5->addLayout(horizontalLayout_9);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        stringsTableWidget = new QTableWidget(DatabaseEditorWidget);
        if (stringsTableWidget->columnCount() < 1)
            stringsTableWidget->setColumnCount(1);
        if (stringsTableWidget->rowCount() < 16)
            stringsTableWidget->setRowCount(16);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        stringsTableWidget->setItem(0, 0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        stringsTableWidget->setItem(1, 0, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        stringsTableWidget->setItem(2, 0, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        stringsTableWidget->setItem(3, 0, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        stringsTableWidget->setItem(4, 0, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        stringsTableWidget->setItem(5, 0, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        stringsTableWidget->setItem(6, 0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        stringsTableWidget->setItem(7, 0, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        stringsTableWidget->setItem(8, 0, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        stringsTableWidget->setItem(9, 0, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        stringsTableWidget->setItem(10, 0, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        stringsTableWidget->setItem(11, 0, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        stringsTableWidget->setItem(12, 0, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        stringsTableWidget->setItem(13, 0, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        stringsTableWidget->setItem(14, 0, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        stringsTableWidget->setItem(15, 0, __qtablewidgetitem15);
        stringsTableWidget->setObjectName(QString::fromUtf8("stringsTableWidget"));
        stringsTableWidget->setMinimumSize(QSize(400, 0));
        stringsTableWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 61, 61);\n"
"font: 12pt \"Sans Serif\";"));
        stringsTableWidget->setRowCount(16);
        stringsTableWidget->setColumnCount(1);
        stringsTableWidget->horizontalHeader()->setVisible(false);
        stringsTableWidget->horizontalHeader()->setMinimumSectionSize(124);
        stringsTableWidget->horizontalHeader()->setStretchLastSection(true);

        horizontalLayout_6->addWidget(stringsTableWidget);

        archeGroup = new QGroupBox(DatabaseEditorWidget);
        archeGroup->setObjectName(QString::fromUtf8("archeGroup"));
        archeGroup->setMaximumSize(QSize(300, 300));
        archeGroup->setStyleSheet(QString::fromUtf8("background-color: rgb(81, 81, 83);\n"
"font: 12pt \"Sans Serif\";"));
        gridLayout = new QGridLayout(archeGroup);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        addArcheButton = new QPushButton(archeGroup);
        addArcheButton->setObjectName(QString::fromUtf8("addArcheButton"));
        addArcheButton->setEnabled(false);
        addArcheButton->setMaximumSize(QSize(40, 40));
        addArcheButton->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 61, 61);\n"
"font: 12pt \"Sans Serif\";"));
        addArcheButton->setText(QString::fromUtf8("+"));

        gridLayout->addWidget(addArcheButton, 0, 2, 1, 1);

        archeComboBox = new QComboBox(archeGroup);
        archeComboBox->setObjectName(QString::fromUtf8("archeComboBox"));
        archeComboBox->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 61, 61);\n"
"font: 12pt \"Sans Serif\";"));
        archeComboBox->setEditable(true);
        archeComboBox->setInsertPolicy(QComboBox::InsertPolicy::NoInsert);

        gridLayout->addWidget(archeComboBox, 0, 0, 1, 1);

        verticalLayout_13 = new QVBoxLayout();
        verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
        removeArcheButton = new QPushButton(archeGroup);
        removeArcheButton->setObjectName(QString::fromUtf8("removeArcheButton"));
        removeArcheButton->setEnabled(false);
        removeArcheButton->setMaximumSize(QSize(40, 40));
        removeArcheButton->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 61, 61);\n"
"font: 12pt \"Sans Serif\";"));
        removeArcheButton->setText(QString::fromUtf8("-"));

        verticalLayout_13->addWidget(removeArcheButton);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_13->addItem(verticalSpacer);


        gridLayout->addLayout(verticalLayout_13, 1, 2, 1, 1);

        archeList = new QListWidget(archeGroup);
        archeList->setObjectName(QString::fromUtf8("archeList"));
        archeList->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 61, 61);\n"
"font: 12pt \"Sans Serif\";"));

        gridLayout->addWidget(archeList, 1, 0, 1, 1);


        horizontalLayout_6->addWidget(archeGroup);

        categoriesWidget = new QListWidget(DatabaseEditorWidget);
        categoriesWidget->setObjectName(QString::fromUtf8("categoriesWidget"));
        categoriesWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 61, 61);\n"
"font: 12pt \"Sans Serif\";"));
        categoriesWidget->setResizeMode(QListView::ResizeMode::Adjust);
        categoriesWidget->setViewMode(QListView::ViewMode::IconMode);

        horizontalLayout_6->addWidget(categoriesWidget);


        verticalLayout_5->addLayout(horizontalLayout_6);


        horizontalLayout_7->addLayout(verticalLayout_5);


        retranslateUi(DatabaseEditorWidget);

        QMetaObject::connectSlotsByName(DatabaseEditorWidget);
    } // setupUi

    void retranslateUi(QWidget *DatabaseEditorWidget)
    {
        DatabaseEditorWidget->setWindowTitle(QCoreApplication::translate("DatabaseEditorWidget", "Form", nullptr));
        nameLineEdit->setPlaceholderText(QCoreApplication::translate("DatabaseEditorWidget", "Name", nullptr));
        passLabel->setText(QCoreApplication::translate("DatabaseEditorWidget", "passcode", nullptr));
        passLineEdit->setInputMask(QString());
        aliasLabel->setText(QCoreApplication::translate("DatabaseEditorWidget", "alias", nullptr));
        aliasLineEdit->setInputMask(QString());
        descPlainTextEdit->setPlaceholderText(QCoreApplication::translate("DatabaseEditorWidget", "Card Text", nullptr));
        scopesGroup->setTitle(QCoreApplication::translate("DatabaseEditorWidget", "Scope", nullptr));
        typesGroup->setTitle(QCoreApplication::translate("DatabaseEditorWidget", "Card Type", nullptr));
        racesGroup->setTitle(QCoreApplication::translate("DatabaseEditorWidget", "Monster Type", nullptr));
        attributesGroup->setTitle(QCoreApplication::translate("DatabaseEditorWidget", "Attribute", nullptr));
        levelLabel->setText(QCoreApplication::translate("DatabaseEditorWidget", "Level/Rank", nullptr));
        atkSpinBox->setSpecialValueText(QCoreApplication::translate("DatabaseEditorWidget", "ATK/?", nullptr));
        atkSpinBox->setPrefix(QCoreApplication::translate("DatabaseEditorWidget", "ATK/", nullptr));
        lScaleLabel->setText(QCoreApplication::translate("DatabaseEditorWidget", "L", nullptr));
        rScaleSpinBox->setSuffix(QString());
        rScaleLabel->setText(QCoreApplication::translate("DatabaseEditorWidget", "R", nullptr));
        defSpinBox->setSpecialValueText(QCoreApplication::translate("DatabaseEditorWidget", "DEF/?", nullptr));
        defSpinBox->setPrefix(QCoreApplication::translate("DatabaseEditorWidget", "DEF/", nullptr));

        const bool __sortingEnabled = stringsTableWidget->isSortingEnabled();
        stringsTableWidget->setSortingEnabled(false);
        stringsTableWidget->setSortingEnabled(__sortingEnabled);

        archeGroup->setTitle(QCoreApplication::translate("DatabaseEditorWidget", "Archetype / Setcode", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DatabaseEditorWidget: public Ui_DatabaseEditorWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATABASE_EDITOR_WIDGET_H
