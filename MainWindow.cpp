#include "MainWindow.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <algorithm>
#include <QStringList>

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Upcoming Tasks & Events - Smart Calendar");
    resize(800, 600);

    m_networkManager = new NetworkManager(this);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QHBoxLayout *topBarLayout = new QHBoxLayout();
    syncBtn = new QPushButton("Sync with Gmail", this);
    topBarLayout->addStretch();
    topBarLayout->addWidget(syncBtn);

    QHBoxLayout *addLayout = new QHBoxLayout();
    smartInputEdit = new QLineEdit(this);
    smartInputEdit->setPlaceholderText("Type an event (e.g. 'Project Meeting 28/04/2026 12:00')");

    categoryCombo = new QComboBox(this);
    categoryCombo->addItems({"University", "Personal", "Other"});

    addBtn = new QPushButton("Add Event", this);

    addLayout->addWidget(smartInputEdit);
    addLayout->addWidget(categoryCombo);
    addLayout->addWidget(addBtn);

    eventListWidget = new QListWidget(this);

    mainLayout->addLayout(topBarLayout);
    mainLayout->addLayout(addLayout);
    mainLayout->addWidget(eventListWidget);

    connect(addBtn, &QPushButton::clicked, this, &MainWindow::onAddEventClicked);
    connect(syncBtn, &QPushButton::clicked, this, &MainWindow::onSyncClicked);
    connect(m_networkManager, &NetworkManager::syncSuccess, this, &MainWindow::onSyncSuccess);
    connect(m_networkManager, &NetworkManager::syncError, this, &MainWindow::onSyncError);
}

void MainWindow::onAddEventClicked() {
    QString input = smartInputEdit->text();
    if (input.isEmpty()) return;

    Event newEvent = Event::parseEventFromString(input, static_cast<EventCategory>(categoryCombo->currentIndex()));

    if (newEvent.isValid()) {
	m_events.push_back(newEvent);
	sort(m_events.begin(), m_events.end());
	refreshEventList();
	smartInputEdit->clear();
  } else {
	QMessageBox::warning(this, "Invalid Format", "Could not detect a valid date. Please use DD/MM/YYYY Hr:Min");
  }
}

void MainWindow::onSyncClicked() {

	syncBtn->setEnabled(false);
	syncBtn->setText("Syncing...");
	QString myEmail = "karim.ie.rehan@gmail.com";
	QString myAppPassword = "qdkqgcszliohpfhs";

	m_networkManager->syncWithGmail(myEmail, myAppPassword);
}

void MainWindow::onSyncSuccess(const QString& data) {
  syncBtn->setEnabled(true);
  syncBtn->setText("Sync with Gmail");

  QStringList lines = data.split("\n", Qt::SkipEmptyParts);
  int eventsFound = 0;

  for (const QString& currentLine : lines) {
	Event newEvent = Event::parseEventFromString(currentLine, EventCategory::University);
	if (newEvent.isValid()) {
m_events.push_back(newEvent);
eventsFound++;
	}
  }

  if (eventsFound > 0) {
	sort(m_events.begin(), m_events.end());
	refreshEventList();
	QMessageBox::information(this, "Sync Complete", QString("Success! Found %1 event(s) in your last 3 emails.").arg(eventsFound));
  } else {
	QMessageBox::information(this, "Sync Complete", "Checked recent emails, but no valid dates were found.\n\nRaw Text:\n" + data);
  }
}

void MainWindow::onSyncError(const QString& error) {
  syncBtn->setEnabled(true);
  syncBtn->setText("Sync with Gmail");
  QMessageBox::critical(this, "Sync Failed", "Could not synchronize.\n" + error);
}

void MainWindow::refreshEventList() {
  eventListWidget->clear();
  for (const Event& e : m_events) {
	QString displayText = QString("%1\n%2\n\n[%3]")
.arg(e.title())
.arg(e.dateTime().toString("dd/MM/yyyy hh:mm AP"))
.arg(e.categoryString());

	QListWidgetItem *item = new QListWidgetItem(displayText);
	eventListWidget->addItem(item);
  }
}
