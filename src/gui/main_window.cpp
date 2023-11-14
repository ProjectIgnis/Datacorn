#include "main_window.hpp"

#include <QActionGroup>
#include <QDesktopServices> // openUrl
#include <QFileDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QScreen>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QString>
#include <QTabBar>
#include <QTranslator>
#include <QUrl>
#include <array>
#include <cstring> // std::memcpy
#include <ui_main_window.h>

#include "database_editor_widget.hpp"
#include "sql_util.hpp"

namespace
{

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

QString const SQL_QUERY_CODE_AND_NAME_LIST(R"(
SELECT id,name
FROM texts WHERE id IN (
)");

} // namespace

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, currentTranslator(nullptr)
	, ui(std::make_unique<Ui::MainWindow>())
{
	ui->setupUi(this);
#ifdef Q_OS_WIN
	setGeometry(
		QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(),
	                        QGuiApplication::primaryScreen()->geometry()));
#endif
	createLanguageMenu();
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
	// Allow filtering for middle mouse clicks to close tabs
	ui->dbEditorTabsWidget->installEventFilter(this);
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

	// TODO: Allow selecting the default locale via an option?
	loadLanguage("en");
}

MainWindow::~MainWindow()
{
	if(currentTranslator)
		QApplication::removeTranslator(currentTranslator.get());
}

void MainWindow::changeEvent(QEvent* event)
{
	switch(event->type())
	{
	case QEvent::LanguageChange:
		return ui->retranslateUi(this);
	case QEvent::LocaleChange:
	{
		QString locale = QLocale::system().name();
		locale.truncate(locale.lastIndexOf('_'));
		return loadLanguage(locale);
	}
	default:
		return QWidget::changeEvent(event);
	}
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	event->ignore();
	QString unsavedDbs;
	for(int i = 0; i < ui->dbEditorTabsWidget->count(); i++)
	{
		auto* tab = static_cast<DatabaseEditorWidget*>(
			ui->dbEditorTabsWidget->widget(i));
		if(tab->hasUnsavedData())
			unsavedDbs.append('\n').append(tab->tabName(true));
	}
	if(!unsavedDbs.isEmpty() &&
	   QMessageBox::question(
		   this, tr("Exit discarding changes?"),
		   tr("The following databases have unsaved data. Proceed anyways?") +
			   unsavedDbs) != QMessageBox::Yes)
		return;
	event->accept();
}

// private slots

void MainWindow::languageChanged(QAction* action)
{
	if(action != nullptr)
	{
		loadLanguage(action->data().toString());
	}
}

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
	addTab(file);
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
	addTab(file);
}

void MainWindow::showClipboardDatabase()
{
	auto db = clipboardDatabase();
	auto const ptr = db.password();
	if(!ptr.isEmpty())
	{
		ui->dbEditorTabsWidget->setCurrentWidget(
			&widgetFromConnection(SQL_CLIPBOARD_CONN));
		return;
	}
	addTab(SQL_CLIPBOARD_CONN);
}

void MainWindow::closeDatabase(int index)
{
	if(index < 0)
		index = ui->dbEditorTabsWidget->currentIndex();
	auto* tab = static_cast<DatabaseEditorWidget*>(
		ui->dbEditorTabsWidget->widget(index));
	if(tab->hasUnsavedData() &&
	   QMessageBox::question(
		   this, tr("Close database?"),
		   tr("The database '%1' has unsaved changes. Proceed anyways?")
			   .arg(tab->tabName(true))) != QMessageBox::Yes)
		return;
	auto const dbConnection = tab->database().connectionName();
	ui->dbEditorTabsWidget->removeTab(index);
	delete tab;
	if(dbConnection == SQL_CLIPBOARD_CONN) // Long live the "Clipboard" db!
		clipboardDatabase().setPassword("");
	else
		QSqlDatabase::removeDatabase(dbConnection);
	enableEditing(ui->dbEditorTabsWidget->count() != 0);
}

void MainWindow::newCard()
{
	emit currentTab().newCard();
}

void MainWindow::saveData()
{
	emit currentTab().saveData();
}

void MainWindow::deleteData()
{
	emit currentTab().deleteData();
}

void MainWindow::copySelectedCards()
{
	auto& tab = currentTab();
	auto const codes = tab.selectedCards();
	if(codes.size() == 0)
		return;
	auto dbSrc = tab.database();
	auto dbDst = clipboardDatabase();
	dbDst.exec("DELETE FROM datas;");
	dbDst.exec("DELETE FROM texts;");
	copyCards(codes, dbSrc, dbDst);
	// Update Clipboard card list if opened in tab
	if(!dbDst.password().isEmpty()) // TODO: Properly update card list
		emit widgetFromConnection(SQL_CLIPBOARD_CONN).refreshCardList();
}

void MainWindow::pasteClipboardCards()
{
	auto dbSrc = clipboardDatabase();
	auto dbDst = currentTab().database();
	if(dbDst.connectionName() == SQL_CLIPBOARD_CONN)
		return;
	auto const codes = [&]() -> QVector<quint32>
	{
		QVector<quint32> ret;
		auto q = buildQuery(dbSrc, "SELECT id FROM datas;");
		execQuery(q);
		while(q.next())
			ret.append(q.value(0).toUInt());
		return ret;
	}();
	if(codes.size() == 0)
		return;
	// Check cards to be overwritten, and confirm if necessary.
	auto const stmt = [&]() -> QString
	{
		QString ret(SQL_QUERY_CODE_AND_NAME_LIST);
		for(auto const code : codes)
			ret.append(QString::number(code)).append(',');
		ret.append("0);");
		return ret;
	}();
	auto q = buildQuery(dbDst, stmt);
	execQuery(q);
	QString cardsToOverwrite;
	while(q.next())
	{
		cardsToOverwrite.append("\n[")
			.append(q.value(0).toString())
			.append("] ");
		cardsToOverwrite.append(q.value(1).toString());
	}
	if(!cardsToOverwrite.isEmpty() &&
	   QMessageBox::question(
		   this, tr("Confirm Overwrite"),
		   tr("The following cards already exist in this database, are you "
	          "sure you want to replace them?") +
			   cardsToOverwrite) != QMessageBox::Yes)
		return;
	copyCards(codes, dbSrc, dbDst);
	// TODO: Highlight pasted cards in db
	emit currentTab().refreshCardList(); // TODO: Properly update card list
}

void MainWindow::openHomepage()
{
	QDesktopServices::openUrl(
		QUrl("https://www.youtube.com/watch?v=dQw4w9WgXcQ"));
}

// protected

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
	auto parse_event = [&]
	{
		static int pressedTab = -1;
		if(obj != ui->dbEditorTabsWidget)
			return false;
		if(event->type() != QEvent::MouseButtonPress &&
		   event->type() != QEvent::MouseButtonRelease)
			return false;
		auto mouseEvent = static_cast<QMouseEvent*>(event);
		if(mouseEvent->button() != Qt::MiddleButton)
			return false;
		auto tabIndex =
			ui->dbEditorTabsWidget->tabBar()->tabAt(mouseEvent->pos());
		if(tabIndex == -1)
		{
			pressedTab = -1;
			return false;
		}
		if(event->type() == QEvent::MouseButtonPress)
		{
			pressedTab = tabIndex;
		}
		else if(std::exchange(pressedTab, -1) == tabIndex)
		{
			emit ui->dbEditorTabsWidget->tabCloseRequested(tabIndex);
		}
		return true;
	};
	return parse_event() || QObject::eventFilter(obj, event);
}

// private

void MainWindow::loadLanguage(const QString& newLanguage)
{
	if(currLang != newLanguage)
	{
		currLang = newLanguage;
		QLocale locale = QLocale(currLang);
		QLocale::setDefault(locale);

		auto& newTranslator = translations[currLang];
		if(currentTranslator)
			QApplication::removeTranslator(currentTranslator.get());
		currentTranslator = newTranslator;
		if(currentTranslator)
			QApplication::installTranslator(currentTranslator.get());
	}
}

// we create the menu entries dynamically, dependent on the existing
// translations.
void MainWindow::createLanguageMenu()
{
	auto* langGroup = new QActionGroup(ui->menuLanguage);
	langGroup->setExclusive(true);

	connect(ui->actionHomepage, &QAction::triggered, this,
	        &MainWindow::openHomepage);

	connect(langGroup, &QActionGroup::triggered, this,
	        &MainWindow::languageChanged);

	auto add_translator =
		[&](QString locale, const std::shared_ptr<QTranslator>& translator)
	{
		translations.insert(locale, translator);
		QString lang = QLocale::languageToString(QLocale(locale).language());

		auto* action = new QAction("&" + lang, this);
		action->setCheckable(true);
		action->setData(locale);

		ui->menuLanguage->addAction(action);
		langGroup->addAction(action);

		return action;
	};

	add_translator("en", nullptr)->setChecked(true);

	auto load_translations_from_path = [&](QString path)
	{
		auto translationFiles = QDir(path).entryList(QStringList("*.qm"));

		for(auto filename : translationFiles)
		{
			auto locale = filename;
			// get locale extracted by filename
			locale.truncate(locale.lastIndexOf('.')); // "es"

			if(translations.contains(locale))
				continue;

			auto translator = std::make_shared<QTranslator>();
			if(!translator->load(filename, path))
				continue;

			(void)add_translator(locale, translator);
		}
	};

	// Load from internal resources
	load_translations_from_path(":/");
	load_translations_from_path(
		QApplication::applicationDirPath().append("/languages"));
}

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
	auto const pass = db.password().toULongLong(&ok, 16);
	Q_ASSERT(ok);
	DatabaseEditorWidget* ptr{};
	std::memcpy(&ptr, &pass, sizeof(DatabaseEditorWidget*));
	return *ptr;
}

QSqlDatabase MainWindow::clipboardDatabase() const
{
	auto db = QSqlDatabase::database(SQL_CLIPBOARD_CONN, false);
	Q_ASSERT(db.isValid());
	return db;
}

void MainWindow::setupCleanDB(QSqlDatabase& db) const
{
	bool const isDbOpen = db.open();
	Q_ASSERT(isDbOpen);
	db.exec(SQL_QUERY_CREATE_DATAS_TABLE);
	db.exec(SQL_QUERY_CREATE_TEXTS_TABLE);
}

void MainWindow::addTab(QString const& file)
{
	auto& tabber = *ui->dbEditorTabsWidget;
	auto* newTab = new DatabaseEditorWidget(tabber, file);
	auto const idx = tabber.addTab(newTab, newTab->tabName());
	tabber.setCurrentIndex(idx);
	enableEditing(true);
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
