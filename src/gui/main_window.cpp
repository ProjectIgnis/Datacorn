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
#include "sql_util.hpp"

namespace
{

QString const SQL_CLIPBOARD_CONN("CLIPBOARD");

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

QString const SQL_QUERY_DATA_AND_TEXT_LIST(R"(
SELECT datas.id,alias,setcode,type,atk,def,level,race,attribute,ot,category,
texts.id,name,desc,str1,str2,str3,str4,str5,str6,str7,str8,str9,str10,str11,str12,str13,str14,str15,str16
FROM datas, texts WHERE datas.id = texts.id AND texts.id IN (
)");
static constexpr int TEXT_INDEX_START = 11;

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
	connect(ui->actionShowClipboardDatabase, &QAction::triggered, this,
	        &MainWindow::showClipboardDatabase);
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
	connect(ui->actionCopySelectedCards, &QAction::triggered, this,
	        &MainWindow::copySelectedCards);
	connect(ui->actionPasteClipboardCards, &QAction::triggered, this,
	        &MainWindow::pasteClipboardCards);
	connect(ui->actionHomepage, &QAction::triggered, this,
	        &MainWindow::openHomepage);
	// Setup "Clipboard" database
	auto db = QSqlDatabase::addDatabase(SQL_DB_DRIVER, SQL_CLIPBOARD_CONN);
	db.setDatabaseName(":memory:");
	setupCleanDB(db);
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
	setupCleanDB(db);
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

void MainWindow::showClipboardDatabase()
{
	auto db = QSqlDatabase::database(SQL_CLIPBOARD_CONN, false);
	auto const ptr = db.password();
	if(!ptr.isEmpty())
	{
		ui->dbEditorTabsWidget->setCurrentWidget(&widgetFromConnection(SQL_CLIPBOARD_CONN));
		return;
	}
	// Create "Clipboard" view widget
	auto* newTab =
		new DatabaseEditorWidget(ui->dbEditorTabsWidget, SQL_CLIPBOARD_CONN);
	ui->dbEditorTabsWidget->setCurrentIndex(
		ui->dbEditorTabsWidget->addTab(newTab, tr("Clipboard")));
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
	if(dbConnection != SQL_CLIPBOARD_CONN) // Long live the "Clipboard" db!
		QSqlDatabase::removeDatabase(dbConnection);
	else
		QSqlDatabase::database(SQL_CLIPBOARD_CONN, false).setPassword("");
	enableEditing(ui->dbEditorTabsWidget->count() != 0);
}

void MainWindow::newCard()
{
	currentTab().newCard();
}

void MainWindow::saveData()
{
	currentTab().saveData();
}

void MainWindow::deleteData()
{
	currentTab().deleteData();
}

void MainWindow::copySelectedCards()
{
	auto& tab = currentTab();
	auto const codes = tab.selectedCards();
	if(codes.size() == 0)
		return;
	auto dbSrc = QSqlDatabase::database(tab.databaseConnection(), false);
	auto dbDst = QSqlDatabase::database(SQL_CLIPBOARD_CONN, false);
	dbDst.exec("DELETE FROM datas;");
	dbDst.exec("DELETE FROM texts;");
	copyCards(codes, dbSrc, dbDst);
	// TODO: Update target db's widget
}

void MainWindow::pasteClipboardCards()
{
	auto& tab = currentTab();
	auto const tabDbConnection = tab.databaseConnection();
	if(tabDbConnection == SQL_CLIPBOARD_CONN)
		return;
	auto dbSrc = QSqlDatabase::database(SQL_CLIPBOARD_CONN, false);
	auto dbDst = QSqlDatabase::database(tabDbConnection, false);
	// TODO: Confirm cards to be overwritten
	auto const codes = [&]() -> QVector<quint32>
	{
		QVector<quint32> ret;
		auto q = buildQuery(dbSrc, "SELECT id FROM datas;");
		execQuery(q);
		while(q.next())
			ret.append(q.value(0).toUInt());
		return ret;
	}();
	copyCards(codes, dbSrc, dbDst);
	// TODO: Update target db's widget
}

void MainWindow::openHomepage()
{
	QDesktopServices::openUrl(
		QUrl("https://www.youtube.com/watch?v=dQw4w9WgXcQ"));
}

// private

void MainWindow::copyCards(QVector<quint32> const& codes, QSqlDatabase& dbSrc,
                           QSqlDatabase& dbDst)
{
	auto const stmt = [&]() -> QString
	{
		QString ret(SQL_QUERY_DATA_AND_TEXT_LIST);
		for(auto const code : codes)
			ret.append(QString::number(code)).append(',');
		ret.append("0);");
		return ret;
	}();
	auto q1 = buildQuery(dbSrc, stmt);
	execQuery(q1);
	auto const recordCount = q1.record().count();
	auto q2 = buildQuery(
		dbDst, QString(SQL_INSERT_DATA).replace("INSERT", "INSERT OR REPLACE"));
	auto q3 = buildQuery(
		dbDst, QString(SQL_INSERT_TEXT).replace("INSERT", "INSERT OR REPLACE"));
	while(q1.next())
	{
		for(int i = 0; i < TEXT_INDEX_START; i++)
			q2.bindValue(i, q1.value(i));
		for(int i = 0; i < recordCount - TEXT_INDEX_START; i++)
			q3.bindValue(i, q1.value(TEXT_INDEX_START + i));
		execQuery(q2);
		execQuery(q3);
	}
}

DatabaseEditorWidget& MainWindow::currentTab() const
{
	Q_ASSERT(ui->dbEditorTabsWidget->count() > 0);
	return *static_cast<DatabaseEditorWidget*>(
		ui->dbEditorTabsWidget->currentWidget());
}

DatabaseEditorWidget& MainWindow::widgetFromConnection(
	QString const& dbConnection) const
{
	auto db = QSqlDatabase::database(dbConnection, false);
	Q_ASSERT(!db.password().isEmpty());
	bool ok;
	auto const ptr = db.password().toULongLong(&ok, 16);
	Q_ASSERT(ok);
	return *reinterpret_cast<DatabaseEditorWidget*>(ptr);
}

void MainWindow::setupCleanDB(QSqlDatabase& db) const
{
	bool const isDbOpen = db.open();
	Q_ASSERT(isDbOpen);
	db.exec(SQL_QUERY_CREATE_DATAS_TABLE);
	db.exec(SQL_QUERY_CREATE_TEXTS_TABLE);
}

void MainWindow::enableEditing(bool editing)
{
	ui->actionCloseDatabase->setEnabled(editing);
	ui->actionSaveData->setEnabled(editing);
	ui->actionNewCard->setEnabled(editing);
	ui->actionDeleteData->setEnabled(editing);
	ui->actionCopySelectedCards->setEnabled(editing);
	ui->actionPasteClipboardCards->setEnabled(editing);
}
