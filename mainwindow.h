#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "recipedb.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    std::unordered_map<QString, Recipe> recipeDB;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


};

#endif // MAINWINDOW_H
