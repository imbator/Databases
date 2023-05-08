#include "databaseservice.h"

DatabaseService::DatabaseService()
{
    table_name = "students";
    user_name = "admin";
    password = "12345";
    host_name = "192.168.56.101";
}

void DatabaseService::databaseConnect()
{
    // TODO: перенести инициализацию всех параметров базы данных в скрипт

    // Создание объекта для работы С БД:
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(host_name); // IP-адрес вашей виртуальной машины Ubuntu (см. на виртуалке)
    db.setPort(5432); // порт по умолчанию для PostgreSQL
    db.setDatabaseName(table_name); // имя вашей базы данных (см. на виртуалке)
    db.setUserName(user_name); // ваше имя пользователя для базы данных (см. на виртуалке)
    db.setPassword(password); // ваш пароль для базы данных (см. на виртуалке)
    db.setConnectOptions("client_encoding=UTF8");

    if (!db.open()) {
        qDebug() << "Failed to connect to database.";
        qDebug() << "Error: ";
        qDebug() << db.lastError().text();
        connection_status = false;
    } else {
       qDebug() << "Connected to database succesfully!";
       connection_status = true;
    }
}

void DatabaseService::databaseCreate()
{
    // Закрыть все соединения с базой данных
    db.close();

    QSqlDatabase::removeDatabase("QPSQL");

    // Подключиться к базе данных postgres
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(host_name);
    db.setPort(5432);
    db.setUserName(user_name);
    db.setPassword(password);
    db.setDatabaseName("postgres");
    if (!db.open()) {
        qDebug() << "Unable to connect to postgres database.";
        qDebug() << "Error: ";
        qDebug() << db.lastError().text();
        return;
    }

    // Выполнить оператор DROP DATABASE
    QSqlQuery query;
    query.exec("DROP DATABASE students");

    // Закрыть соединение с базой данных postgres
    db.close();

    // Подключиться к целевой базе данных
    db.setDatabaseName("students");
    if (!db.open()) {
        qDebug() << "Unable to connect to Students database";
        return;
    }

}

void DatabaseService::makeQuery(QFile file)
{
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file.";
        return;
    }
    QString query = QString::fromUtf8(file.readAll());

    // Создаем объект очереди для реализации запроса
    QSqlQuery sql_query(query);
    QString message;

    if (sql_query.lastError().isValid())
        message = "Error executing query:" + sql_query.lastError().text();
    else
        message = "Query in file " + file.fileName() + " executed sucesfully!";

    emit log(message);
    emit update();
}

bool DatabaseService::getConnectionStatus()
{
    return this->connection_status;
}

QSqlDatabase& DatabaseService::getDatabase()
{
    return this->db;
}
