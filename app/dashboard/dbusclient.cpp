#include "dbusclient.h"
#include <QDBusReply>
#include <QDebug>

DBusClient::DBusClient(QObject *parent)
    : QObject{parent}, _dbus(QDBusConnection::sessionBus()), _iface(Q_NULLPTR)
{
  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &DBusClient::setData);
  timer->start(10);

  this->_iface = new QDBusInterface("com.test.dbusService", "/com/test/dbusService", "com.test.dbusService");
  if (!_iface->isValid()) {
      qDebug() << "Interface not valid: " << qPrintable(_iface->lastError().message());
      exit(1);
    }
}

DBusClient::~DBusClient() {
  if (_iface)
    delete _iface;
}


int DBusClient::speed() const {
  QDBusReply<int> reply = _iface->call("getSpeed");
  int value = reply.value();

  qDebug() << value;

  return value;
}

int DBusClient::rpm() const {
  QDBusReply<int> reply = _iface->call("getRpm");
  int value = reply.value();

  return value;
}

void DBusClient::setData() {
  this->_speed = speed();
  this->_rpm = rpm();

  emit speedChanged(_speed);
  emit rpmChanged(_rpm);
}
