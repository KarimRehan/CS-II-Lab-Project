#include "LoginWindow.hpp"
#include "SignUpDialog.hpp"
#include "MainWindow.hpp"
#include <QVBoxLayout>
#include <QLabel>
#include "AuthManager.hpp"
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("Auto-detecting calendar");
  resize(500, 400);

  QWidget *centralWidget = new QWidget(this);
  QVBoxLayout *layout = new QVBoxLayout(centralWidget);
  layout->setContentsMargins(50, 50, 50, 50);
  layout->setSpacing(15);

  QLabel *title = new QLabel("Calendar Login", this);
  title->setAlignment(Qt::AlignCenter);

  usernameInput = new QLineEdit(this);
  usernameInput->setPlaceholderText("Username/Email");

  passwordInput = new QLineEdit(this);
  passwordInput->setPlaceholderText("Password");
  passwordInput->setEchoMode(QLineEdit::Password);

  loginBtn = new QPushButton("Login", this);
  signUpBtn = new QPushButton("Sign Up", this);

  layout->addWidget(title);
  layout->addWidget(new QLabel("Username/Email", this));
  layout->addWidget(usernameInput);
  layout->addWidget(new QLabel("Password", this));
  layout->addWidget(passwordInput);
  layout->addWidget(loginBtn);
  layout->addWidget(signUpBtn);

  setCentralWidget(centralWidget);

  connect(signUpBtn, &QPushButton::clicked, this, &LoginWindow::onSignUpClicked);
  connect(loginBtn, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
}

void LoginWindow::onSignUpClicked() {
  SignUpDialog dialog(this);
  dialog.exec();
}

void LoginWindow::onLoginClicked() {
  QString email = usernameInput->text();
  QString password = passwordInput->text();

  if (AuthManager::instance().validateUser(email, password)) {
	MainWindow *mainWindow = new MainWindow();
	mainWindow->show();
	this->close();
  } else {
	QMessageBox::critical(this, "Login Failed", "Invalid Email or Password.\nPlease try again or sign up.");
  }
}
