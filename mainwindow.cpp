#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "scenemanager.h"
#include "sceneview.h"
#include "diagramelements.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(view = new SceneView(this));
    view->setScene(scene = new SceneManager(this));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::newFrom(const QString &file)
{
    SoCInfo *info = new SoCInfo();
    if (!info->load(file)) {
        QMessageBox::critical(this, tr("Open error"), info->errorMessage());
        delete info;
        return false;
    } else {
        scene->clear();
        SocBodyItem *body = new SocBodyItem(info, QSize(400,400));
        scene->addItem(body);
        return true;
    }
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionNew_triggered()
{
    newFrom(QFileDialog::
            getOpenFileName(this,
                            tr("Open Chip"),
                            QDir::currentPath(),
                            tr("Chip files (*.chip);;All files (*)")));
}
