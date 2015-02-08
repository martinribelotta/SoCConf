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

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionNew_triggered()
{
    QString path = QFileDialog::
            getOpenFileName(this,
                            tr("Open Chip"),
                            QDir::currentPath(),
                            tr("Chip files (*.chip);;All files (*)"));
    SoCInfo info = SoCInfo::fromFile(path); //"/home/martin/Proyectos/git/martinribelotta/SoCConf/test.chip"));
    if (info.pins.isEmpty())
        QMessageBox::critical(this, tr("Open error"), tr("Error reading %1").arg(path));
    else {
        SocBodyItem *body = new SocBodyItem(info, QSize(400,400));
        scene->addItem(body);
    }
}
