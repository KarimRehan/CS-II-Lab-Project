#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QString>
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

using namespace std;

class NetworkManager : public QObject {
  Q_OBJECT

public:
  explicit NetworkManager(QObject *parent = nullptr);
  ~NetworkManager();

  void syncWithGmail(const QString& email, const QString& appPassword);

signals:
  void syncSuccess(const QString& data);
  void syncError(const QString& errorMessage);

private:
  bool m_isRunning;
};

#endif
