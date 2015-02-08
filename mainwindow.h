#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QGraphicsView;

class SceneManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();

    void on_actionNew_triggered();

private:
    Ui::MainWindow *ui;
    QGraphicsView *view;
    SceneManager *scene;
};

#endif // MAINWINDOW_H
