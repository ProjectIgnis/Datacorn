#include "main_window.hpp"

#include <QDesktopServices> // openUrl
#include <QFileDialog>
#include <QMessageBox>
#include <QScreen>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QString>
#include <QTranslator>
#include <QUrl>
#include <array>
#include <ui_main_window.h>

#include "database_editor_widget.hpp"

namespace
{

QString const SQL_DB_DRIVER("QSQLITE");

std::array const SQL_DATAS_TABLE_FIELDS = {
	QString{"PRAGMA table_info('datas');"},
	QString{"alias0,atk0,attribute0,category0,def0,id1,level0,ot0,race0,"
            "setcode0,type0"},
};

std::array const SQL_TEXTS_TABLE_FIELDS = {
	QString{"PRAGMA table_info('texts');"},
	QString{"desc0,id1,name0,str10,str100,str110,str120,str130,str140,str150,"
            "str160,str20,str30,str40,str50,str60,str70,str80,str90"},
};

QString const SQL_QUERY_CREATE_DATAS_TABLE(R"(
CREATE TABLE "datas" (
    "id"        INTEGER,
    "ot"        INTEGER,
    "alias"     INTEGER,
    "setcode"   INTEGER,
    "type"      INTEGER,
    "atk"       INTEGER,
    "def"       INTEGER,
    "level"     INTEGER,
    "race"      INTEGER,
    "attribute" INTEGER,
    "category"  INTEGER,
    PRIMARY KEY("id")
)
)");

QString const SQL_QUERY_CREATE_TEXTS_TABLE(R"(
CREATE TABLE "texts" (
    "id"    INTEGER,
    "name"  TEXT,
    "desc"  TEXT,
    "str1"  TEXT,
    "str2"  TEXT,
    "str3"  TEXT,
    "str4"  TEXT,
    "str5"  TEXT,
    "str6"  TEXT,
    "str7"  TEXT,
    "str8"  TEXT,
    "str9"  TEXT,
    "str10" TEXT,
    "str11" TEXT,
    "str12" TEXT,
    "str13" TEXT,
    "str14" TEXT,
    "str15" TEXT,
    "str16" TEXT,
    PRIMARY KEY("id")
)
)");

} // namespace

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, spanishTranslator(std::make_unique<QTranslator>())
	, ui(std::make_unique<Ui::MainWindow>())
{
	(void)spanishTranslator->load(":/es");
	ui->setupUi(this);
#ifdef Q_OS_WIN
	setGeometry(
		QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(),
	                        QGuiApplication::primaryScreen()->geometry()));
#endif
	connect(ui->actionNewDatabase, &QAction::triggered, this,
	        &MainWindow::newDatabase);
	connect(ui->actionOpenDatabase, &QAction::triggered, this,
	        &MainWindow::openDatabase);
	connect(ui->actionCloseDatabase, &QAction::triggered,
	        [this]() { closeDatabase(-1); });
	connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
	connect(ui->dbEditorTabsWidget, &QTabWidget::tabCloseRequested, this,
	        &MainWindow::closeDatabase);
	connect(ui->actionNewCard, &QAction::triggered, this, &MainWindow::newCard);
	connect(ui->actionSaveData, &QAction::triggered, this,
	        &MainWindow::saveData);
	connect(ui->actionDeleteData, &QAction::triggered, this,
	        &MainWindow::deleteData);
	connect(ui->actionHomepage, &QAction::triggered, this,
	        &MainWindow::openHomepage);
}

MainWindow::~MainWindow()
{
	QApplication::removeTranslator(spanishTranslator.get());
}

void MainWindow::changeEvent(QEvent* event)
{
	if(event->type() == QEvent::LanguageChange)
	{
		ui->retranslateUi(this);
		// FIXME: Implement translating tabs
	}
	else
		QWidget::changeEvent(event);
}

// private slots

void MainWindow::newDatabase()
{
	const QString file = QFileDialog::getSaveFileName(
		this, tr("Save Database As"), ".",
		tr("YGOPro Database (*.cdb *.db *.sqlite)"));
	if(file.isEmpty())
		return;
	if(QSqlDatabase::contains(file))
	{
		// TODO: Completely override opened database instead of just selecting.
		ui->dbEditorTabsWidget->setCurrentWidget(&widgetFromConnection(file));
		return;
	}
	QFile::remove(file);
	auto db = QSqlDatabase::addDatabase(SQL_DB_DRIVER, file);
	db.setDatabaseName(file);
	bool const isDbOpen = db.open();
	Q_ASSERT(isDbOpen);
	db.exec(SQL_QUERY_CREATE_DATAS_TABLE);
	db.exec(SQL_QUERY_CREATE_TEXTS_TABLE);
	auto* newTab = new DatabaseEditorWidget(ui->dbEditorTabsWidget, file);
	ui->dbEditorTabsWidget->setCurrentIndex(
		ui->dbEditorTabsWidget->addTab(newTab, file.split('/').last()));
	enableEditing(true);
}

void MainWindow::openDatabase()
{
	auto does_db_have_correct_format = [&](QSqlDatabase& db)
	{
		auto verify_table = [&](const auto& query_and_result)
		{
			auto q = db.exec(query_and_result[0]);
			auto record = q.record();
			auto name_index = record.indexOf("name");
			auto pk_index = record.indexOf("pk");
			if(name_index == -1 || pk_index == -1)
				return false;
			QStringList columns;
			while(q.next())
				columns << q.value(name_index).toString() +
							   q.value(pk_index).toString();
			columns.sort(Qt::CaseInsensitive);
			return columns.join(',').compare(query_and_result[1],
			                                 Qt::CaseInsensitive) == 0;
		};
		return verify_table(SQL_DATAS_TABLE_FIELDS) &&
		       verify_table(SQL_TEXTS_TABLE_FIELDS);
	};
	QString const file = QFileDialog::getOpenFileName(
		this, tr("Select Database"), ".",
		tr("YGOPro Database (*.cdb *.db *.sqlite)"));
	if(file.isEmpty())
		return;
	if(QSqlDatabase::contains(file))
	{
		ui->dbEditorTabsWidget->setCurrentWidget(&widgetFromConnection(file));
		return;
	}
	auto db = QSqlDatabase::addDatabase(SQL_DB_DRIVER, file);
	db.setDatabaseName(file);
	if(!db.open())
	{
		QMessageBox::critical(this, tr("Error Opening Database"),
		                      db.lastError().text());
		QSqlDatabase::removeDatabase(file);
		return;
	}
	if(!does_db_have_correct_format(db))
	{
		QMessageBox::critical(
			this, tr("Error Opening Database"),
			tr("Selected file is not a proper YGOPRO database."));
		db.close();
		QSqlDatabase::removeDatabase(file);
		return;
	}
	auto* newTab = new DatabaseEditorWidget(ui->dbEditorTabsWidget, file);
	ui->dbEditorTabsWidget->setCurrentIndex(
		ui->dbEditorTabsWidget->addTab(newTab, file.split('/').last()));
	enableEditing(true);
}

void MainWindow::closeDatabase(int index)
{
	if(index < 0)
		index = ui->dbEditorTabsWidget->currentIndex();
	auto* tab = static_cast<DatabaseEditorWidget*>(
		ui->dbEditorTabsWidget->widget(index));
	auto const dbConnection = tab->databaseConnection();
	ui->dbEditorTabsWidget->removeTab(index);
	delete tab;
	QSqlDatabase::removeDatabase(dbConnection);
	enableEditing(ui->dbEditorTabsWidget->count() != 0);
}

void MainWindow::newCard()
{
	static_cast<DatabaseEditorWidget*>(ui->dbEditorTabsWidget->currentWidget())
		->newCard();
}

void MainWindow::saveData()
{
	static_cast<DatabaseEditorWidget*>(ui->dbEditorTabsWidget->currentWidget())
		->saveData();
}

void MainWindow::deleteData()
{
	static_cast<DatabaseEditorWidget*>(ui->dbEditorTabsWidget->currentWidget())
		->deleteData();
}

void MainWindow::openHomepage()
{
	QDesktopServices::openUrl(
		QUrl("https://www.youtube.com/watch?v=dQw4w9WgXcQ"));
}

// private

void MainWindow::enableEditing(bool editing)
{
	ui->actionSaveData->setEnabled(editing);
	ui->actionNewCard->setEnabled(editing);
	ui->actionDeleteData->setEnabled(editing);
	ui->actionCloseDatabase->setEnabled(editing);
}

DatabaseEditorWidget& MainWindow::widgetFromConnection(
	QString const& dbConnection)
{
	auto db = QSqlDatabase::database(dbConnection, false);
	Q_ASSERT(!db.password().isEmpty());
	bool ok;
	auto const ptr = db.password().toULongLong(&ok, 16);
	Q_ASSERT(ok);
	return *reinterpret_cast<DatabaseEditorWidget*>(ptr);
}
