#include "main_window.hpp"

#include <array>
#include <QDateTime>
#include <QDesktopServices> // openUrl
#include <QFileDialog>
#include <QMessageBox>
#include <QRegExpValidator>
#include <QScrollBar>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QTranslator>

#include "../archetypes.hpp"
#include "ui_main_window.h"

namespace
{

constexpr qint32 QMARK_ATK_DEF = -2;
constexpr quint32 TYPE_LINK = 0x4000000;

// Used to access archetype list item's value.
constexpr auto ARCHETYPE_ROLE = Qt::UserRole + 1;

struct BitField
{
	quint64 value;
	char const* name;
};

// clang-format off
constexpr std::array const TYPE_FIELDS{
	BitField{0x1, QT_TR_NOOP("Monster")},
	BitField{0x2, QT_TR_NOOP("Spell")},
	BitField{0x4, QT_TR_NOOP("Trap")},
	BitField{0x10, QT_TR_NOOP("Normal")},
	BitField{0x20, QT_TR_NOOP("Effect")},
	BitField{0x40, QT_TR_NOOP("Fusion")},
	BitField{0x80, QT_TR_NOOP("Ritual")},
	BitField{0x100, QT_TR_NOOP("Trap Monster")},
	BitField{0x200, QT_TR_NOOP("Spirit")},
	BitField{0x400, QT_TR_NOOP("Union")},
	BitField{0x800, QT_TR_NOOP("Gemini")},
	BitField{0x1000, QT_TR_NOOP("Tuner")},
	BitField{0x2000, QT_TR_NOOP("Synchro")},
	BitField{0x4000, QT_TR_NOOP("Token")},
	BitField{0x8000, QT_TR_NOOP("Maximum")},
	BitField{0x10000, QT_TR_NOOP("Quick-Play")},
	BitField{0x20000, QT_TR_NOOP("Continuous")},
	BitField{0x40000, QT_TR_NOOP("Equip")},
	BitField{0x80000, QT_TR_NOOP("Field")},
	BitField{0x100000, QT_TR_NOOP("Counter")},
	BitField{0x200000, QT_TR_NOOP("Flip")},
	BitField{0x400000, QT_TR_NOOP("Toon")},
	BitField{0x800000, QT_TR_NOOP("Xyz")},
	BitField{0x1000000, QT_TR_NOOP("Pendulum")},
	BitField{0x2000000, QT_TR_NOOP("Special Summon")},
	BitField{0x4000000, QT_TR_NOOP("Link")},
	BitField{0x8000000, QT_TR_NOOP("Skill")},
	BitField{0x10000000, QT_TR_NOOP("Action")},
	BitField{0x20000000, QT_TR_NOOP("Plus")},
	BitField{0x40000000, QT_TR_NOOP("Minus")},
	BitField{0x80000000, QT_TR_NOOP("Armor")},
};
constexpr std::array const RACE_FIELDS{
	BitField{0x1, QT_TR_NOOP("Warrior")},
	BitField{0x2, QT_TR_NOOP("Spellcaster")},
	BitField{0x4, QT_TR_NOOP("Fairy")},
	BitField{0x8, QT_TR_NOOP("Fiend")},
	BitField{0x10, QT_TR_NOOP("Zombie")},
	BitField{0x20, QT_TR_NOOP("Machine")},
	BitField{0x40, QT_TR_NOOP("Aqua")},
	BitField{0x80, QT_TR_NOOP("Pyro")},
	BitField{0x100, QT_TR_NOOP("Rock")},
	BitField{0x200, QT_TR_NOOP("Winged-Beast")},
	BitField{0x400, QT_TR_NOOP("Plant")},
	BitField{0x800, QT_TR_NOOP("Insect")},
	BitField{0x1000, QT_TR_NOOP("Thunder")},
	BitField{0x2000, QT_TR_NOOP("Dragon")},
	BitField{0x4000, QT_TR_NOOP("Beast")},
	BitField{0x8000, QT_TR_NOOP("Beast-Warrior")},
	BitField{0x10000, QT_TR_NOOP("Dinosaur")},
	BitField{0x20000, QT_TR_NOOP("Fish")},
	BitField{0x40000, QT_TR_NOOP("Sea-Serpent")},
	BitField{0x80000, QT_TR_NOOP("Reptile")},
	BitField{0x100000, QT_TR_NOOP("Psychic")},
	BitField{0x200000, QT_TR_NOOP("Divine")},
	BitField{0x400000, QT_TR_NOOP("Creator God")},
	BitField{0x800000, QT_TR_NOOP("Wyrm")},
	BitField{0x1000000, QT_TR_NOOP("Cyberse")},
	BitField{0x2000000, QT_TR_NOOP("Illusionist")},
	BitField{0x4000000, QT_TR_NOOP("Cyborg")},
	BitField{0x8000000, QT_TR_NOOP("Magical Knight")},
	BitField{0x10000000, QT_TR_NOOP("High Dragon")},
	BitField{0x20000000, QT_TR_NOOP("Omega Psychic")},
	BitField{0x40000000, QT_TR_NOOP("Celestial Warrior")},
	BitField{0x80000000, QT_TR_NOOP("Galaxy")},
};
constexpr std::array const ATTRIBUTE_FIELDS{
	BitField{0x1, QT_TR_NOOP("EARTH")},
	BitField{0x2, QT_TR_NOOP("WATER")},
	BitField{0x4, QT_TR_NOOP("FIRE")},
	BitField{0x8, QT_TR_NOOP("WIND")},
	BitField{0x10, QT_TR_NOOP("LIGHT")},
	BitField{0x20, QT_TR_NOOP("DARK")},
	BitField{0x40, QT_TR_NOOP("DIVINE")},
};
constexpr std::array const SCOPE_FIELDS{
	BitField{0x1, QT_TR_NOOP("OCG")},
	BitField{0x2, QT_TR_NOOP("TCG")},
	BitField{0x4, QT_TR_NOOP("Anime")},
	BitField{0x8, QT_TR_NOOP("Illegal")},
	BitField{0x10, QT_TR_NOOP("Video Game")},
	BitField{0x20, QT_TR_NOOP("Custom")},
	BitField{0x40, QT_TR_NOOP("Speed")},
	BitField{0x100, QT_TR_NOOP("Pre-Release")},
	BitField{0x200, QT_TR_NOOP("Rush")},
	BitField{0x400, QT_TR_NOOP("Legend")},
	BitField{0x1000, QT_TR_NOOP("Hidden")},
};
constexpr std::array const CATEGORY_FIELDS{
	BitField{0x1, QT_TR_NOOP("Destroy Monster")},
	BitField{0x2, QT_TR_NOOP("Destroy S/T")},
	BitField{0x4, QT_TR_NOOP("Destroy Deck")},
	BitField{0x8, QT_TR_NOOP("Destroy Hand")},
	BitField{0x10, QT_TR_NOOP("Send to GY")},
	BitField{0x20, QT_TR_NOOP("Send to Hand")},
	BitField{0x40, QT_TR_NOOP("Send to Deck")},
	BitField{0x80, QT_TR_NOOP("Banish")},
	BitField{0x100, QT_TR_NOOP("Draw")},
	BitField{0x200, QT_TR_NOOP("Search")},
	BitField{0x400, QT_TR_NOOP("Change ATK/DEF")},
	BitField{0x800, QT_TR_NOOP("Change Level/Rank")},
	BitField{0x1000, QT_TR_NOOP("Position")},
	BitField{0x2000, QT_TR_NOOP("Piercing")},
	BitField{0x4000, QT_TR_NOOP("Direct Attack")},
	BitField{0x8000, QT_TR_NOOP("Multi Attack")},
	BitField{0x10000, QT_TR_NOOP("Negate Activation")},
	BitField{0x20000, QT_TR_NOOP("Negate Effect")},
	BitField{0x40000, QT_TR_NOOP("Damage LP")},
	BitField{0x80000, QT_TR_NOOP("Recover LP")},
	BitField{0x100000, QT_TR_NOOP("Special Summon")},
	BitField{0x200000, QT_TR_NOOP("Non-effect-related")},
	BitField{0x400000, QT_TR_NOOP("Token-related")},
	BitField{0x800000, QT_TR_NOOP("Fusion-related")},
	BitField{0x1000000, QT_TR_NOOP("Ritual-related")},
	BitField{0x2000000, QT_TR_NOOP("Synchro-related")},
	BitField{0x4000000, QT_TR_NOOP("Xyz-related")},
	BitField{0x8000000, QT_TR_NOOP("Link-related")},
	BitField{0x10000000, QT_TR_NOOP("Counter-related")},
	BitField{0x20000000, QT_TR_NOOP("Gamble")},
	BitField{0x40000000, QT_TR_NOOP("Control")},
	BitField{0x80000000, QT_TR_NOOP("Move Zones")},
};
// clang-format on

QString const SQL_DB_DRIVER("QSQLITE");

QString const SDL_QUERY_CREATE_DATAS_TABLE(R"(
CREATE TABLE IF NOT EXISTS "datas" (
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
);
)");

QString const SDL_QUERY_CREATE_TEXTS_TABLE(R"(
CREATE TABLE IF NOT EXISTS "texts" (
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
);
)");

QString const SQL_QUERY_FIRST_ROW_CODE(R"(
SELECT id FROM datas ORDER BY ROWID ASC LIMIT 1;
)");

QString const SQL_QUERY_DATA(R"(
SELECT id,alias,setcode,type,atk,def,level,race,attribute,ot,category
FROM datas WHERE datas.id = ?;
)");

QString const SQL_QUERY_TEXT(R"(
SELECT name,desc,str1,str2,str3,str4,str5,str6,str7,str8,str9,str10,str11,str12,str13,str14,str15,str16
FROM texts WHERE texts.id = ?;
)");

QString const SQL_DELETE_DATA(R"(
DELETE FROM datas
WHERE id = ?;
)");

QString const SQL_DELETE_TEXT(R"(
DELETE FROM texts
WHERE id = ?;
)");

QString const SQL_INSERT_DATA(R"(
INSERT INTO datas (id,alias,setcode,type,atk,def,level,race,attribute,ot,category)
VALUES (?,?,?,?,?,?,?,?,?,?,?);
)");

QString const SQL_INSERT_TEXT(R"(
INSERT INTO texts (id,name,desc,str1,str2,str3,str4,str5,str6,str7,str8,str9,str10,str11,str12,str13,str14,str15,str16)
VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);
)");

inline bool isChecked(QListWidgetItem* item) noexcept
{
	return item->checkState() == Qt::Checked;
}

inline void setChecked(QListWidgetItem* item, bool value) noexcept
{
	item->setCheckState(value ? Qt::Checked : Qt::Unchecked);
}

inline void setRegexValidator(QLineEdit& parent, QString const& regex)
{
	parent.setValidator(new QRegExpValidator(QRegExp(regex), &parent));
}

class CardCodeNameSqlModel final : public QSqlTableModel
{
public:
	explicit CardCodeNameSqlModel(QWidget* parent, QSqlDatabase db)
		: QSqlTableModel(parent, db)
	{
		setTable("texts");
		setEditStrategy(QSqlTableModel::OnManualSubmit);
		select();
		setHeaderData(0, Qt::Horizontal, tr("Code"));
		setHeaderData(1, Qt::Horizontal, tr("Name"));
	}

	virtual ~CardCodeNameSqlModel() = default;

private:
	QString selectStatement() const override
	{
		return QString(R"(SELECT "id", "name" FROM "texts" WHERE )") + filter();
	}
};

} // namespace

class FilteringHeader final : public QHeaderView
{
public:
	FilteringHeader(QTableView& parent)
		: QHeaderView(Qt::Horizontal, &parent), model(nullptr)
	{
		setSortIndicatorShown(false);
		setSectionsClickable(false);
		for(int i = 0; i < 2; i++)
		{
			auto* w = new QLineEdit(this);
			w->setClearButtonEnabled(true);
			w->setPlaceholderText(tr("Filter"));
			w->setVisible(true);
			filters.push_back(w);
			connect(w, &QLineEdit::textEdited, this,
			        &FilteringHeader::updateTableFilters);
		}
		setRegexValidator(*filters[0], "[1-9][0-9]*");
		connect(this, &FilteringHeader::sectionResized, this,
		        &FilteringHeader::adjustFilters);
		connect(this, &FilteringHeader::sectionClicked, this,
		        &FilteringHeader::adjustFilters);
		connect(parent.horizontalScrollBar(), &QScrollBar::valueChanged, this,
		        &FilteringHeader::adjustFilters);
		connect(parent.verticalScrollBar(), &QScrollBar::valueChanged, this,
		        &FilteringHeader::adjustFilters);
	}

	QSize sizeHint() const override
	{
		QSize s = QHeaderView::sizeHint();
		s.setHeight(s.height() + filters[0]->sizeHint().height());
		return s;
	}

	void updateGeometries() override
	{
		setViewportMargins(0, 0, 0, filters[0]->sizeHint().height());
		QHeaderView::updateGeometries();
		adjustFilters();
	}

	void setModel(QSqlTableModel* newModel)
	{
		model = newModel;
		updateTableFilters();
	}
private slots:
	void adjustFilters()
	{
		int const y = QHeaderView::sizeHint().height();
		int section = 0;
		for(auto* f : filters)
		{
			f->move(sectionPosition(section) - offset(), y);
			f->resize(sectionSize(section), f->sizeHint().height());
			++section;
		}
	}

	void updateTableFilters(QString const& text = "")
	{
		if(model == nullptr)
			return;
		model->setFilter(QString(R"("id" LIKE "%%1%" AND "name" LIKE "%%2%")")
		                     .arg(filters[0]->text(), filters[1]->text()));
		model->select();
	}

private:
	QList<QLineEdit*> filters;
	QSqlTableModel* model;
};

// public

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, spanishTranslator(std::make_unique<QTranslator>())
	, ui(std::make_unique<Ui::MainWindow>())
	, cardListFilter(nullptr) // Must be fully init'd later due to "setupUi".
	, customArchetype(false)
{
	spanishTranslator->load(":/es");
	QApplication::instance()->installTranslator(spanishTranslator.get());
	ui->setupUi(this);
	connect(ui->actionNewDatabase, &QAction::triggered, this,
	        &MainWindow::newDatabase);
	connect(ui->actionOpenDatabase, &QAction::triggered, this,
	        &MainWindow::openDatabase);
	connect(ui->actionCloseDatabase, &QAction::triggered, this,
	        &MainWindow::closeDatabase);
	connect(ui->actionSaveData, &QAction::triggered, this,
	        &MainWindow::saveData);
	connect(ui->actionEnglish, &QAction::triggered, this,
	        &MainWindow::toEnglish);
	connect(ui->actionSpanish, &QAction::triggered, this,
	        &MainWindow::toSpanish);
	connect(ui->actionHomepage, &QAction::triggered, this,
	        &MainWindow::openHomepage);
	connect(ui->addArcheButton, &QPushButton::clicked, this,
	        &MainWindow::addArchetypeToList);
	connect(ui->removeArcheButton, &QPushButton::clicked, this,
	        &MainWindow::removeArchetypeFromList);
	connect(ui->archeList, &QListWidget::currentItemChanged, this,
	        &MainWindow::onArcheListItemChanged);
	connect(ui->cardCodeNameList, &QAbstractItemView::activated, this,
	        &MainWindow::onCardsListItemActivated);
	connect(ui->archeComboBox, QOverload<int>::of(&QComboBox::activated), this,
	        &MainWindow::onArcheComboIndexActivated);
	connect(ui->archeComboBox, &QComboBox::editTextChanged, this,
	        &MainWindow::onArcheComboEditTextChanged);
	setRegexValidator(*ui->passLineEdit, "[0-9]+");
	setRegexValidator(*ui->aliasLineEdit, "[0-9]+");
	cardListFilter = new FilteringHeader(*ui->cardCodeNameList);
	ui->cardCodeNameList->setHorizontalHeader(cardListFilter);
	auto populate_cbs = [&](QListWidget* parent, auto const& fields)
	{
		using Item = QListWidgetItem;
		std::unique_ptr<Item*[]> boxes(new Item*[fields.size()]);
		for(size_t i = 0; i < fields.size(); ++i)
		{
			auto* item = new QListWidgetItem(tr(fields[i].name), parent);
			item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled |
			               Qt::ItemNeverHasChildren);
			setChecked(item, false);
			boxes[i] = item;
		}
		return boxes;
	};
	typeCbs = populate_cbs(ui->typesWidget, TYPE_FIELDS);
	raceCbs = populate_cbs(ui->racesWidget, RACE_FIELDS);
	attributeCbs = populate_cbs(ui->attributesWidget, ATTRIBUTE_FIELDS);
	scopeCbs = populate_cbs(ui->scopesWidget, SCOPE_FIELDS);
	categoryCbs = populate_cbs(ui->categoriesWidget, CATEGORY_FIELDS);
	ui->archeComboBox->blockSignals(true);
	auto const end = ARCHETYPES_MAP.constEnd();
	for(auto it = ARCHETYPES_MAP.constBegin(); it != end; ++it)
	{
		ui->archeComboBox->addItem(formatArchetype(it.key(), it.value()));
		ui->archeComboBox->setItemData(ui->archeComboBox->count() - 1, it.key(),
		                               ARCHETYPE_ROLE);
	}
	ui->archeComboBox->setCurrentIndex(0);
	ui->archeComboBox->blockSignals(false);
}

MainWindow::~MainWindow()
{
	QApplication::instance()->removeTranslator(spanishTranslator.get());
}

void MainWindow::changeEvent(QEvent* event)
{
	if(event->type() == QEvent::LanguageChange)
		ui->retranslateUi(this);
	else
		QWidget::changeEvent(event);
}

// private slots

void MainWindow::newDatabase()
{
	if(!checkAndAskToCloseDb())
		return;
	const QString file = QFileDialog::getSaveFileName(
		this, tr("Save Database As"), ".",
		tr("YGOPro Database (*.cdb *.db *.sqlite)"));
	if(file.isEmpty())
		return;
	QFile::remove(file);
	auto db = QSqlDatabase::addDatabase(SQL_DB_DRIVER);
	db.setDatabaseName(file);
	if(!db.open())
	{
		// TODO: show message
		return;
	}
	QSqlQuery(SDL_QUERY_CREATE_DATAS_TABLE, db);
	QSqlQuery(SDL_QUERY_CREATE_TEXTS_TABLE, db);
	fillCardList();
	enableEditing(true);
}

void MainWindow::openDatabase()
{
	auto does_db_have_correct_format = [&](QSqlDatabase& db)
	{
		return true; // TODO
	};
	if(!checkAndAskToCloseDb())
		return;
	QString const file = QFileDialog::getOpenFileName(
		this, tr("Select Database"), ".",
		tr("YGOPro Database (*.cdb *.db *.sqlite)"));
	if(file.isEmpty())
		return;
	auto db = QSqlDatabase::addDatabase(SQL_DB_DRIVER);
	db.setDatabaseName(file);
	if(!db.open())
	{
		QMessageBox::critical(this, tr("Error Opening Database"),
		                      db.lastError().text());
		closeDatabase();
		return;
	}
	if(!does_db_have_correct_format(db))
	{
		QMessageBox::critical(
			this, tr("Error Opening Database"),
			tr("Selected file is not a proper YGOPRO database."));
		closeDatabase();
		return;
	}
	fillCardList();
	QSqlQuery q(SQL_QUERY_FIRST_ROW_CODE, db);
	q.first();
	enableEditing(true);
	updateUiWithCode(q.value(0).toUInt());
}

void MainWindow::closeDatabase()
{
	cardListFilter->setModel(nullptr);
	ui->cardCodeNameList->setModel(nullptr);
	auto db = QSqlDatabase::database();
	if(db.isValid())
	{
		db.close();
		QSqlDatabase::removeDatabase(db.connectionName());
	}
	enableEditing(false);
	updateUiWithCode(0U);
}

void MainWindow::saveData()
{
	updateCardWithUi();
	ui->cardCodeNameList->update();
}

void MainWindow::toEnglish()
{
	ui->actionEnglish->setEnabled(false);
	ui->actionSpanish->setEnabled(true);
	ui->actionSpanish->setChecked(false);
	QApplication::instance()->removeTranslator(spanishTranslator.get());
}

void MainWindow::toSpanish()
{
	ui->actionSpanish->setEnabled(false);
	ui->actionEnglish->setEnabled(true);
	ui->actionEnglish->setChecked(false);
	QApplication::instance()->installTranslator(spanishTranslator.get());
}

void MainWindow::openHomepage()
{
	QDesktopServices::openUrl(
		QUrl("https://www.youtube.com/watch?v=dQw4w9WgXcQ"));
}

void MainWindow::addArchetypeToList(bool clicked)
{
	if(ui->archeList->count() >= 4 &&
	   QMessageBox::question(
		   this, tr("Add Archetype?"),
		   tr("The database schema can only save up to 4 archetypes, even if "
	          "you add this one it won't be saved. Proceed anyways?")) !=
	       QMessageBox::Yes)
		return;
	if(!customArchetype)
	{
		// Easy case: Just append the currently selected archetype from the
		// ComboBox's data.
		addArchetype(static_cast<quint16>(
			ui->archeComboBox->currentData(ARCHETYPE_ROLE).toUInt()));
		return;
	}
	// Here we do a best effort to parse the ComboBox's current text and get
	// the archetype's code either in decimal or hexadecimal.
	auto const text = ui->archeComboBox->currentText().split('|')[0];
	quint16 setcode;
	bool ok;
	for(auto base : {10, 16})
	{
		setcode = text.toUInt(&ok, base);
		if(ok && setcode != 0)
		{
			addArchetype(setcode);
			return;
		}
	}
	QMessageBox::critical(
		this, tr("Couldn't Parse Archetype"),
		tr("The currently set archetype could not be parsed either in decimal "
	       "or hexadecimal format. Either fix the format or select one of the "
	       "preset archetypes from the list."));
}

void MainWindow::removeArchetypeFromList(bool clicked)
{
	Q_ASSERT(ui->archeList->currentItem() != nullptr);
	delete ui->archeList->takeItem(
		ui->archeList->row(ui->archeList->currentItem()));
}

void MainWindow::onArcheListItemChanged(QListWidgetItem* current,
                                        QListWidgetItem* previous)
{
	ui->removeArcheButton->setEnabled(current != nullptr);
}

void MainWindow::onArcheComboIndexActivated(int index)
{
	customArchetype = false;
}

void MainWindow::onArcheComboEditTextChanged(QString const& text)
{
	customArchetype = true;
	ui->addArcheButton->setEnabled(!text.isEmpty());
}

void MainWindow::onCardsListItemActivated(QModelIndex const& index)
{
	updateUiWithCode(index.siblingAtColumn(0).data().toUInt());
}

// private

QString MainWindow::formatArchetype(quint16 code, char const* name) const
{
	QString const ret(R"(0x%1 | %2)");
	auto const code_str = QString::number(code, 16);
	return ret.arg(code_str, name == nullptr ? "???" : tr(name));
}

void MainWindow::addArchetype(quint16 code)
{
	auto const search = ARCHETYPES_MAP.find(code);
	ui->archeList->addItem(formatArchetype(
		code, search != ARCHETYPES_MAP.constEnd() ? search.value() : nullptr));
	auto& item = *ui->archeList->item(ui->archeList->count() - 1);
	item.setData(ARCHETYPE_ROLE, code);
}

bool MainWindow::checkAndAskToCloseDb()
{
	if(!QSqlDatabase::database().isValid())
		return true;
	if(QMessageBox::question(
		   this, tr("Close Opened Database?"),
		   tr("Do you wish to close the currently opened database?")) ==
	   QMessageBox::Yes)
	{
		closeDatabase();
		return true;
	}
	return false;
}

void MainWindow::enableEditing(bool editing)
{
	ui->actionCloseDatabase->setEnabled(editing);
	ui->dbGroup->setEnabled(editing);
	ui->cardGroup->setEnabled(editing);
	ui->actionSaveData->setEnabled(editing);
}

void MainWindow::fillCardList()
{
	auto db = QSqlDatabase::database();
	auto* model = new CardCodeNameSqlModel(ui->cardCodeNameList, db);
	cardListFilter->setModel(model);
	ui->cardCodeNameList->setModel(model);
	ui->cardCodeNameList->resizeColumnsToContents();
}

void MainWindow::updateUiWithCode(quint32 code)
{
	auto toggle_cbs =
		[&](quint64 bits, auto const& fields, QListWidgetItem** cbs)
	{
		for(size_t i = 0; i < fields.size(); ++i)
			setChecked(cbs[i], (bits & fields[i].value) != 0U);
	};
	int const stringsRowCount = ui->stringsTableWidget->rowCount();
	// Clean the UI first
	toggle_cbs(0U, TYPE_FIELDS, typeCbs.get());
	toggle_cbs(0U, RACE_FIELDS, raceCbs.get());
	toggle_cbs(0U, ATTRIBUTE_FIELDS, attributeCbs.get());
	toggle_cbs(0U, SCOPE_FIELDS, scopeCbs.get());
	toggle_cbs(0U, CATEGORY_FIELDS, categoryCbs.get());
	ui->passLineEdit->setText("0");
	ui->aliasLineEdit->setText("0");
	ui->nameLineEdit->setText("");
	ui->archeList->clear();
	ui->descPlainTextEdit->setPlainText("");
	ui->atkQmCheckBox->setChecked(false);
	ui->atkSpinBox->setEnabled(true);
	ui->atkSpinBox->setValue(0);
	ui->defQmCheckBox->setChecked(false);
	ui->defSpinBox->setEnabled(true);
	ui->defSpinBox->setValue(0);
	ui->levelSpinBox->setValue(0);
	ui->lScaleSpinBox->setValue(0);
	ui->rScaleSpinBox->setValue(0);
	ui->markerBottomLeftButton->setChecked(false);
	ui->markerBottomButton->setChecked(false);
	ui->markerBottomRightButton->setChecked(false);
	ui->markerLeftButton->setChecked(false);
	ui->markerRightButton->setChecked(false);
	ui->markerTopLeftButton->setChecked(false);
	ui->markerTopButton->setChecked(false);
	ui->markerTopRightButton->setChecked(false);
	for(int i = 0; i < stringsRowCount; ++i)
		ui->stringsTableWidget->item(i, 0)->setText("");
	if(code == 0U) // if 0 then we just exit to leave UI in "clean" state
		return;
	// Query data and strings
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
	// Populate the fields with the new data and strings
	ui->passLineEdit->setText(q1.value(0).toString());
	ui->aliasLineEdit->setText(q1.value(1).toString());
	{ // Setcode population
		static constexpr auto MAX_SETCODES = 4;
		quint64 const setcodes = q1.value(2).toULongLong();
		for(unsigned i = 0U; i < MAX_SETCODES; i++)
		{
			quint16 const setcode = (setcodes >> (i * 16U)) & 0xFFFFU;
			if(setcode == 0)
				continue;
			addArchetype(setcode);
		}
	}
	quint32 const type = q1.value(3).toUInt();
	toggle_cbs(type, TYPE_FIELDS, typeCbs.get());
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
	toggle_cbs(q1.value(7).toUInt(), RACE_FIELDS, raceCbs.get());
	toggle_cbs(q1.value(8).toUInt(), ATTRIBUTE_FIELDS, attributeCbs.get());
	toggle_cbs(q1.value(9).toUInt(), SCOPE_FIELDS, scopeCbs.get());
	toggle_cbs(q1.value(10).toUInt(), CATEGORY_FIELDS, categoryCbs.get());
	ui->nameLineEdit->setText(q2.value(0).toString());
	ui->descPlainTextEdit->setPlainText(q2.value(1).toString());
	for(int i = 0; i < stringsRowCount; ++i)
	{
		auto& item = *ui->stringsTableWidget->item(i, 0);
		item.setText(q2.value(2 + i).toString());
	}
}

void MainWindow::updateCardWithUi()
{
	qint32 const code = ui->passLineEdit->text().toUInt();
	if(code == 0)
	{
		QMessageBox::warning(this, tr("Invalid passcode"),
		                     tr("Passcode cannot be 0 or empty."));
		return;
	}
	auto db = QSqlDatabase::database();
	Q_ASSERT(db.isValid());
	QSqlQuery q1(db);
	q1.prepare(SQL_DELETE_DATA);
	QSqlQuery q2(db);
	q2.prepare(SQL_DELETE_TEXT);
	QSqlQuery q3(db);
	q3.prepare(SQL_INSERT_DATA);
	QSqlQuery q4(db);
	q4.prepare(SQL_INSERT_TEXT);
	// Remove previous data
	q1.bindValue(0, code);
	q1.exec();
	// Remove previous strings
	q2.bindValue(0, code);
	q2.exec();
	// Insert data
	auto compute_bitfield = [&](auto const& fields,
	                            QListWidgetItem** cbs) -> quint64
	{
		quint64 value = 0x0;
		for(size_t i = 0; i < fields.size(); ++i)
			value |= isChecked(cbs[i]) ? fields[i].value : 0x0;
		return value;
	};
	quint64 const type = compute_bitfield(TYPE_FIELDS, typeCbs.get());
	auto compute_def_value = [&]() -> qint32
	{
		if((type & TYPE_LINK) == 0U)
			return ui->defQmCheckBox->isChecked() ? QMARK_ATK_DEF
			                                      : ui->defSpinBox->value();
		quint32 link = 0U;
		link |= ui->markerBottomLeftButton->isChecked() ? 0x1 : 0;
		link |= ui->markerBottomButton->isChecked() ? 0x2 : 0;
		link |= ui->markerBottomRightButton->isChecked() ? 0x4 : 0;
		link |= ui->markerLeftButton->isChecked() ? 0x8 : 0;
		link |= ui->markerRightButton->isChecked() ? 0x20 : 0;
		link |= ui->markerTopLeftButton->isChecked() ? 0x40 : 0;
		link |= ui->markerTopButton->isChecked() ? 0x80 : 0;
		link |= ui->markerTopRightButton->isChecked() ? 0x100 : 0;
		return static_cast<qint32>(link);
	};
	auto compute_level_value = [&]() -> quint32
	{
		return ui->levelSpinBox->value() &
		       ((ui->lScaleSpinBox->value() & 0xFF) << 24U) &
		       ((ui->rScaleSpinBox->value() & 0xFF) << 16U);
	};
	q3.bindValue(0, code);
	q3.bindValue(1, ui->aliasLineEdit->text().toUInt());
	q3.bindValue(
		2,
		[&]()
		{
			quint64 setcodes = 0;
			// NOTE: Limitation of current DB schema.
		    // Only 4 quint16 can fit in a quint64.
			int const max = std::min(4, ui->archeList->count());
			for(int i = 0; i < max; i++)
			{
				quint16 const setcode =
					ui->archeList->item(i)->data(ARCHETYPE_ROLE).toUInt();
				setcodes |= static_cast<quint64>(setcode & 0xFFFFU)
			                << (i * 16U);
			}
			return setcodes;
		}());
	q3.bindValue(3, type);
	q3.bindValue(4, ui->atkQmCheckBox->isChecked() ? QMARK_ATK_DEF
	                                               : ui->atkSpinBox->value());
	q3.bindValue(5, compute_def_value());
	q3.bindValue(6, compute_level_value());
	q3.bindValue(7, compute_bitfield(RACE_FIELDS, raceCbs.get()));
	q3.bindValue(8, compute_bitfield(ATTRIBUTE_FIELDS, attributeCbs.get()));
	q3.bindValue(9, compute_bitfield(SCOPE_FIELDS, scopeCbs.get()));
	q3.bindValue(10, compute_bitfield(CATEGORY_FIELDS, categoryCbs.get()));
	q3.exec();
	// Insert strings
	q4.bindValue(0, code);
	q4.bindValue(1, ui->nameLineEdit->text());
	q4.bindValue(2, ui->descPlainTextEdit->toPlainText());
	int const stringsRowCount = ui->stringsTableWidget->rowCount();
	for(int i = 0; i < stringsRowCount; ++i)
	{
		auto& item = *ui->stringsTableWidget->item(i, 0);
		q4.bindValue(3 + i, item.text());
	}
	q4.exec();
}
