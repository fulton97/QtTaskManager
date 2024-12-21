#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtTaskManager.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QTextEdit>

class QtTaskManager : public QMainWindow
{
    Q_OBJECT

public:
    QtTaskManager(QWidget *parent = nullptr);
    ~QtTaskManager();

private:
    Ui::QtTaskManagerClass ui;
    void addTask(QString taskText);

 private slots:
    void handleNewTask();  
    void deleteTask();
    void taskChanged(QListWidgetItem* item);
};
