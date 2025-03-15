/********************************************************************************
** Form generated from reading UI file 'main_window.ui'
**
** Created by: Qt User Interface Compiler version 5.15.16
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_WINDOW_H
#define UI_MAIN_WINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNewDatabase;
    QAction *actionOpenDatabase;
    QAction *actionHomepage;
    QAction *actionAbout;
    QAction *actionCloseDatabase;
    QAction *actionSaveData;
    QAction *actionExit;
    QAction *actionNewCard;
    QAction *actionDeleteData;
    QAction *actionCopySelectedCards;
    QAction *actionPasteClipboardCards;
    QAction *actionShowClipboardDatabase;
    QAction *actionCutSelectedCards;
    QAction *actionDeleteSelectedCards;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_7;
    QTabWidget *dbEditorTabsWidget;
    QMenuBar *menuBar;
    QMenu *menuDatabase;
    QMenu *menuHelp;
    QMenu *menuCard;
    QMenu *menuAction;
    QMenu *menuLanguage;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(798, 716);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        actionNewDatabase = new QAction(MainWindow);
        actionNewDatabase->setObjectName(QString::fromUtf8("actionNewDatabase"));
        actionOpenDatabase = new QAction(MainWindow);
        actionOpenDatabase->setObjectName(QString::fromUtf8("actionOpenDatabase"));
        actionHomepage = new QAction(MainWindow);
        actionHomepage->setObjectName(QString::fromUtf8("actionHomepage"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionAbout->setMenuRole(QAction::MenuRole::AboutRole);
        actionCloseDatabase = new QAction(MainWindow);
        actionCloseDatabase->setObjectName(QString::fromUtf8("actionCloseDatabase"));
        actionCloseDatabase->setEnabled(false);
        actionSaveData = new QAction(MainWindow);
        actionSaveData->setObjectName(QString::fromUtf8("actionSaveData"));
        actionSaveData->setEnabled(false);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionNewCard = new QAction(MainWindow);
        actionNewCard->setObjectName(QString::fromUtf8("actionNewCard"));
        actionNewCard->setEnabled(false);
        actionDeleteData = new QAction(MainWindow);
        actionDeleteData->setObjectName(QString::fromUtf8("actionDeleteData"));
        actionDeleteData->setEnabled(false);
        actionCopySelectedCards = new QAction(MainWindow);
        actionCopySelectedCards->setObjectName(QString::fromUtf8("actionCopySelectedCards"));
        actionCopySelectedCards->setEnabled(false);
        actionPasteClipboardCards = new QAction(MainWindow);
        actionPasteClipboardCards->setObjectName(QString::fromUtf8("actionPasteClipboardCards"));
        actionPasteClipboardCards->setEnabled(false);
        actionShowClipboardDatabase = new QAction(MainWindow);
        actionShowClipboardDatabase->setObjectName(QString::fromUtf8("actionShowClipboardDatabase"));
        actionCutSelectedCards = new QAction(MainWindow);
        actionCutSelectedCards->setObjectName(QString::fromUtf8("actionCutSelectedCards"));
        actionCutSelectedCards->setEnabled(false);
        actionDeleteSelectedCards = new QAction(MainWindow);
        actionDeleteSelectedCards->setObjectName(QString::fromUtf8("actionDeleteSelectedCards"));
        actionDeleteSelectedCards->setEnabled(false);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->setEnabled(true);
        horizontalLayout_7 = new QHBoxLayout(centralWidget);
        horizontalLayout_7->setSpacing(1);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(1, 1, 1, 1);
        dbEditorTabsWidget = new QTabWidget(centralWidget);
        dbEditorTabsWidget->setObjectName(QString::fromUtf8("dbEditorTabsWidget"));
        dbEditorTabsWidget->setAcceptDrops(true);
        dbEditorTabsWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(81, 81, 83);"));
        dbEditorTabsWidget->setTabsClosable(true);
        dbEditorTabsWidget->setMovable(true);

        horizontalLayout_7->addWidget(dbEditorTabsWidget);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 798, 22));
        menuDatabase = new QMenu(menuBar);
        menuDatabase->setObjectName(QString::fromUtf8("menuDatabase"));
        menuDatabase->setEnabled(true);
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuCard = new QMenu(menuBar);
        menuCard->setObjectName(QString::fromUtf8("menuCard"));
        menuAction = new QMenu(menuBar);
        menuAction->setObjectName(QString::fromUtf8("menuAction"));
        menuLanguage = new QMenu(menuBar);
        menuLanguage->setObjectName(QString::fromUtf8("menuLanguage"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuDatabase->menuAction());
        menuBar->addAction(menuCard->menuAction());
        menuBar->addAction(menuAction->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuBar->addAction(menuLanguage->menuAction());
        menuDatabase->addAction(actionNewDatabase);
        menuDatabase->addAction(actionOpenDatabase);
        menuDatabase->addAction(actionShowClipboardDatabase);
        menuDatabase->addAction(actionCloseDatabase);
        menuDatabase->addSeparator();
        menuDatabase->addAction(actionExit);
        menuHelp->addAction(actionHomepage);
        menuCard->addAction(actionNewCard);
        menuCard->addAction(actionSaveData);
        menuAction->addAction(actionCutSelectedCards);
        menuAction->addAction(actionCopySelectedCards);
        menuAction->addAction(actionPasteClipboardCards);
        menuAction->addAction(actionDeleteSelectedCards);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Datacorn", nullptr));
        actionNewDatabase->setText(QCoreApplication::translate("MainWindow", "&New Database...", nullptr));
#if QT_CONFIG(shortcut)
        actionNewDatabase->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+N", nullptr));
#endif // QT_CONFIG(shortcut)
        actionOpenDatabase->setText(QCoreApplication::translate("MainWindow", "&Open Database...", nullptr));
#if QT_CONFIG(shortcut)
        actionOpenDatabase->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionHomepage->setText(QCoreApplication::translate("MainWindow", "&Homepage", nullptr));
        actionAbout->setText(QCoreApplication::translate("MainWindow", "About", nullptr));
        actionCloseDatabase->setText(QCoreApplication::translate("MainWindow", "&Close Database", nullptr));
#if QT_CONFIG(shortcut)
        actionCloseDatabase->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+W", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSaveData->setText(QCoreApplication::translate("MainWindow", "&Save Data", nullptr));
#if QT_CONFIG(shortcut)
        actionSaveData->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionExit->setText(QCoreApplication::translate("MainWindow", "&Exit", nullptr));
#if QT_CONFIG(shortcut)
        actionExit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        actionNewCard->setText(QCoreApplication::translate("MainWindow", "&New Card...", nullptr));
#if QT_CONFIG(shortcut)
        actionNewCard->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+N", nullptr));
#endif // QT_CONFIG(shortcut)
        actionDeleteData->setText(QCoreApplication::translate("MainWindow", "&Delete Data", nullptr));
#if QT_CONFIG(shortcut)
        actionDeleteData->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+D", nullptr));
#endif // QT_CONFIG(shortcut)
        actionCopySelectedCards->setText(QCoreApplication::translate("MainWindow", "&Copy Selected Cards", nullptr));
#if QT_CONFIG(shortcut)
        actionCopySelectedCards->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+C", nullptr));
#endif // QT_CONFIG(shortcut)
        actionPasteClipboardCards->setText(QCoreApplication::translate("MainWindow", "&Paste Clipboard Cards", nullptr));
#if QT_CONFIG(shortcut)
        actionPasteClipboardCards->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+V", nullptr));
#endif // QT_CONFIG(shortcut)
        actionShowClipboardDatabase->setText(QCoreApplication::translate("MainWindow", "&Show Clipboard Database", nullptr));
        actionCutSelectedCards->setText(QCoreApplication::translate("MainWindow", "Cut Selected Cards", nullptr));
#if QT_CONFIG(shortcut)
        actionCutSelectedCards->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+X", nullptr));
#endif // QT_CONFIG(shortcut)
        actionDeleteSelectedCards->setText(QCoreApplication::translate("MainWindow", "Delete Selected Cards", nullptr));
#if QT_CONFIG(shortcut)
        actionDeleteSelectedCards->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+D", nullptr));
#endif // QT_CONFIG(shortcut)
        menuDatabase->setTitle(QCoreApplication::translate("MainWindow", "&Database", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "&Help", nullptr));
        menuCard->setTitle(QCoreApplication::translate("MainWindow", "&Card", nullptr));
        menuAction->setTitle(QCoreApplication::translate("MainWindow", "&Edit", nullptr));
        menuLanguage->setTitle(QCoreApplication::translate("MainWindow", "&Language", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_WINDOW_H
