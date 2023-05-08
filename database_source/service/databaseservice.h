#ifndef DATABASESERVICE_H
#define DATABASESERVICE_H

#include <memory.h>
#include <QObject>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include "config/configuratedatabase.h"


class DatabaseService : public QObject
{
    Q_OBJECT
    // Database backend
public:
    DatabaseService() {
        this->database_name = DATABASE_NAME;
        this->user_name = USER_NAME;
        this->password = PASSWORD;
        this->host_name = HOST_NAME;
    }
    void databaseConnect();
    void databaseCreate();
    void makeQuery(QFile file);


    bool getConnectionStatus();
    QSqlDatabase& getDatabase();

signals:
    void log(QString&);
    void update();

private:
    QSqlDatabase db;

    QString database_name;
    QString user_name;
    QString password;
    QString host_name;

    bool connection_status;
};

#endif // DATABASESERVICE_H
