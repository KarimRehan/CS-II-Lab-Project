#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <QString>
#include <QMap>
#include <QFile>
#include <QTextStream>
#include <QStringList>

class AuthManager {
public:
    static AuthManager& instance() {
        static AuthManager instance;
        return instance;
    }

    bool registerUser(const QString& email, const QString& password) {
        if (users.contains(email)) {
            return false;
        }
        users[email] = password;
        saveUsersToFile();
        return true;
    }

    bool validateUser(const QString& email, const QString& password) {
        return users.contains(email) && users[email] == password;
    }

private:
    AuthManager() {
        loadUsersFromFile();
    }

    QMap<QString, QString> users;
    const QString databasePath = "users_database.txt";

    void saveUsersToFile() {
        QFile file(databasePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            for (auto it = users.constBegin(); it != users.constEnd(); ++it) {
                out << it.key() << "," << it.value() << "\n";
            }
            file.close();
        }
    }

    void loadUsersFromFile() {
        QFile file(databasePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine();
                QStringList parts = line.split(",");
                if (parts.size() == 2) {
                    users[parts[0]] = parts[1];
                }
            }
            file.close();
        }
    }
};

#endif
