#ifndef GUI_MAIN_WINDOW_HPP
#define GUI_MAIN_WINDOW_HPP
#include <memory>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QTranslator;
class QTreeWidgetItem;
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow final : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	void changeEvent(QEvent *event) override;
private slots:
// 	void newDatabase();
	void openDatabase();
	void closeDatabase();

	void toSpanish();
	void toEnglish();
private:
	std::unique_ptr<QTranslator> spanishTranslator;
	std::unique_ptr<Ui::MainWindow> ui;
	std::unique_ptr<QCheckBox*[]> typeCbs;
	std::unique_ptr<QCheckBox*[]> raceCbs;
	std::unique_ptr<QCheckBox*[]> attributeCbs;
	std::unique_ptr<QCheckBox*[]> scopeCbs;
	std::unique_ptr<QCheckBox*[]> categoryCbs;

	bool checkAndAskToCloseDb();

	void updateUiWithCode(quint32 code);
	void updateCardWithUi();
};

#endif // GUI_MAIN_WINDOW_HPP
