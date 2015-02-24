#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "scenemanager.h"
#include "sceneview.h"
#include "diagramelements.h"
#include "aboutdialog.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QFileDialog>
#include <QMessageBox>
#include <QGradient>
#include <QTreeView>

#include <cmath>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QApplication::applicationName());
    setCentralWidget(view = new SceneView(this));
    view->setScene(scene = new SceneManager(this));
//    QLinearGradient gradient;
//    gradient.setCoordinateMode(QGradient::StretchToDeviceMode);
//    gradient.setColorAt(0, QColor(Qt::green).lighter(150));
//    gradient.setColorAt(1, QColor(Qt::green).lighter(200));
//    QBrush bg(QColor(Qt::yellow).lighter(170)); //gradient);
//    scene->setBackgroundBrush(bg);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::newFrom(const QString &file)
{
    if (file.isEmpty())
        return false;
    SoCInfo *info = new SoCInfo();
    if (!info->load(file)) {
        QMessageBox::critical(this, tr("Open error"), info->errorMessage());
        delete info;
        return false;
    } else {
        scene->clear();
        SocBodyItem *body = new SocBodyItem(info, QSize(400,400));
        scene->addItem(body);
        QRectF b = body->boundingRegion(body->transform()).boundingRect();
        float g = std::max(b.width(), b.height());
        scene->setSceneRect(b.adjusted(-g, -g, g, g));
        view->centerOn(body);
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

void MainWindow::on_actionZoom_In_triggered()
{
    qreal factor = std::pow(1.001, 18);
    view->scale(factor, factor);
}

void MainWindow::on_actionZoom_Out_triggered()
{
    qreal factor = std::pow(1.001, -18);
    view->scale(factor, factor);
}

void MainWindow::on_actionZoom_1_1_triggered()
{
    view->resetTransform();
    view->centerOn(scene->sceneRect().center());
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog(this).exec();
}
