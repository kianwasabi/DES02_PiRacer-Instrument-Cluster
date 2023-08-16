#include "dbusclient.h"
#include <QDBusReply>
#include <QDebug>


DBusClient::DBusClient(QObject *parent)
    : QObject{parent}, _dbus(QDBusConnection::sessionBus()), _iface(Q_NULLPTR)
{
  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &DBusClient::setData);
  timer->start(100);

  this->_iface = new QDBusInterface("com.test.d-bus", "/com/test/d-bus", "com.test.d-bus");
  if (!m_interface->isValid()) {
      qDebug() << "Interface not valid: " << qPrintable(m_interface->lastError().message());
      exit(1);
    }
}

DBusClient::~DBusClient() {
  if (_iface)
    delete _iface;
}


size_t DBusClient::speed() const {
  QDBusReply<size_t> reply = _iface->call("getSpeed");
  int value = reply.value();

  return value;
}

size_t DBusClient::rpm() const {
  QDBusReply<size_t> reply = _iface->call("getRpm");
  int value = reply.value();

  return value;
}

void setData() {
  this->_speed = speed();
  this->_rpm = rpm();

  emit speedChanged(_speed);
  emit rpmChanged(_rpm);
}
