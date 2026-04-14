#include "NetworkManager.hpp"
#include <iostream>

using namespace std;

NetworkManager::NetworkManager(QObject *parent) : QObject(parent) {
  m_isRunning = false;
}

NetworkManager::~NetworkManager() {}

void NetworkManager::syncWithGmail(const QString& email, const QString& appPassword) {
  if (m_isRunning) {
	emit syncError("A sync operation is already in progress.");
	return;
  }
  m_isRunning = true;

  try {
	boost::asio::io_context io_context;
	boost::asio::ssl::context ssl_context(boost::asio::ssl::context::tlsv12_client);
	ssl_context.set_default_verify_paths();

	boost::asio::ip::tcp::resolver resolver(io_context);
	boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve("imap.gmail.com", "993");

	boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket(io_context, ssl_context);

	boost::asio::connect(socket.lowest_layer(), endpoints);
	socket.handshake(boost::asio::ssl::stream_base::client);

	string responseString;

	boost::asio::read_until(socket, boost::asio::dynamic_buffer(responseString), "\r\n");
	responseString.clear();

	string loginCmd = "A01 LOGIN " + email.toStdString() + " " + appPassword.toStdString() + "\r\n";
	boost::asio::write(socket, boost::asio::buffer(loginCmd));

	boost::asio::read_until(socket, boost::asio::dynamic_buffer(responseString), "\r\n");

	if (responseString.find("OK") == string::npos && responseString.find("Success") == string::npos) {
	emit syncError("Login Failed. Check App Password.");
	m_isRunning = false;
	return;
	}
	responseString.clear();

	boost::asio::write(socket, boost::asio::buffer("A02 SELECT INBOX\r\n"));
	int totalMessages = 0;

	do {
	responseString.clear();
	boost::asio::read_until(socket, boost::asio::dynamic_buffer(responseString), "\r\n");

	if (responseString.find("EXISTS") != string::npos) {
size_t firstSpace = responseString.find(" ");
size_t secondSpace = responseString.find(" ", firstSpace + 1);
if (firstSpace != string::npos && secondSpace != string::npos) {
string numStr = responseString.substr(firstSpace + 1, secondSpace - firstSpace - 1);
totalMessages = stoi(numStr);
}
	}
	} while (responseString.find("A02 OK") == string::npos && responseString.find("A02 BAD") == string::npos);

	if (totalMessages == 0) {
	boost::asio::write(socket, boost::asio::buffer("A04 LOGOUT\r\n"));
	responseString.clear();
	boost::asio::read_until(socket, boost::asio::dynamic_buffer(responseString), "\r\n");
	m_isRunning = false;
	return;
	}

	int startMsg = totalMessages - 2;
	if (startMsg < 1) {
	startMsg = 1;
	}
	string fetchCmd = "A03 FETCH " + to_string(startMsg) + ":" + to_string(totalMessages) + " BODY[TEXT]\r\n";
	boost::asio::write(socket, boost::asio::buffer(fetchCmd));

	string fullEmailBody = "";

	do {
	responseString.clear();
	boost::asio::read_until(socket, boost::asio::dynamic_buffer(responseString), "\r\n");
	fullEmailBody += responseString;

	} while (responseString.find("A03 OK") == string::npos && responseString.find("A03 BAD") == string::npos);

	boost::asio::write(socket, boost::asio::buffer("A04 LOGOUT\r\n"));
	responseString.clear();
	boost::asio::read_until(socket, boost::asio::dynamic_buffer(responseString), "\r\n");

	emit syncSuccess(QString::fromStdString(fullEmailBody));

  } catch (const exception& e) {
	emit syncError(QString("IMAP Error: %1").arg(e.what()));
  }

  m_isRunning = false;
}
