#include "database_editor_widget.hpp"

#include <QCompleter>
#include <QDateTime>
#include <QMessageBox>
#include <QRegularExpressionValidator>
#include <QScreen>
#include <QScrollBar>
#include <QShortcut>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QString>
#include <QStringList>
#include <array>
#include <cstring> // std::memcpy
#include <ui_database_editor_widget.h>

#include "../archetypes.hpp"
#include "new_card_dialog.hpp"
#include "sql_util.hpp"
#include "trim_on_paste_line_edit.hpp"

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
	BitField{0x1, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Monster")},
	BitField{0x2, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Spell")},
	BitField{0x4, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Trap")},
	BitField{0x10, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Normal")},
	BitField{0x20, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Effect")},
	BitField{0x40, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Fusion")},
	BitField{0x80, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Ritual")},
	BitField{0x100, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Trap Monster")},
	BitField{0x200, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Spirit")},
	BitField{0x400, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Union")},
	BitField{0x800, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Gemini")},
	BitField{0x1000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Tuner")},
	BitField{0x2000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Synchro")},
	BitField{0x4000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Token")},
	BitField{0x8000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Maximum")},
	BitField{0x10000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Quick-Play")},
	BitField{0x20000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Continuous")},
	BitField{0x40000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Equip")},
	BitField{0x80000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Field")},
	BitField{0x100000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Counter")},
	BitField{0x200000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Flip")},
	BitField{0x400000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Toon")},
	BitField{0x800000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Xyz")},
	BitField{0x1000000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Pendulum")},
	BitField{0x2000000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Special Summon")},
	BitField{0x4000000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Link")},
	BitField{0x8000000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Skill")},
	BitField{0x10000000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Action")},
	BitField{0x20000000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Plus")},
	BitField{0x40000000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Minus")},
	BitField{0x80000000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Armor")},
};
constexpr std::array const RACE_FIELDS{
	BitField{0x1, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Warrior")},
	BitField{0x2, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Spellcaster")},
	BitField{0x4, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Fairy")},
	BitField{0x8, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Fiend")},
	BitField{0x10, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Zombie")},
	BitField{0x20, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Machine")},
	BitField{0x40, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Aqua")},
	BitField{0x80, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Pyro")},
	BitField{0x100, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Rock")},
	BitField{0x200, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Winged-Beast")},
	BitField{0x400, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Plant")},
	BitField{0x800, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Insect")},
	BitField{0x1000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Thunder")},
	BitField{0x2000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Dragon")},
	BitField{0x4000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Beast")},
	BitField{0x8000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Beast-Warrior")},
	BitField{0x10000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Dinosaur")},
	BitField{0x20000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Fish")},
	BitField{0x40000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Sea-Serpent")},
	BitField{0x80000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Reptile")},
	BitField{0x100000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Psychic")},
	BitField{0x200000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Divine")},
	BitField{0x400000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Creator God")},
	BitField{0x800000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Wyrm")},
	BitField{0x1000000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Cyberse")},
	BitField{0x2000000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Illusion")},
	BitField{0x4000000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Cyborg")},
	BitField{0x8000000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Magical Knight")},
	BitField{0x10000000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "High Dragon")},
	BitField{0x20000000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Omega Psychic")},
	BitField{0x40000000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Celestial Warrior")},
	BitField{0x80000000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Galaxy")},
};
constexpr std::array const ATTRIBUTE_FIELDS{
	BitField{0x1, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "EARTH")},
	BitField{0x2, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "WATER")},
	BitField{0x4, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "FIRE")},
	BitField{0x8, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "WIND")},
	BitField{0x10, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "LIGHT")},
	BitField{0x20, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "DARK")},
	BitField{0x40, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "DIVINE")},
};
constexpr std::array const SCOPE_FIELDS{
	BitField{0x1, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "OCG")},
	BitField{0x2, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "TCG")},
	BitField{0x4, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Anime")},
	BitField{0x8, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Illegal")},
	BitField{0x10, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Video Game")},
	BitField{0x20, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Custom")},
	BitField{0x40, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Speed")},
	BitField{0x100, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Pre-Release")},
	BitField{0x200, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Rush")},
	BitField{0x400, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Legend")},
	BitField{0x1000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Hidden")},
};
constexpr std::array const CATEGORY_FIELDS{
	BitField{0x1, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Destroy Monster")},
	BitField{0x2, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Destroy S/T")},
	BitField{0x4, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Destroy Deck")},
	BitField{0x8, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Destroy Hand")},
	BitField{0x10, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Send to GY")},
	BitField{0x20, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Send to Hand")},
	BitField{0x40, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Send to Deck")},
	BitField{0x80, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Banish")},
	BitField{0x100, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Draw")},
	BitField{0x200, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Search")},
	BitField{0x400, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Change ATK/DEF")},
	BitField{0x800, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Change Level/Rank")},
	BitField{0x1000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Position")},
	BitField{0x2000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Piercing")},
	BitField{0x4000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Direct Attack")},
	BitField{0x8000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Multi Attack")},
	BitField{0x10000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Negate Activation")},
	BitField{0x20000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Negate Effect")},
	BitField{0x40000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Damage LP")},
	BitField{0x80000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Recover LP")},
	BitField{0x100000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Special Summon")},
	BitField{0x200000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Non-effect-related")},
	BitField{0x400000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Token-related")},
	BitField{0x800000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Fusion-related")},
	BitField{0x1000000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Ritual-related")},
	BitField{0x2000000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Synchro-related")},
	BitField{0x4000000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Xyz-related")},
	BitField{0x8000000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Link-related")},
	BitField{0x10000000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Counter-related")},
	BitField{0x20000000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Gamble")},
	BitField{0x40000000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Control")},
	BitField{0x80000000, QT_TRANSLATE_NOOP("DatabaseEditorWidget", "Move Zones")},
};
// clang-format on

QString const SQL_QUERY_FIRST_ROW_CODE(R"(
SELECT id FROM datas ORDER BY ROWID ASC LIMIT 1;
)");

QString const SQL_QUERY_CODE_EXISTS(R"(
SELECT EXISTS(SELECT 1 FROM datas WHERE datas.id = ?);
)");

QString const SQL_QUERY_DATA(R"(
SELECT alias,setcode,type,atk,def,level,race,attribute,ot,category
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
	Q_OBJECT
public:
	explicit CardCodeNameSqlModel(QWidget* parent, QSqlDatabase const& db)
		: QSqlTableModel(parent, db)
	{
		setTable("texts");
		setEditStrategy(QSqlTableModel::OnManualSubmit);
		select();
		setHeaderData(0, Qt::Horizontal, tr("Code"));
		setHeaderData(1, Qt::Horizontal, tr("Name"));
	}

	~CardCodeNameSqlModel() override = default;

private:
	QString selectStatement() const override
	{
		return QString(R"(SELECT "id", "name" FROM "texts" WHERE )") + filter();
	}
};

} // namespace

class FilteringHeader final : public QHeaderView
{
	Q_OBJECT
public:
	explicit FilteringHeader(QTableView& parent)
		: QHeaderView(Qt::Horizontal, &parent), model(nullptr)
	{
		setSortIndicatorShown(false);
		setSectionsClickable(false);
		for(int i = 0; i < 2; i++)
		{
			auto* w = new TrimOnPasteLineEdit(this);
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

	QSqlTableModel* getModel() const { return model; }

	void setModel(QSqlTableModel* newModel)
	{
		if((model = newModel) == nullptr)
			return;
		updateTableFilters();
		model->select();
	}
private slots:
	void adjustFilters()
	{
		int section = 0;
		for(auto* f : std::as_const(filters))
		{
			int const y = sectionSizeFromContents(section).height();
			f->move(sectionPosition(section) - offset(), y);
			f->resize(sectionSize(section), f->sizeHint().height());
			++section;
		}
	}

	void updateTableFilters(QString const& text = "")
	{
		Q_UNUSED(text);
		if(model == nullptr)
			return;
		model->setFilter(QString(R"("id" LIKE "%%1%" AND "name" LIKE "%%2%")")
		                     .arg(filters[0]->text(), filters[1]->text()));
	}

private:
	QList<QLineEdit*> filters;
	QSqlTableModel* model;
};

// public

DatabaseEditorWidget::DatabaseEditorWidget(QTabWidget& parent,
                                           QString const& dbConnection)
	: QWidget(&parent)
	, parent(parent)
	, ui(std::make_unique<Ui::DatabaseEditorWidget>())
	, dbConnection(dbConnection)
	, unsavedData(false)
	, cardListFilter(nullptr) // Must be fully init'd later due to "setupUi".
	, stringsRowCount(0)      // Must be fully init'd later due to "setupUi".
	, previousCode(0)
	, customArchetype(false)
{
	// UI setup
	ui->setupUi(this);
	connect(ui->addArcheButton, &QPushButton::clicked, this,
	        &DatabaseEditorWidget::addArchetypeToList);
	connect(ui->removeArcheButton, &QPushButton::clicked, this,
	        [&](auto...) { removeArchetypeFromList(); });
	connect(ui->archeList, &QListWidget::currentItemChanged, this,
	        &DatabaseEditorWidget::onArcheListItemChanged);
	auto* shortcut = new QShortcut(QKeySequence(Qt::Key_Delete), ui->archeList);
	connect(shortcut, &QShortcut::activated, this,
	        &DatabaseEditorWidget::removeArchetypeFromList);
	connect(ui->cardCodeNameList, &QAbstractItemView::clicked, this,
	        &DatabaseEditorWidget::onCardsListItemClicked);
	connect(ui->archeComboBox,
	        static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this,
	        &DatabaseEditorWidget::onArcheComboIndexActivated);
	connect(ui->archeComboBox, &QComboBox::editTextChanged, this,
	        &DatabaseEditorWidget::onArcheComboEditTextChanged);
	setRegexValidator(*ui->passLineEdit, "[0-9]+");
	setRegexValidator(*ui->aliasLineEdit, "[0-9]+");
	connect(ui->passLineEdit, &QLineEdit::textEdited, this,
	        &DatabaseEditorWidget::setUnsaved);
	connect(ui->aliasLineEdit, &QLineEdit::textEdited, this,
	        &DatabaseEditorWidget::setUnsaved);
	connect(ui->nameLineEdit, &QLineEdit::textEdited, this,
	        &DatabaseEditorWidget::setUnsaved);
	connect(ui->descPlainTextEdit, &QPlainTextEdit::textChanged, this,
	        &DatabaseEditorWidget::setUnsaved);
	auto const spinBoxValueChanged =
		static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged);
	connect(ui->atkSpinBox, spinBoxValueChanged, this,
	        &DatabaseEditorWidget::setUnsaved);
	connect(ui->defSpinBox, spinBoxValueChanged, this,
	        &DatabaseEditorWidget::setUnsaved);
	connect(ui->levelSpinBox, spinBoxValueChanged, this,
	        &DatabaseEditorWidget::setUnsaved);
	connect(ui->lScaleSpinBox, spinBoxValueChanged, this,
	        &DatabaseEditorWidget::setUnsaved);
	connect(ui->rScaleSpinBox, spinBoxValueChanged, this,
	        &DatabaseEditorWidget::setUnsaved);
	connect(ui->markerBottomLeftButton, &QPushButton::toggled, this,
	        &DatabaseEditorWidget::setUnsaved);
	connect(ui->markerBottomButton, &QPushButton::toggled, this,
	        &DatabaseEditorWidget::setUnsaved);
	connect(ui->markerBottomRightButton, &QPushButton::toggled, this,
	        &DatabaseEditorWidget::setUnsaved);
	connect(ui->markerLeftButton, &QPushButton::toggled, this,
	        &DatabaseEditorWidget::setUnsaved);
	connect(ui->markerRightButton, &QPushButton::toggled, this,
	        &DatabaseEditorWidget::setUnsaved);
	connect(ui->markerTopLeftButton, &QPushButton::toggled, this,
	        &DatabaseEditorWidget::setUnsaved);
	connect(ui->markerTopButton, &QPushButton::toggled, this,
	        &DatabaseEditorWidget::setUnsaved);
	connect(ui->markerTopRightButton, &QPushButton::toggled, this,
	        &DatabaseEditorWidget::setUnsaved);
	connect(ui->stringsTableWidget, &QTableWidget::cellChanged, this,
	        &DatabaseEditorWidget::setUnsaved);
	cardListFilter = new FilteringHeader(*ui->cardCodeNameList);
	ui->cardCodeNameList->setHorizontalHeader(cardListFilter);
	stringsRowCount = ui->stringsTableWidget->rowCount();
	{
		QStringList stringsRownNames;
		for(int i = 0; i < stringsRowCount; i++)
			stringsRownNames.append(QString::number(i));
		ui->stringsTableWidget->setVerticalHeaderLabels(stringsRownNames);
	}
	auto populate_cbs = [&](QListWidget* itemParent, auto const& fields)
	{
		using Item = QListWidgetItem;
		std::unique_ptr<Item*[]> boxes(new Item*[fields.size()]);
		for(size_t i = 0; i < fields.size(); ++i)
		{
			auto* item = new Item(tr(fields[i].name), itemParent);
			item->setFlags(Qt::ItemIsEnabled | Qt::ItemNeverHasChildren);
			setChecked(item, false);
			boxes[i] = item;
		}
		connect(itemParent, &QListWidget::itemClicked, this,
		        &DatabaseEditorWidget::onListCheckboxClicked);
		connect(itemParent, &QListWidget::itemChanged, this,
		        &DatabaseEditorWidget::setUnsaved);
		return boxes;
	};
	typeCbs = populate_cbs(ui->typesWidget, TYPE_FIELDS);
	raceCbs = populate_cbs(ui->racesWidget, RACE_FIELDS);
	attributeCbs = populate_cbs(ui->attributesWidget, ATTRIBUTE_FIELDS);
	scopeCbs = populate_cbs(ui->scopesWidget, SCOPE_FIELDS);
	categoryCbs = populate_cbs(ui->categoriesWidget, CATEGORY_FIELDS);
	ui->archeComboBox->blockSignals(true);
	ui->archeComboBox->lineEdit()->setPlaceholderText(
		tr("Select or type an archetype to add"));
	auto const end = ARCHETYPES_MAP.constEnd();
	for(auto it = ARCHETYPES_MAP.constBegin(); it != end; ++it)
	{
		ui->archeComboBox->addItem(formatArchetype(it.key(), it.value()));
		ui->archeComboBox->setItemData(ui->archeComboBox->count() - 1, it.key(),
		                               ARCHETYPE_ROLE);
	}
	ui->archeComboBox->setCurrentIndex(-1);
	ui->archeComboBox->completer()->setCaseSensitivity(Qt::CaseInsensitive);
	ui->archeComboBox->completer()->setCompletionMode(
		QCompleter::PopupCompletion);
	ui->archeComboBox->completer()->setFilterMode(Qt::MatchContains);
	ui->archeComboBox->blockSignals(false);
	// DB setup
	auto db = QSqlDatabase::database(dbConnection, false);
	// NOTE: Yes, very ugly, but beats complicated logic to associate the
	// database connection with the editor widget.
	{
		static_assert(sizeof(qulonglong) >= sizeof(this));
		qulonglong pass{};
		auto* ptr = this;
		std::memcpy(&pass, &ptr, sizeof(this));
		db.setPassword(QString("%1").arg(pass, 0, 16));
	}
	fillCardList(db);
	auto q = buildQuery(db, SQL_QUERY_FIRST_ROW_CODE);
	execQuery(q);
	if(q.first())
		updateUiWithCode(q.value(0).toUInt());
}

DatabaseEditorWidget::~DatabaseEditorWidget()
{}

void DatabaseEditorWidget::changeEvent(QEvent* event)
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
		retranslateArchetypes();
	}
	else
		QWidget::changeEvent(event);
}

QSqlDatabase DatabaseEditorWidget::database() const
{
	auto db = QSqlDatabase::database(dbConnection, false);
	Q_ASSERT(db.isValid());
	return db;
}

QString DatabaseEditorWidget::tabName(bool ignoreUnsavedState) const
{
	QString name;
	if(dbConnection == SQL_CLIPBOARD_CONN)
		name = tr("Clipboard");
	else
		name = dbConnection.split('/').last();
	if(unsavedData && !ignoreUnsavedState)
		name.append('*');
	return name;
}

QVector<quint32> DatabaseEditorWidget::selectedCards() const
{
	QVector<quint32> ret;
	auto const* selectionModel = ui->cardCodeNameList->selectionModel();
	auto const rows = selectionModel->selectedRows();
	for(auto const& index : rows)
		ret.append(index.data().toUInt());
	return ret;
}

bool DatabaseEditorWidget::hasUnsavedData() const
{
	return unsavedData;
}

// public slots

void DatabaseEditorWidget::newCard()
{
	if(hasUnsavedData() &&
	   QMessageBox::question(this, tr("Discard changes?"),
	                         tr("Creating a new card would discard current "
	                            "unsaved changes. Proceed anyways?")) !=
	       QMessageBox::Yes)
		return;
	auto db = QSqlDatabase::database(dbConnection, false);
	auto const r = NewCardDialog::display(db, previousCode != 0);
	if(r.dialogResult == QDialog::Rejected)
		return;
	if(!r.copy)
		updateUiWithCode(0);
	previousCode = 0; // NOTE: Used to avoid removing previous card, if any.
	ui->passLineEdit->setText(QString::number(r.newCode));
	updateCardWithUi();
	setSaved();
}

void DatabaseEditorWidget::saveData()
{
	updateCardWithUi();
	ui->cardCodeNameList->update();
	setSaved();
}

void DatabaseEditorWidget::refreshCardList()
{
	cardListFilter->getModel()->select();
}

// private slots

void DatabaseEditorWidget::addArchetypeToList([[maybe_unused]] bool clicked)
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
		ui->archeComboBox->clearEditText();
		ui->archeComboBox->setCurrentIndex(-1);
		return;
	}
	// Here we do a best effort to parse the ComboBox's current text and get
	// the archetype's code either in decimal or hexadecimal.
	auto const text = ui->archeComboBox->currentText().split('|')[0];
	for(auto base : {10, 16})
	{
		bool ok;
		auto setcode = text.toUShort(&ok, base);
		if(ok && setcode != 0)
		{
			addArchetype(setcode);
			ui->archeComboBox->clearEditText();
			ui->archeComboBox->setCurrentIndex(-1);
			return;
		}
	}
	QMessageBox::critical(
		this, tr("Couldn't Parse Archetype"),
		tr("The currently set archetype could not be parsed either in decimal "
	       "or hexadecimal format. Either fix the format or select one of the "
	       "preset archetypes from the list."));
}

void DatabaseEditorWidget::removeArchetypeFromList()
{
	Q_ASSERT(ui->archeList->currentItem() != nullptr);
	delete ui->archeList->takeItem(
		ui->archeList->row(ui->archeList->currentItem()));
	setUnsaved();
}

void DatabaseEditorWidget::onArcheListItemChanged(
	QListWidgetItem* current, [[maybe_unused]] QListWidgetItem* previous)
{
	ui->removeArcheButton->setEnabled(current != nullptr);
}

void DatabaseEditorWidget::onArcheComboIndexActivated(
	[[maybe_unused]] int index)
{
	customArchetype = false;
	ui->addArcheButton->setEnabled(index >= 0);
}

void DatabaseEditorWidget::onArcheComboEditTextChanged(QString const& text)
{
	customArchetype = true;
	ui->addArcheButton->setEnabled(!text.isEmpty());
}

void DatabaseEditorWidget::onCardsListItemClicked(QModelIndex const& index)
{
	updateUiWithCode(index.sibling(index.row(), 0).data().toUInt());
}

void DatabaseEditorWidget::onListCheckboxClicked(QListWidgetItem* item)
{
	setChecked(item, item->checkState() != Qt::Checked);
}

void DatabaseEditorWidget::setUnsaved()
{
	if(unsavedData)
		return;
	unsavedData = true;
	parent.setTabText(parent.indexOf(this), tabName());
}

// private

QString DatabaseEditorWidget::formatArchetype(quint16 code,
                                              char const* name) const
{
	QString const ret(R"(0x%1 | %2)");
	auto const code_str = QString::number(code, 16).toUpper();
	return ret.arg(code_str, name == nullptr ? "???" : tr(name));
}

void DatabaseEditorWidget::addArchetype(quint16 code, bool ignoreUnsavedState)
{
	auto const search = ARCHETYPES_MAP.find(code);
	ui->archeList->addItem(formatArchetype(
		code, search != ARCHETYPES_MAP.constEnd() ? search.value() : nullptr));
	auto& item = *ui->archeList->item(ui->archeList->count() - 1);
	item.setData(ARCHETYPE_ROLE, code);
	if(!ignoreUnsavedState)
		setUnsaved();
}

void DatabaseEditorWidget::retranslateArchetypes()
{
	auto const end = ARCHETYPES_MAP.constEnd();
	auto i = 0;
	for(auto it = ARCHETYPES_MAP.constBegin(); it != end; ++it, ++i)
	{
		ui->archeComboBox->setItemText(i,
		                               formatArchetype(it.key(), it.value()));
	}

	for(int i = 0; i < ui->archeList->count(); ++i)
	{
		auto& item = *ui->archeList->item(i);
		auto code = item.data(ARCHETYPE_ROLE).toUInt();
		auto const search = ARCHETYPES_MAP.find(code);
		item.setText(
			formatArchetype(code, search != end ? search.value() : nullptr));
	}
}

void DatabaseEditorWidget::fillCardList(QSqlDatabase& db)
{
	auto* model = new CardCodeNameSqlModel(ui->cardCodeNameList, db);
	cardListFilter->setModel(model);
	ui->cardCodeNameList->setModel(model);
	ui->cardCodeNameList->resizeColumnsToContents();
}

void DatabaseEditorWidget::updateUiWithCode(quint32 code)
{
	// Prevent setting the unsaved state while we update the UI
	unsavedData = true;
	// Set internal code so we know which card to "move" from
	previousCode = code;
	// Helper function to quickly iterate comboboxes per bit of a value
	auto toggle_cbs =
		[&](quint64 bits, auto const& fields, QListWidgetItem** cbs)
	{
		for(size_t i = 0; i < fields.size(); ++i)
			setChecked(cbs[i], (bits & fields[i].value) != 0U);
	};
	// Clean the UI
	toggle_cbs(0U, TYPE_FIELDS, typeCbs.get());
	toggle_cbs(0U, RACE_FIELDS, raceCbs.get());
	toggle_cbs(0U, ATTRIBUTE_FIELDS, attributeCbs.get());
	toggle_cbs(0U, SCOPE_FIELDS, scopeCbs.get());
	toggle_cbs(0U, CATEGORY_FIELDS, categoryCbs.get());
	ui->passLineEdit->setText("0");
	ui->aliasLineEdit->setText("0");
	ui->nameLineEdit->setText("");
	ui->archeComboBox->clearEditText();
	ui->archeComboBox->setCurrentIndex(-1);
	ui->addArcheButton->setEnabled(false);
	ui->archeList->clear();
	ui->descPlainTextEdit->setPlainText("");
	ui->atkSpinBox->setEnabled(true);
	ui->atkSpinBox->setValue(ui->atkSpinBox->minimum());
	ui->defSpinBox->setEnabled(true);
	ui->defSpinBox->setValue(ui->defSpinBox->minimum());
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
	{
		unsavedData = false;
		return;
	}
	// Query data and strings
	auto db = database();
	auto q1 = buildQuery(db, SQL_QUERY_DATA);
	q1.bindValue(0, code);
	execQuery(q1, true);
	auto q2 = buildQuery(db, SQL_QUERY_TEXT);
	q2.bindValue(0, code);
	execQuery(q2, true);
	// Populate the fields with the new data and strings
	ui->passLineEdit->setText(QString::number(code));
	ui->aliasLineEdit->setText(q1.value(0).toString());
	{ // Setcode population
		static constexpr auto MAX_SETCODES = 4;
		quint64 const setcodes = q1.value(1).toULongLong();
		for(unsigned i = 0U; i < MAX_SETCODES; i++)
		{
			quint16 const setcode = (setcodes >> (i * 16U)) & 0xFFFFU;
			if(setcode == 0)
				continue;
			addArchetype(setcode, true);
		}
	}
	quint32 const type = q1.value(2).toUInt();
	toggle_cbs(type, TYPE_FIELDS, typeCbs.get());
	qint32 const atk = q1.value(3).toInt();
	ui->atkSpinBox->setValue(atk == QMARK_ATK_DEF ? ui->atkSpinBox->minimum()
	                                              : atk);
	if(qint32 const def = q1.value(4).toInt(); (type & TYPE_LINK) == 0U)
	{
		ui->defSpinBox->setValue(
			def == QMARK_ATK_DEF ? ui->defSpinBox->minimum() : def);
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
	quint32 const dbLevel = q1.value(5).toUInt();
	ui->levelSpinBox->setValue(dbLevel & 0x800000FF);
	ui->lScaleSpinBox->setValue((dbLevel >> 24U) & 0xFF);
	ui->rScaleSpinBox->setValue((dbLevel >> 16U) & 0xFF);
	toggle_cbs(q1.value(6).toUInt(), RACE_FIELDS, raceCbs.get());
	toggle_cbs(q1.value(7).toUInt(), ATTRIBUTE_FIELDS, attributeCbs.get());
	toggle_cbs(q1.value(8).toUInt(), SCOPE_FIELDS, scopeCbs.get());
	toggle_cbs(q1.value(9).toUInt(), CATEGORY_FIELDS, categoryCbs.get());
	ui->nameLineEdit->setText(q2.value(0).toString());
	ui->descPlainTextEdit->setPlainText(q2.value(1).toString());
	for(int i = 0; i < stringsRowCount; ++i)
	{
		auto& item = *ui->stringsTableWidget->item(i, 0);
		item.setText(q2.value(2 + i).toString());
	}
	unsavedData = false;
}

void DatabaseEditorWidget::updateCardWithUi()
{
	quint32 const newCode = ui->passLineEdit->text().toUInt();
	if(newCode == 0)
	{
		QMessageBox::warning(this, tr("Invalid passcode"),
		                     tr("Passcode cannot be 0 or empty."));
		return;
	}
	auto db = database();
	bool const targetCardExists = cardExists(db, newCode);
	if(previousCode != newCode && targetCardExists &&
	   QMessageBox::question(this, tr("Confirm Overwrite"),
	                         tr("The chosen card code already exists, do "
	                            "you wish to overwrite it?")) !=
	       QMessageBox::Yes)
		return;
	if(targetCardExists)
		removeCard(db, newCode);
	if(previousCode != 0)
		removeCard(db, previousCode);
	auto q1 = buildQuery(db, SQL_INSERT_DATA);
	auto q2 = buildQuery(db, SQL_INSERT_TEXT);
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
		{
			auto const def = ui->defSpinBox->value();
			return def == ui->defSpinBox->minimum() ? QMARK_ATK_DEF : def;
		}
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
		return (ui->levelSpinBox->value() & 0x800000FF) |
		       ((ui->lScaleSpinBox->value() & 0xFF) << 24U) |
		       ((ui->rScaleSpinBox->value() & 0xFF) << 16U);
	};
	q1.bindValue(0, newCode);
	q1.bindValue(1, ui->aliasLineEdit->text().toUInt());
	q1.bindValue(
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
	q1.bindValue(3, type);
	auto const atk = ui->atkSpinBox->value();
	q1.bindValue(4, atk == ui->atkSpinBox->minimum() ? QMARK_ATK_DEF : atk);
	q1.bindValue(5, compute_def_value());
	q1.bindValue(6, compute_level_value());
	q1.bindValue(7, compute_bitfield(RACE_FIELDS, raceCbs.get()));
	q1.bindValue(8, compute_bitfield(ATTRIBUTE_FIELDS, attributeCbs.get()));
	q1.bindValue(9, compute_bitfield(SCOPE_FIELDS, scopeCbs.get()));
	q1.bindValue(10, compute_bitfield(CATEGORY_FIELDS, categoryCbs.get()));
	execQuery(q1);
	// Insert strings
	q2.bindValue(0, newCode);
	q2.bindValue(1, ui->nameLineEdit->text());
	q2.bindValue(2, ui->descPlainTextEdit->toPlainText());
	int const stringsRowCount = ui->stringsTableWidget->rowCount();
	for(int i = 0; i < stringsRowCount; ++i)
	{
		auto& item = *ui->stringsTableWidget->item(i, 0);
		q2.bindValue(3 + i, item.text());
	}
	execQuery(q2);
	previousCode = newCode;
	refreshCardList(); // TODO: Properly list and track new code
}

void DatabaseEditorWidget::setSaved()
{
	unsavedData = false;
	parent.setTabText(parent.indexOf(this), tabName());
}

bool DatabaseEditorWidget::cardExists(QSqlDatabase& db, quint32 code) const
{
	auto q = buildQuery(db, SQL_QUERY_CODE_EXISTS);
	q.bindValue(0, code);
	execQuery(q, true);
	return q.value(0).toBool();
}

void DatabaseEditorWidget::removeCard(QSqlDatabase& db, quint32 code)
{
	// Remove data
	auto q1 = buildQuery(db, SQL_DELETE_DATA);
	q1.bindValue(0, code);
	execQuery(q1);
	// Remove strings
	auto q2 = buildQuery(db, SQL_DELETE_TEXT);
	q2.bindValue(0, code);
	execQuery(q2);
}

#include "database_editor_widget.moc"
