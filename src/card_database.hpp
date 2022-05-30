#ifndef CARD_DATABASE_HPP
#define CARD_DATABASE_HPP
#include <memory>

class CardDatabase final
{
	Q_OBJECT
public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	void changeEvent(QEvent *event) override;
private slots:
	void toSpanish();
	void toEnglish();
private:
	std::unique_ptr<QTranslator> spanishTranslator;
	std::unique_ptr<Ui::MainWindow> ui;
};

#endif // CARD_DATABASE_HPP
