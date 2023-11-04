#ifndef GUI_MAIN_WINDOW_HPP
#define GUI_MAIN_WINDOW_HPP
#include <QMainWindow>
#include <QMap>
#include <memory>

QT_BEGIN_NAMESPACE
class QSqlDatabase;
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
	void closeDatabase(int index);

	void newCard();
	void saveData();
	void deleteData();

	void copySelectedCards();
	void pasteClipboardCards();

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

	static void copyCards(QVector<quint32> const& codes, QSqlDatabase& dbSrc,
	                      QSqlDatabase& dbDst);

	DatabaseEditorWidget& currentTab() const;
	DatabaseEditorWidget& widgetFromConnection(
		QString const& dbConnection) const;
	QSqlDatabase clipboardDatabase() const;
	void setupCleanDB(QSqlDatabase& db) const;

	void addTab(QString const& file);
	void enableEditing(bool editing);
};

#endif // GUI_MAIN_WINDOW_HPP
