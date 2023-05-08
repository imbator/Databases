#ifndef DATABASEINTERFACE_H
#define DATABASEINTERFACE_H

#include <memory.h>
#include <QWidget>
#include <QSqlTableModel>
#include <QString>
#include <QFileDialog>
#include "service/databaseservice.h"

namespace Ui {
class DatabaseInterface;
}

class DatabaseInterface : public QWidget
{
    Q_OBJECT

public:
    explicit DatabaseInterface(QWidget *parent = nullptr);
    ~DatabaseInterface();

private slots:
    void loadQuery();
    void recreateDatabase();
    void updateDatabaseInterface();

private:
    Ui::DatabaseInterface *ui;
    std::unique_ptr<DatabaseService> database_unit_;
};

#endif // DATABASEINTERFACE_H
