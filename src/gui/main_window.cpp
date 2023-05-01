#include "main_window.hpp"

#include <QDateTime>
#include <QDesktopServices> // openUrl
#include <QFileDialog>
#include <QMessageBox>
#include <QRegularExpressionValidator>
#include <QScrollBar>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QString>
#include <QStringList>
#include <QTranslator>
#include <array>
#include <ui_main_window.h>

#include "../archetypes.hpp"

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
	BitField{0x1, QT_TRANSLATE_NOOP("MainWindow", "Monster")},
	BitField{0x2, QT_TRANSLATE_NOOP("MainWindow", "Spell")},
	BitField{0x4, QT_TRANSLATE_NOOP("MainWindow", "Trap")},
	BitField{0x10, QT_TRANSLATE_NOOP("MainWindow", "Normal")},
	BitField{0x20, QT_TRANSLATE_NOOP("MainWindow", "Effect")},
	BitField{0x40, QT_TRANSLATE_NOOP("MainWindow", "Fusion")},
	BitField{0x80, QT_TRANSLATE_NOOP("MainWindow", "Ritual")},
	BitField{0x100, QT_TRANSLATE_NOOP("MainWindow", "Trap Monster")},
	BitField{0x200, QT_TRANSLATE_NOOP("MainWindow", "Spirit")},
	BitField{0x400, QT_TRANSLATE_NOOP("MainWindow", "Union")},
	BitField{0x800, QT_TRANSLATE_NOOP("MainWindow", "Gemini")},
	BitField{0x1000, QT_TRANSLATE_NOOP("MainWindow", "Tuner")},
	BitField{0x2000, QT_TRANSLATE_NOOP("MainWindow", "Synchro")},
	BitField{0x4000, QT_TRANSLATE_NOOP("MainWindow", "Token")},
	BitField{0x8000, QT_TRANSLATE_NOOP("MainWindow", "Maximum")},
	BitField{0x10000, QT_TRANSLATE_NOOP("MainWindow", "Quick-Play")},
	BitField{0x20000, QT_TRANSLATE_NOOP("MainWindow", "Continuous")},
	BitField{0x40000, QT_TRANSLATE_NOOP("MainWindow", "Equip")},
	BitField{0x80000, QT_TRANSLATE_NOOP("MainWindow", "Field")},
	BitField{0x100000, QT_TRANSLATE_NOOP("MainWindow", "Counter")},
	BitField{0x200000, QT_TRANSLATE_NOOP("MainWindow", "Flip")},
	BitField{0x400000, QT_TRANSLATE_NOOP("MainWindow", "Toon")},
	BitField{0x800000, QT_TRANSLATE_NOOP("MainWindow", "Xyz")},
	BitField{0x1000000, QT_TRANSLATE_NOOP("MainWindow", "Pendulum")},
	BitField{0x2000000, QT_TRANSLATE_NOOP("MainWindow", "Special Summon")},
	BitField{0x4000000, QT_TRANSLATE_NOOP("MainWindow", "Link")},
	BitField{0x8000000, QT_TRANSLATE_NOOP("MainWindow", "Skill")},
	BitField{0x10000000, QT_TRANSLATE_NOOP("MainWindow", "Action")},
	BitField{0x20000000, QT_TRANSLATE_NOOP("MainWindow", "Plus")},
	BitField{0x40000000, QT_TRANSLATE_NOOP("MainWindow", "Minus")},
	BitField{0x80000000, QT_TRANSLATE_NOOP("MainWindow", "Armor")},
};
constexpr std::array const RACE_FIELDS{
	BitField{0x1, QT_TRANSLATE_NOOP("MainWindow", "Warrior")},
	BitField{0x2, QT_TRANSLATE_NOOP("MainWindow", "Spellcaster")},
	BitField{0x4, QT_TRANSLATE_NOOP("MainWindow", "Fairy")},
	BitField{0x8, QT_TRANSLATE_NOOP("MainWindow", "Fiend")},
	BitField{0x10, QT_TRANSLATE_NOOP("MainWindow", "Zombie")},
	BitField{0x20, QT_TRANSLATE_NOOP("MainWindow", "Machine")},
	BitField{0x40, QT_TRANSLATE_NOOP("MainWindow", "Aqua")},
	BitField{0x80, QT_TRANSLATE_NOOP("MainWindow", "Pyro")},
	BitField{0x100, QT_TRANSLATE_NOOP("MainWindow", "Rock")},
	BitField{0x200, QT_TRANSLATE_NOOP("MainWindow", "Winged-Beast")},
	BitField{0x400, QT_TRANSLATE_NOOP("MainWindow", "Plant")},
	BitField{0x800, QT_TRANSLATE_NOOP("MainWindow", "Insect")},
	BitField{0x1000, QT_TRANSLATE_NOOP("MainWindow", "Thunder")},
	BitField{0x2000, QT_TRANSLATE_NOOP("MainWindow", "Dragon")},
	BitField{0x4000, QT_TRANSLATE_NOOP("MainWindow", "Beast")},
	BitField{0x8000, QT_TRANSLATE_NOOP("MainWindow", "Beast-Warrior")},
	BitField{0x10000, QT_TRANSLATE_NOOP("MainWindow", "Dinosaur")},
	BitField{0x20000, QT_TRANSLATE_NOOP("MainWindow", "Fish")},
	BitField{0x40000, QT_TRANSLATE_NOOP("MainWindow", "Sea-Serpent")},
	BitField{0x80000, QT_TRANSLATE_NOOP("MainWindow", "Reptile")},
	BitField{0x100000, QT_TRANSLATE_NOOP("MainWindow", "Psychic")},
	BitField{0x200000, QT_TRANSLATE_NOOP("MainWindow", "Divine")},
	BitField{0x400000, QT_TRANSLATE_NOOP("MainWindow", "Creator God")},
	BitField{0x800000, QT_TRANSLATE_NOOP("MainWindow", "Wyrm")},
	BitField{0x1000000, QT_TRANSLATE_NOOP("MainWindow", "Cyberse")},
	BitField{0x2000000, QT_TRANSLATE_NOOP("MainWindow", "Illusionist")},
	BitField{0x4000000, QT_TRANSLATE_NOOP("MainWindow", "Cyborg")},
	BitField{0x8000000, QT_TRANSLATE_NOOP("MainWindow", "Magical Knight")},
	BitField{0x10000000, QT_TRANSLATE_NOOP("MainWindow", "High Dragon")},
	BitField{0x20000000, QT_TRANSLATE_NOOP("MainWindow", "Omega Psychic")},
	BitField{0x40000000, QT_TRANSLATE_NOOP("MainWindow", "Celestial Warrior")},
	BitField{0x80000000, QT_TRANSLATE_NOOP("MainWindow", "Galaxy")},
};
constexpr std::array const ATTRIBUTE_FIELDS{
	BitField{0x1, QT_TRANSLATE_NOOP("MainWindow", "EARTH")},
	BitField{0x2, QT_TRANSLATE_NOOP("MainWindow", "WATER")},
	BitField{0x4, QT_TRANSLATE_NOOP("MainWindow", "FIRE")},
	BitField{0x8, QT_TRANSLATE_NOOP("MainWindow", "WIND")},
	BitField{0x10, QT_TRANSLATE_NOOP("MainWindow", "LIGHT")},
	BitField{0x20, QT_TRANSLATE_NOOP("MainWindow", "DARK")},
	BitField{0x40, QT_TRANSLATE_NOOP("MainWindow", "DIVINE")},
};
constexpr std::array const SCOPE_FIELDS{
	BitField{0x1, QT_TRANSLATE_NOOP("MainWindow", "OCG")},
	BitField{0x2, QT_TRANSLATE_NOOP("MainWindow", "TCG")},
	BitField{0x4, QT_TRANSLATE_NOOP("MainWindow", "Anime")},
	BitField{0x8, QT_TRANSLATE_NOOP("MainWindow", "Illegal")},
	BitField{0x10, QT_TRANSLATE_NOOP("MainWindow", "Video Game")},
	BitField{0x20, QT_TRANSLATE_NOOP("MainWindow", "Custom")},
	BitField{0x40, QT_TRANSLATE_NOOP("MainWindow", "Speed")},
	BitField{0x100, QT_TRANSLATE_NOOP("MainWindow", "Pre-Release")},
	BitField{0x200, QT_TRANSLATE_NOOP("MainWindow", "Rush")},
	BitField{0x400, QT_TRANSLATE_NOOP("MainWindow", "Legend")},
	BitField{0x1000, QT_TRANSLATE_NOOP("MainWindow", "Hidden")},
};
constexpr std::array const CATEGORY_FIELDS{
	BitField{0x1, QT_TRANSLATE_NOOP("MainWindow", "Destroy Monster")},
	BitField{0x2, QT_TRANSLATE_NOOP("MainWindow", "Destroy S/T")},
	BitField{0x4, QT_TRANSLATE_NOOP("MainWindow", "Destroy Deck")},
	BitField{0x8, QT_TRANSLATE_NOOP("MainWindow", "Destroy Hand")},
	BitField{0x10, QT_TRANSLATE_NOOP("MainWindow", "Send to GY")},
	BitField{0x20, QT_TRANSLATE_NOOP("MainWindow", "Send to Hand")},
	BitField{0x40, QT_TRANSLATE_NOOP("MainWindow", "Send to Deck")},
	BitField{0x80, QT_TRANSLATE_NOOP("MainWindow", "Banish")},
	BitField{0x100, QT_TRANSLATE_NOOP("MainWindow", "Draw")},
	BitField{0x200, QT_TRANSLATE_NOOP("MainWindow", "Search")},
	BitField{0x400, QT_TRANSLATE_NOOP("MainWindow", "Change ATK/DEF")},
	BitField{0x800, QT_TRANSLATE_NOOP("MainWindow", "Change Level/Rank")},
	BitField{0x1000, QT_TRANSLATE_NOOP("MainWindow", "Position")},
	BitField{0x2000, QT_TRANSLATE_NOOP("MainWindow", "Piercing")},
	BitField{0x4000, QT_TRANSLATE_NOOP("MainWindow", "Direct Attack")},
	BitField{0x8000, QT_TRANSLATE_NOOP("MainWindow", "Multi Attack")},
	BitField{0x10000, QT_TRANSLATE_NOOP("MainWindow", "Negate Activation")},
	BitField{0x20000, QT_TRANSLATE_NOOP("MainWindow", "Negate Effect")},
	BitField{0x40000, QT_TRANSLATE_NOOP("MainWindow", "Damage LP")},
	BitField{0x80000, QT_TRANSLATE_NOOP("MainWindow", "Recover LP")},
	BitField{0x100000, QT_TRANSLATE_NOOP("MainWindow", "Special Summon")},
	BitField{0x200000, QT_TRANSLATE_NOOP("MainWindow", "Non-effect-related")},
	BitField{0x400000, QT_TRANSLATE_NOOP("MainWindow", "Token-related")},
	BitField{0x800000, QT_TRANSLATE_NOOP("MainWindow", "Fusion-related")},
	BitField{0x1000000, QT_TRANSLATE_NOOP("MainWindow", "Ritual-related")},
	BitField{0x2000000, QT_TRANSLATE_NOOP("MainWindow", "Synchro-related")},
	BitField{0x4000000, QT_TRANSLATE_NOOP("MainWindow", "Xyz-related")},
	BitField{0x8000000, QT_TRANSLATE_NOOP("MainWindow", "Link-related")},
	BitField{0x10000000, QT_TRANSLATE_NOOP("MainWindow", "Counter-related")},
	BitField{0x20000000, QT_TRANSLATE_NOOP("MainWindow", "Gamble")},
	BitField{0x40000000, QT_TRANSLATE_NOOP("MainWindow", "Control")},
	BitField{0x80000000, QT_TRANSLATE_NOOP("MainWindow", "Move Zones")},
};
// clang-format on

QString const SQL_DB_DRIVER("QSQLITE");

std::array SQL_DATAS_TABLE_FIELDS = {
	QString{"PRAGMA table_info('datas');"},
	QString{
		"alias0,atk0,attribute0,category0,def0,id1,level0,ot0,race0,setcode0,"
		"type0"},
};

std::array SQL_TEXTS_TABLE_FIELDS = {
	QString{"PRAGMA table_info('texts');"},
	QString{"desc0,id1,name0,str10,str100,str110,str120,str130,str140,str150,"
            "str160,"
            "str20,str30,str40,str50,str60,str70,str80,str90"},
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

const QString SQL_QUERY_TABLE_FIELDS(R"(
PRAGMA table_info('texts');
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
	parent.setValidator(
		new QRegularExpressionValidator(QRegularExpression(regex), &parent));
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
	connect(ui->archeComboBox,
	        static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this,
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
	{
		ui->retranslateUi(this);
		auto retranslate_cbs = [&](auto const& fields, QListWidgetItem** cbs)
		{
			for(size_t i = 0; i < fields.size(); ++i)
				cbs[i]->setText(tr(fields[i].name));
		};
		retranslate_cbs(TYPE_FIELDS, typeCbs.get());
		retranslate_cbs(RACE_FIELDS, raceCbs.get());
		retranslate_cbs(ATTRIBUTE_FIELDS, attributeCbs.get());
		retranslate_cbs(SCOPE_FIELDS, scopeCbs.get());
		retranslate_cbs(CATEGORY_FIELDS, categoryCbs.get());
	}
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
	Q_ASSERT(db.open());
	QSqlQuery(SQL_QUERY_CREATE_DATAS_TABLE, db);
	QSqlQuery(SQL_QUERY_CREATE_TEXTS_TABLE, db);
	fillCardList();
	enableEditing(true);
}

void MainWindow::openDatabase()
{
	auto does_db_have_correct_format = [&](QSqlDatabase& db)
	{
		auto verify_table =
			[&](const auto& query_and_result)
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
			columns.sort();
			return columns.join(',').compare(query_and_result[1],
			                                 Qt::CaseInsensitive) == 0;
		};
		return verify_table(SQL_DATAS_TABLE_FIELDS) &&
		       verify_table(SQL_TEXTS_TABLE_FIELDS);
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
	updateUiWithCode(index.sibling(index.row(), 0).data().toUInt());
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
	Q_ASSERT(q1.prepare(SQL_QUERY_DATA));
	q1.bindValue(0, code);
	bool const q1result = q1.exec() && q1.first();
	Q_ASSERT(q1result);
	QSqlQuery q2(db);
	Q_ASSERT(q2.prepare(SQL_QUERY_TEXT));
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
	Q_ASSERT(q1.prepare(SQL_DELETE_DATA));
	QSqlQuery q2(db);
	Q_ASSERT(q2.prepare(SQL_DELETE_TEXT));
	QSqlQuery q3(db);
	Q_ASSERT(q3.prepare(SQL_INSERT_DATA));
	QSqlQuery q4(db);
	Q_ASSERT(q4.prepare(SQL_INSERT_TEXT));
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
