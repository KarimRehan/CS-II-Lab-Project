#include <QApplication>
#include "LoginWindow.hpp"

int main(int argc, char *argv[]) {
QApplication app(argc, argv);
LoginWindow login;
login.show();

return app.exec();
}
