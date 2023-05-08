#!/usr/bin/python3

import os
import sys

postgres_directory = '/etc/postgresql/12/main/pg_hba.conf'

# Получаем айпишник шлюза по умолчанию для интерфейса enp0s8
gateway = os.popen("ip route | grep '^default via' | grep enp0s8 | awk '{print $3}'").read().strip()

# Открываем файл pg_hba.conf для записи
with open(postgres_directory, 'a') as f:
    # Добавляем строку разрешения для указанного пользователя и базы данных
    f.write(f'host {sys.argv[1]} {sys.argv[2]} {gateway}/24 md5\n')
    
# Печатаем сообщение об успешной записи
print(f'Successfully added "{sys.argv[2]}" user to "{sys.argv[1]}" database in pg_hba.conf')

os.system("sudo service postgresql restart")