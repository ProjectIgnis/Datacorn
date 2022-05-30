#include "main_window.hpp"

#include <QDateTime>
// #include <QDesktopServices> // openUrl
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTranslator>

#include "ui_main_window.h"

namespace
{

QString const SQL_DB_DRIVER("QSQLITE");

QString const SQL_QUERY_DATA(R"(
SELECT id,alias,setcode,type,atk,def,level,race,attribute,ot,category
FROM datas WHERE datas.id = ?;
)");

QString const SQL_QUERY_TEXT(R"(
SELECT name,desc,str1,str2,str3,str4,str5,str6,str7,str8,str9,str10,str11,str12,str13,str14,str15,str16
FROM texts WHERE texts.id = ?;
)");

}

// public

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	spanishTranslator(std::make_unique<QTranslator>()),
	ui(std::make_unique<Ui::MainWindow>())
{
	spanishTranslator->load(":/es");
	QApplication::instance()->installTranslator(spanishTranslator.get());
	ui->setupUi(this);
	// TODO: Create custom Validator that works with unsigned integers...
// 	ui->passLineEdit->setValidator(new QIntValidator(0, 4294967295, this));
// 	ui->aliasLineEdit->setValidator(new QIntValidator(0, 4294967295, this));
// 	connect(ui->actionNewDatabase, &QAction::triggered, this, &MainWindow::newDatabase);
	connect(ui->actionOpenDatabase, &QAction::triggered, this, &MainWindow::openDatabase);
	connect(ui->actionCloseDatabase, &QAction::triggered, this, &MainWindow::closeDatabase);
	connect(ui->actionSpanish, &QAction::triggered, this, &MainWindow::toSpanish);
	connect(ui->actionEnglish, &QAction::triggered, this, &MainWindow::toEnglish);
}

MainWindow::~MainWindow()
{
	QApplication::instance()->removeTranslator(spanishTranslator.get());
}

void MainWindow::changeEvent(QEvent *event)
{
	if (event->type() == QEvent::LanguageChange)
		ui->retranslateUi(this);
	else
		QWidget::changeEvent(event);
}

// private slots

void MainWindow::openDatabase()
{
	auto does_db_have_correct_format = [&](QSqlDatabase& db)
	{
		return true; // TODO
	};
	if(!checkAndAskToCloseDb())
		return;
	QString const file = QFileDialog::getOpenFileName
	(
		this,
		tr("Select Database"),
		".",
		tr("YGOPro Database (*.cdb *.db *.sqlite)")
	);
	if(file.isEmpty())
		return;
	auto db = QSqlDatabase::addDatabase(SQL_DB_DRIVER);
	db.setDatabaseName(file);
	if(!db.open())
	{
		QMessageBox::critical(this, tr("Error Opening Database"), db.lastError().text());
		closeDatabase();
		return;
	}
	if(!does_db_have_correct_format(db))
	{
		QMessageBox::critical(this, tr("Error Opening Database"), tr("Selected file is not a proper YGOPRO database."));
		closeDatabase();
		return;
	}
	updateUiWithCode(5043010); // TODO: Maybe load the first card on the db?
}

void MainWindow::closeDatabase()
{
	auto db = QSqlDatabase::database();
	if(db.isValid())
	{
		db.close();
		QSqlDatabase::removeDatabase(db.connectionName());
	}
	updateUiWithCode(0U);
}

void MainWindow::toSpanish()
{
	ui->actionSpanish->setEnabled(false);
	ui->actionEnglish->setEnabled(true);
	ui->actionEnglish->setChecked(false);
	QApplication::instance()->installTranslator(spanishTranslator.get());
}

void MainWindow::toEnglish()
{
	ui->actionEnglish->setEnabled(false);
	ui->actionSpanish->setEnabled(true);
	ui->actionSpanish->setChecked(false);
	QApplication::instance()->removeTranslator(spanishTranslator.get());
}

// private

bool MainWindow::checkAndAskToCloseDb()
{
	if(!QSqlDatabase::database().isValid())
		return true;
	if(QMessageBox::question(this, tr("Close Opened Database?"), tr("Do you wish to close the currently opened database?")) == QMessageBox::Yes)
	{
		closeDatabase();
		return true;
	}
	return false;
}

void MainWindow::updateUiWithCode(quint32 code)
{
	// Clean the UI first
	if(code == 0U) // If code is 0 then we don't fill in the info
		return;
	// Query the data
	auto db = QSqlDatabase::database();
	Q_ASSERT(db.isValid());
	QSqlQuery q1(db);
	q1.prepare(SQL_QUERY_DATA);
	q1.bindValue(0, code);
	bool const q1result = q1.exec() && q1.first();
	Q_ASSERT(q1result);
	QSqlQuery q2(db);
	q2.prepare(SQL_QUERY_TEXT);
	q2.bindValue(0, code);
	bool const q2result = q2.exec() && q2.first();
	Q_ASSERT(q2result);
	// Proceed to populate the fields with the new data
	ui->passLineEdit->setText(q1.value(0).toString());
	ui->aliasLineEdit->setText(q1.value(1).toString());
	// TODO: 2 - setcodes / archetypes
	constexpr quint32 TYPE_LINK = 0x4000000;
	quint32 const type = q1.value(3).toUInt();
	// TODO: 3 - type - checkboxes
	constexpr qint32 QMARK_ATK_DEF = -2;
	qint32 const atk = q1.value(4).toInt();
	ui->atkQmCheckBox->setChecked(atk == QMARK_ATK_DEF);
	ui->atkSpinBox->setEnabled(atk != QMARK_ATK_DEF);
	ui->atkSpinBox->setValue(std::max(atk, 0));
	if(qint32 const def = q1.value(5).toInt(); (type & TYPE_LINK) == 0U)
	{
		ui->defQmCheckBox->setChecked(def == QMARK_ATK_DEF);
		ui->defSpinBox->setEnabled(def != QMARK_ATK_DEF);
		ui->defSpinBox->setValue(std::max(def, 0));
	}
	else
	{
		ui->markerBottomLeftButton->setChecked((def & 0x1) != 0U);
		ui->markerBottomButton->setChecked((def & 0x2) != 0U);
		ui->markerBottomRightButton->setChecked((def & 0x4) != 0U);
		ui->markerLeftButton->setChecked((def & 0x8) != 0U);
		ui->markerRightButton->setChecked((def & 0x20) != 0U);
		ui->markerTopLeftButton->setChecked((def & 0x40) != 0U);
		ui->markerTopButton->setChecked((def & 0x80) != 0U);
		ui->markerTopRightButton->setChecked((def & 0x100) != 0U);
	}
	quint32 const dbLevel = q1.value(6).toUInt();
	ui->levelSpinBox->setValue(dbLevel & 0x800000FF);
	ui->lScaleSpinBox->setValue((dbLevel >> 24U) & 0xFF);
	ui->rScaleSpinBox->setValue((dbLevel >> 16U) & 0xFF);
	// TODO: 7 - race - checkboxes
	// TODO: 8 - attribute - checkboxes
	// TODO: 9 - ot/scope - checkboxes
	// TODO: 10 - category - checkboxes

	ui->nameLineEdit->setText(q2.value(0).toString());
	ui->descPlainTextEdit->setPlainText(q2.value(1).toString());
	int const count = ui->stringsTableWidget->rowCount();
	for(int i = 0; i < count; ++i)
	{
		auto& item = *ui->stringsTableWidget->item(i, 0);
		item.setText(q2.value(2 + i).toString());
	}
}

