#ifndef MONITOR_H
#define MONITOR_H

#include <QObject>
#include <QVariant>

class Monitor : public QObject {
	Q_OBJECT
public:
    Q_INVOKABLE QVariantMap init(); //initializes the APIs and SQL connections, returns various data about the computer's devices
	Q_INVOKABLE QVariantMap getData(); //gets the hardware data (usage, temps), formats it so it's recognizable by the QML front-end	
};
#endif //MONITOR_H
