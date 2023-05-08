--DROP DATABASE Students;
--CREATE DATABASE Students;
DROP TABLE IF EXISTS "Студент" CASCADE;
DROP TABLE IF EXISTS "Структурное подразделение" CASCADE;
DROP TABLE IF EXISTS "Группа";
DROP TABLE IF EXISTS "Студенческий билет";
DROP TABLE IF EXISTS "Преподаватель" CASCADE;
DROP TABLE IF EXISTS "Дисциплина" CASCADE;
DROP TABLE IF EXISTS "Трудоустройство";
DROP TABLE IF EXISTS "Результат освоения дисциплины";

CREATE TABLE "Структурное подразделение"(
    "Номер подразделения" BIGINT PRIMARY KEY CHECK("Номер подразделения" > 0),
    "Тип подразделения" VARCHAR(64) NOT NULL CHECK ("Тип подразделения" = ANY ('{Кафедра, Институт}'::VARCHAR[])),
    "Полное название" TEXT NOT NULL,
    "Сокращенное название" VARCHAR(20) NULL,
    "Руководитель" VARCHAR(40) NOT NULL
);