#include "dialogpinconfig.h"
#include "ui_dialogpinconfig.h"

#include "diagramelements.h"

#include <QStringListModel>
#include <QSortFilterProxyModel>

DialogPinConfig::DialogPinConfig(int n, const PinInfo &info, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPinConfig)
{
    ui->setupUi(this);
    QSortFilterProxyModel *p = new QSortFilterProxyModel(this);
    QStringListModel *m = new QStringListModel(info.functions);
    p->setSourceModel(m);
    p->setFilterRegExp(QRegExp("^((?!None).)*$"));
    ui->functionListView->setModel(p);
    ui->functionListView->setCurrentIndex(p->mapFromSource(m->index(n, 0)));
}

DialogPinConfig::~DialogPinConfig()
{
    delete ui;
}

int DialogPinConfig::selectedElement() const
{
    QSortFilterProxyModel* p = qobject_cast<QSortFilterProxyModel*>(ui->functionListView->model());
    return p->mapToSource(ui->functionListView->currentIndex()).row();
}
