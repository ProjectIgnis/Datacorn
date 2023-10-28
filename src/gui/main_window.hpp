#ifndef GUI_MAIN_WINDOW_HPP
#define GUI_MAIN_WINDOW_HPP
#include <QMainWindow>
#include <memory>

QT_BEGIN_NAMESPACE
class QTranslator;
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

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
	void closeDatabase(int index);

	void newCard();
	void saveData();
	void deleteData();

	void openHomepage();
private:
	std::unique_ptr<QTranslator> const spanishTranslator;
	std::unique_ptr<Ui::MainWindow> const ui;

	void enableEditing(bool editing);
};

#endif // GUI_MAIN_WINDOW_HPP
