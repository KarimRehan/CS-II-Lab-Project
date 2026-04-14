#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <vector>
#include "Event.hpp"
#include "NetworkManager.hpp"

using namespace std;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);

private slots:
  void onAddEventClicked();
  void onSyncClicked();
  void onSyncSuccess(const QString& data);
  void onSyncError(const QString& error);

private:
  void refreshEventList();

  QListWidget *eventListWidget;
  QLineEdit *smartInputEdit;
  QComboBox *categoryCombo;
  QPushButton *addBtn;
  QPushButton *syncBtn;

  vector<Event> m_events;
  NetworkManager *m_networkManager;
};

#endif
