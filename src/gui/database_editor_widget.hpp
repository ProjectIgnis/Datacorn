#ifndef GUI_DATABASE_EDITOR_WIDGET_HPP
#define GUI_DATABASE_EDITOR_WIDGET_HPP
#include <QVector>
#include <QWidget>
#include <memory>

QT_BEGIN_NAMESPACE
class QListWidgetItem;
class QSqlDatabase;
class QTabWidget;
namespace Ui
{
class DatabaseEditorWidget;
}
QT_END_NAMESPACE

class FilteringHeader;

class DatabaseEditorWidget final : public QWidget
{
	Q_OBJECT
public:
	explicit DatabaseEditorWidget(QTabWidget& parent,
	                              QString const& dbConnection);
	~DatabaseEditorWidget() override;

	void changeEvent(QEvent* event) override;

	QSqlDatabase database() const;
	QString tabName() const;
	QVector<quint32> selectedCards() const;

	bool hasUnsavedData() const;

public slots:
	void newCard();
	void saveData();
	void deleteData();

private slots:
	void addArchetypeToList(bool clicked);
	void removeArchetypeFromList(bool clicked);
	void onArcheListItemChanged(QListWidgetItem* current,
	                            QListWidgetItem* previous);
	void onArcheComboIndexActivated(int index);
	void onArcheComboEditTextChanged(QString const& text);

	void onCardsListItemActivated(QModelIndex const& index);

	void setUnsaved();

private:
	QTabWidget& parent;
	std::unique_ptr<Ui::DatabaseEditorWidget> ui;
	QString const dbConnection;
	bool unsavedData;
	FilteringHeader* cardListFilter;
	int stringsRowCount;
	quint32 previousCode;
	bool customArchetype;
	std::unique_ptr<QListWidgetItem*[]> typeCbs;
	std::unique_ptr<QListWidgetItem*[]> raceCbs;
	std::unique_ptr<QListWidgetItem*[]> attributeCbs;
	std::unique_ptr<QListWidgetItem*[]> scopeCbs;
	std::unique_ptr<QListWidgetItem*[]> categoryCbs;

	void enableEditing(bool editing);

	QString formatArchetype(quint16 code, char const* name) const;
	void addArchetype(quint16 code, bool ignoreUnsavedState = false);
	void retranslateArchetypes();

	void fillCardList(QSqlDatabase& db);
	void updateUiWithCode(quint32 code);
	void updateCardWithUi();
	void setSaved();

	bool cardExists(QSqlDatabase& db, quint32 code) const;
	void removeCard(QSqlDatabase& db, quint32 code);
};

#endif // GUI_DATABASE_EDITOR_WIDGET_HPP
