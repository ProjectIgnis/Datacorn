#ifndef GUI_MAIN_WINDOW_HPP
#define GUI_MAIN_WINDOW_HPP
#include <QMainWindow>
#include <memory>

QT_BEGIN_NAMESPACE
class QListWidgetItem;
class QTranslator;
class QTreeWidgetItem;
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class FilteringHeader;

class MainWindow final : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow() override;

	void changeEvent(QEvent* event) override;
private slots:
	void newDatabase();
	void openDatabase();
	void closeDatabase();

	void saveData();

	void toEnglish();
	void toSpanish();

	void openHomepage();

	void addArchetypeToList(bool clicked);
	void removeArchetypeFromList(bool clicked);
	void onArcheListItemChanged(QListWidgetItem* current,
	                            QListWidgetItem* previous);

	void onCardsListItemActivated(QModelIndex const& index);

private:
	std::unique_ptr<QTranslator> spanishTranslator;
	std::unique_ptr<Ui::MainWindow> ui;
	FilteringHeader* cardListFilter;
	std::unique_ptr<QListWidgetItem*[]> typeCbs;
	std::unique_ptr<QListWidgetItem*[]> raceCbs;
	std::unique_ptr<QListWidgetItem*[]> attributeCbs;
	std::unique_ptr<QListWidgetItem*[]> scopeCbs;
	std::unique_ptr<QListWidgetItem*[]> categoryCbs;

	QString formatArchetype(quint16 code, char const* name) const;
	void addArchetype(quint16 code);

	bool checkAndAskToCloseDb();
	void enableEditing(bool editing);

	void fillCardList();
	void updateUiWithCode(quint32 code);
	void updateCardWithUi();
};

#endif // GUI_MAIN_WINDOW_HPP
