# Настройка VM и Qt для работы с БД
Для того чтобы обеспечить работу сервера PostgreSQL на виртуальной машине и подключиться к нему через Qt приложение, запущенной в родительской системе, необходимо:

1. Настроить службу postgreSQL на виртуальной машине;
2. Запустить базу данных на настренном сервере;
3. Проверить доступность IP адреса сервера; PostgreSQL из ОС Windows;
4. Настроить параметры подключения к БД в QtCreator, создать базовый интерфейс подключения;
5. Провести подключение.

## Настройка сетевого моста
Первым делом, установите в настройках виртуальной машине тип сетевого адаптера на `сетевой мост`. Это необходимо для того чтобы обеспечить двухстороннюю коммуникацию сервера PostgreSQL на ВМ и приложения Qt на Windows. Имя предложенного сетевого адаптера оставьте без изменений.

## Настройка PostgreSQL на виртуальной машине
Запустите PostgreSQL:
```cmd
sudo service postgresql start
```
Перейдите в режим интерактивной работы:
```
sudo -u postgres psql
```
Здесь может возникнуть ошибка:
```
psql: error: could not connect to server: No such file or directory
Is the server running locally and accepting
connections on Unix domain socket "/var/run/postgresql/.s.PGSQL.5432"?
```
Вероятно, эта проблема связана с неправильной настройкой. Для простоты решения вопроса предлагаю выполнить следующие команды, полностью переустановив службу в нужной нам конфигурации. Если ошибки не возникло, пропустите этот шаг.

```
sudo apt-get remove postgresql
sudo apt-get purge postgresql
apt-get --purge remove postgresql\*
sudo apt-get --purge remove postgresql\*
sudo rm -r /etc/postgresql/
sudo rm -r /etc/postgresql-common/
sudo rm -r /var/lib/postgresql/
sudo rm -r /var/run/postgres
userdel -r postgres
sudo userdel -r postgres
sudo apt-get install postgresql
```

После переустановки сервис должен работать корректно.

Инициализируйте кластер базы данных:
```
sudo pg_createcluster --start 12 main
```
Запустите интерактивную оболочку управления:
```
sudo -u postgres psql
```
Cоздадим пользователя для управления БД и базу данных. Выполните следующие команды:

```
CREATE DATABASE имя_базы_данных;
CREATE USER имя_пользователя WITH PASSWORD 'пароль';
GRANT ALL PRIVILEGES ON DATABASE имя_базы_данных TO имя_пользователя;
```
Мы запустили сервис PostgreSQL и подняли БД на нем.

## Настройка подключения

Нужно убедиться, что IP-адрес сервера, на котором висит база, доступен из основной ОС.

Первым делом, посмотрим, на каком IP адресе запущена виртуальная машина.

```
mpsu@mpsu-vm:~$ ifconfig
enp0s3: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.43.93  netmask 255.255.255.0  broadcast 192.168.43.255
        inet6 fe80::b739:c3a6:1d29:a490  prefixlen 64  scopeid 0x20<link>
        ether 08:00:27:78:bb:9f  txqueuelen 1000  (Ethernet)
        RX packets 1633  bytes 198164 (198.1 KB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 182  bytes 25029 (25.0 KB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

lo: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
        inet 127.0.0.1  netmask 255.0.0.0
        inet6 ::1  prefixlen 128  scopeid 0x10<host>
        loop  txqueuelen 1000  (Local Loopback)
        RX packets 381  bytes 74497 (74.4 KB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 381  bytes 74497 (74.4 KB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
```

Нас интересует `enp0s3`. Искомый IP - `192.168.43.93`.

Узнаем, слушает ли сервер PostgreSQL нужный порт и IP:

```
sudo ss -tlnp | grep 5432

mpsu@mpsu-vm:~$ sudo ss -tlnp | grep 5432
LISTEN    0         244          192.168.43.93:5432             0.0.0.0:*        users:(("postgres",pid=2233,fd=3))     

```
В моем случае, все настроено корректно, но изначально будет что - то типо такого:

```
sudo ss -tlnp | grep 5432
LISTEN    0         244              127.0.0.1:5432             0.0.0.0:*        users:(("postgres",pid=7801,fd=3))   
```

Необходимый адрес на данный момент отсутствует в списке. Для этого необходимо провести конфигурацию. Найдите файл postgresql.conf:
```
locate postgresql.conf
```
И откройте его в редакторе:
```
sudo nano /etc/postgresql/12/main/postgresql.conf
```
Найдите раздел `CONNECTION AND AUTHERICATION` и установите `listen_addresses = '192.168.43.93'`

Сохраните файл и перезапустите сервис:
```
sudo service postgresql restart
```

Далее, нужно добавить связку пользователь-БД в файл конфигурации, чтобы можно было получить к ней доступ извне. Выполните поиск pg_hba.conf и откройте его:

```
sudo nano /etc/postgresql/12/main/pg_hba.conf
```

Добавьте в файл связку:
```
host    students    admin    192.168.43.193/32    md5
```
Здесь students - название БД, которую я создал раннее, admin - имя учетки.

Перезапустите сервис:
```
sudo service postgresql restart
```

Откройте оболочку снова и проверьте, что БД есть в списке:

```
sudo -u postgres psql
\l

Name    |  Owner   | Encoding |   Collate   |    Ctype    |   Access privileges   
-----------+----------+----------+-------------+-------------+-----------------------
 postgres  | postgres | UTF8     | en_US.UTF-8 | en_US.UTF-8 | 
 students  | postgres | UTF8     | en_US.UTF-8 | en_US.UTF-8 | =Tc/postgres         +
           |          |          |             |             | postgres=CTc/postgres+
           |          |          |             |             | admin=CTc/postgres
 template0 | postgres | UTF8     | en_US.UTF-8 | en_US.UTF-8 | =c/postgres          +
           |          |          |             |             | postgres=CTc/postgres
 template1 | postgres | UTF8     | en_US.UTF-8 | en_US.UTF-8 | =c/postgres          +
           |          |          |             |             | postgres=CTc/postgres
(4 rows)

```


Перейдите в ОС Windows и выполните проверку доступности IP-адреса (поставьте IP своей машины):

```
telnet 192.168.43.93 5432
```

> Примечание: если telnet не установлен, откройте cmd от имени админа и выполните `DISM /Online /Enable-Feature /FeatureName:TelnetClient` .

Если все сделно верно, вас перекинет на вкладку с искомым IP адресом (будет черный экран).

## Настройка подключения к Qt
Для работы с БД в Qt Вам понадобится драйвер PSQL. Вы можете проверить его наличие в каталоге установки, например, E:/Qt/6.4.3/mingw_64/plugins/sqldrivers.

> Важно! Так же необходимо установить postgreSQL на родную ОС, так как нам понадобятся библиотеки оттуда. Одного драйвера будет недостаточно. Загрузить можно на официальном сайте:
https://www.postgresql.org/download/. 

В моем случае, PostgreSQL установлен в папке C:\Program Files\PostgreSQL.
Добавьте в PATH (системную переменную) пути к библиотекам и исходникам:
* C:\Program Files\PostgreSQL\15\lib
* C:\Program Files\PostgreSQL\15\bin

## Работа с Qt Creator

Создайте проект Qt на системе сборки qmake (можно и CMake, на Ваше усмотрение, но настройка будет описываться под qmake) и сконфигурируйте .pro файл:
```pro
QT += sql
QT += core
LIBS += -LE:/Qt/6.4.3/mingw_64/plugins/sqldrivers -lqsqlpsql
CONFIG += c++20 # По дефолту 17, лучше поставить поновее
```
Подключите заголовочные файлы:
```cpp
#include <QSqlDatabase> // Для создания объекта БД
#include <QSqlError> // Для обработки ошибок
#include <QSqlQuery> // Для создания запросов

void MainWindow::databaseCreate()
{
    // Создание объекта для работы С БД:
    db = QSqlDatabase::addDatabase("QPSQL");
    //ifconfig -> ip
    // sudo -u postgres psql
    db.setHostName("192.168.43.93"); // IP-адрес вашей виртуальной машины Ubuntu (см. на виртуалке)
    db.setPort(5432); // порт по умолчанию для PostgreSQL
    db.setDatabaseName("students"); // имя вашей базы данных (см. на виртуалке)
    db.setUserName("admin"); // ваше имя пользователя для базы данных (см. на виртуалке)
    db.setPassword("12345"); // ваш пароль для базы данных (см. на виртуалке)

    if (!db.open()) {
        qDebug() << "Failed to connect to database";
        qDebug() << "Error: ";
        qDebug() << db.lastError().text();
        ui->connection_status->setText("Database in not connected.");
        ui->connection_status->setStyleSheet("color: red");
    } else {
       qDebug() << "Connected to database succesfully!";
       ui->connection_status->setText(QString("Connected successfully to %1 database").arg(db.databaseName()));
       ui->connection_status->setStyleSheet("color: green");
    }

    db.close();
}

```
В случае успеха, вы увидите в консоли сообщение:
```
Connected to database succesfully!
```

На этом базовую настройку можно считать оконченной. Все дальнейшие действия ограничиваются лишь Вашей фантазией.