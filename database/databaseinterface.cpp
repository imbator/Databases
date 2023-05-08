#include "databaseinterface.h"
#include "ui_databaseinterface.h"

DatabaseInterface::DatabaseInterface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DatabaseInterface)
{
    ui->setupUi(this);

    database_unit = std::make_unique<DatabaseService>();
    database_unit->databaseConnect();

    if (database_unit->getConnectionStatus()) {
        ui->logBrowser->append("Connected to database sucessfully!");
    } else {
        ui->logBrowser->append("Failed connection to database! Please, check logs in console for more info.");
    }

    connect(database_unit.get(), &DatabaseService::log, ui->logBrowser, &QTextBrowser::append);
    connect(ui->clear, &QPushButton::clicked, ui->logBrowser, &QTextBrowser::clear);
    connect(ui->configuration, &QPushButton::clicked, this, &DatabaseInterface::loadQuery);
    connect(ui->recreate, &QPushButton::clicked, this, &DatabaseInterface::recreateDatabase);
    connect(database_unit.get(), &DatabaseService::update, this, &DatabaseInterface::updateDatabaseInterface);

}

DatabaseInterface::~DatabaseInterface()
{
    delete ui;
}


void DatabaseInterface::loadQuery()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                            tr("Open SQL File"), "",
                            tr("SQL Files (*.sql);;All Files (*)"));
    database_unit->makeQuery(QFile(fileName));
    // Загрузка sql запроса из файла
}

void DatabaseInterface::recreateDatabase()
{
    database_unit->databaseCreate();
}

void DatabaseInterface::updateDatabaseInterface()
{
    QSqlTableModel *model = new QSqlTableModel(this, database_unit->getDatabase());
    model->setTable("Студент");

    // Установка модели для отображения в таблице
    ui->students_table->setModel(model);

    // Получение данных из базы данных
    model->select();
}

