#ifndef LEADSHINEDEBUGPORT_H
#define LEADSHINEDEBUGPORT_H

#include <QObject>

class leadshineDebugPort : public QObject
{
    Q_OBJECT
public:
    explicit leadshineDebugPort(QObject *parent = nullptr);

signals:

public slots:
};

#endif // LEADSHINEDEBUGPORT_H