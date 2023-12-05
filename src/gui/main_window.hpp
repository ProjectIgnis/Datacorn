#ifndef GUI_MAIN_WINDOW_HPP
#define GUI_MAIN_WINDOW_HPP
#include <QMainWindow>
#include <QMap>
#include <memory>

QT_BEGIN_NAMESPACE
class QSqlDatabase;
class QSqlQuery;
class QTranslator;
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class DatabaseEditorWidget;

class MainWindow final : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow() override;

protected:
	void changeEvent(QEvent* event) override;
	void closeEvent(QCloseEvent* event) override;

private slots:
	void languageChanged(QAction* action);

	void newDatabase();
	void openDatabase();
	void showClipboardDatabase();
	void closeSelectedDatabase();
	void closeTabDatabase(int index);

	void newCard();
	void saveData();

	void cutSelectedCards();
	void copySelectedCards();
	void pasteClipboardCards();
	void deleteSelectedCards();

	void openHomepage();

protected:
	bool eventFilter(QObject* obj, QEvent* event) override;

private:
	void createLanguageMenu();
	void loadLanguage(const QString& newLanguage);

	std::shared_ptr<QTranslator> currentTranslator;
	std::unique_ptr<Ui::MainWindow> const ui;

	QMap<QString, std::shared_ptr<QTranslator>> translations;
	QString currLang;

	static QString stmtFromCodes(QVector<quint32> const& codes,
	                             QString const& baseStmt);
	static QSqlQuery queryCards(QVector<quint32> const& codes,
	                            QSqlDatabase& dbSrc);
	static void copyCards(QVector<quint32> const& codes, QSqlDatabase& dbSrc,
	                      QSqlDatabase& dbDst);
	static void deleteCards(QVector<quint32> const& codes, QSqlDatabase& dbSrc);

	QString printCardsForConfirm(QSqlQuery& q) const;
	DatabaseEditorWidget& currentTab() const;
	DatabaseEditorWidget& widgetFromConnection(
		QString const& dbConnection) const;
	QSqlDatabase clipboardDatabase(bool clear = false) const;
	void setupCleanDatabase(QSqlDatabase& db) const;

	void closeDatabaseImpl(int index, bool askForUnsavedData);
	void addTab(QString const& file);
	void enableEditing(bool editing);
};

#endif // GUI_MAIN_WINDOW_HPP
