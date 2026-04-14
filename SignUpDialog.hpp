#ifndef SIGNUPDIALOG_H
#define SIGNUPDIALOG_H

#include <QDialog>
#include <QLineEdit>

class SignUpDialog : public QDialog {
	Q_OBJECT

public:
	explicit SignUpDialog(QWidget *parent = nullptr);

private slots:
	void onCreateAccount();

private:
	QLineEdit *fullNameInput;
	QLineEdit *emailInput;
	QLineEdit *passwordInput;
	QLineEdit *confirmPasswordInput;
};

#endif
