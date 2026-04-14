#include "SignUpDialog.hpp"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include "AuthManager.hpp"

SignUpDialog::SignUpDialog(QWidget *parent) : QDialog(parent) {
  setWindowTitle("Create New Account");
  resize(350, 400);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setSpacing(10);

  QLabel *title = new QLabel("Create New Account", this);
  title->setAlignment(Qt::AlignCenter);

  fullNameInput = new QLineEdit(this);
  emailInput = new QLineEdit(this);

  passwordInput = new QLineEdit(this);
  passwordInput->setEchoMode(QLineEdit::Password);

  confirmPasswordInput = new QLineEdit(this);
  confirmPasswordInput->setEchoMode(QLineEdit::Password);

  QPushButton *createBtn = new QPushButton("Create Account", this);

  layout->addWidget(title);
  layout->addWidget(new QLabel("Full Name", this));
  layout->addWidget(fullNameInput);
  layout->addWidget(new QLabel("Email Address", this));
  layout->addWidget(emailInput);
  layout->addWidget(new QLabel("Password", this));
  layout->addWidget(passwordInput);
  layout->addWidget(new QLabel("Confirm Password", this));
  layout->addWidget(confirmPasswordInput);
  layout->addSpacing(15);
  layout->addWidget(createBtn);

  connect(createBtn, &QPushButton::clicked, this, &SignUpDialog::onCreateAccount);
}

void SignUpDialog::onCreateAccount() {
  QString email = emailInput->text();
  QString password = passwordInput->text();
  QString confirm = confirmPasswordInput->text();

  if (email.isEmpty() || password.isEmpty()) {
	QMessageBox::warning(this, "Error", "Email and Password cannot be empty!");
	return;
  }

  if (password != confirm) {
	QMessageBox::warning(this, "Error", "Passwords do not match!");
	return;
  }

  if (AuthManager::instance().registerUser(email, password)) {
	QMessageBox::information(this, "Success", "Account created successfully!\nYou can now log in.");
	accept();
  } else {
	QMessageBox::warning(this, "Error", "An account with this email already exists!");
  }
}
