#include "dialogpinconfig.h"
#include "diagramelements.h"

#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QFormLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QBoxLayout>
#include <QListView>
#include <QCheckBox>
#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QComboBox>
#include <QScriptValueIterator>

#include <QtDebug>

class EntryView: public QWidget {
public:
    EntryView(QWidget *parent = 0l) : QWidget(parent) { }
    virtual void setData(const QString& valName, const QScriptValue& val, const QScriptValue& idx) = 0;
    virtual QScriptValue value() const = 0;
};

class ListView: public EntryView {
private:
    QListView *view;
    QGroupBox *group;
public:
    ListView(QWidget *parent = 0l): EntryView(parent) {
        QHBoxLayout *layout = new QHBoxLayout(this);
        layout->setMargin(0);
        group = new QGroupBox(this);
        layout->addWidget(group);
        QHBoxLayout *innerLayout = new QHBoxLayout(group);
        view = new QListView(group);
        view->setEditTriggers(QListView::NoEditTriggers);
        view->setSelectionMode(QListView::SingleSelection);
        innerLayout->addWidget(view);
    }

    virtual void setData(const QString& valName, const QScriptValue& val, const QScriptValue& idx) {
        int selectedItem = idx.property(valName).toInteger();
        QSortFilterProxyModel *p = new QSortFilterProxyModel(this);
        QStringListModel *m = new QStringListModel(val.toVariant().toStringList());
        group->setTitle(valName);
        p->setSourceModel(m);
        p->setFilterRegExp(QRegExp("^((?!None).)*$"));
        view->setModel(p);
        view->setCurrentIndex(p->mapFromSource(m->index(selectedItem, 0)));
    }

    virtual QScriptValue value() const {
      QSortFilterProxyModel* p = qobject_cast<QSortFilterProxyModel*>(view->model());
      QModelIndex viewIndex = view->currentIndex();
      return QScriptValue(p->mapToSource(viewIndex).row());
    }
};

class ComboView: public EntryView {
private:
    QComboBox *view;
    QLabel *label;
public:
    ComboView(QWidget *parent = 0l): EntryView(parent) {
        QFormLayout *layout = new QFormLayout(this);
        label = new QLabel(this);
        view = new QComboBox(this);
        view->setEditable(false);
        view->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
        layout->addRow(label, view);
        layout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    }

    virtual void setData(const QString& valName, const QScriptValue& val, const QScriptValue& idx) {
        int selectedItem = idx.property(valName).toInteger();
        QSortFilterProxyModel *p = new QSortFilterProxyModel(this);
        QStringListModel *m = new QStringListModel(val.toVariant().toStringList());
        label->setText(valName);
        p->setSourceModel(m);
        p->setFilterRegExp(QRegExp("^((?!None).)*$"));
        view->setModel(p);
        view->setCurrentIndex(p->mapFromSource(m->index(selectedItem, 0)).row());
    }

    virtual QScriptValue value() const {
      QSortFilterProxyModel* p = qobject_cast<QSortFilterProxyModel*>(view->model());
      int viewIndex = view->currentIndex();
      return QScriptValue(p->mapToSource(p->index(viewIndex, 0)).row());
    }
};

class BoxCheckBox: public QCheckBox {
public:
    BoxCheckBox(QWidget *parent = 0l): QCheckBox(parent) {}

protected:
    virtual void paintEvent(QPaintEvent *e) {
        QCheckBox::paintEvent(e);
        QPainter p(this);
        p.setPen(Qt::red);
        p.drawRect(QRect(QPoint(), size()));
    }
};

class CheckBox: public EntryView {
    QPushButton *check;
public:
    CheckBox(QWidget *parent = 0l): EntryView(parent) {
        QVBoxLayout *l = new QVBoxLayout(this);
        check = new QPushButton(this);
        check->setCheckable(true);
        l->addWidget(check);
        setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
        check->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
    }

    virtual void setData(const QString& valName, const QScriptValue& val, const QScriptValue& idx) {
        check->setText(val.toString());
        check->setChecked(idx.property(valName).toBool());
    }

    virtual QScriptValue value() const {
        return QScriptValue(check->isChecked());
    }
};

static EntryView *createWidget(const QString& name, QWidget *parent) {
    if (name == "listview")
        return new ListView(parent);
    if (name == "checkbox")
        return new CheckBox(parent);
    if (name == "combo")
        return new ComboView(parent);
    return 0l;
}

static QScriptValue findIdx(QScriptValue v) {
    QScriptValueIterator it(v);
    while(it.hasNext()) {
        it.next();
        if (it.name() == "idx")
            return it.value();
    }
    return v.engine()->nullValue();
}

DialogPinConfig::DialogPinConfig(QScriptValue pinInfo, const SoCInfo &socInfo, QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    pIdx = findIdx(pinInfo);
    QScriptValue pinProperties = socInfo.properties();
    QScriptValueIterator it(pinInfo);
    while(it.hasNext()) {
        it.next();
        QString name = it.name();
        QScriptValue val = it.value();
        QScriptValue infoOf = pinProperties.property(name);
        if (infoOf.isValid()) {
            QString type = infoOf.property("type").toString();
            EntryView *e = createWidget(type, this);
            if (e) {
                e->setData(name, val, pIdx);
                e->setProperty("configName", name);
                e->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
                layout->addWidget(e);
                views.append(e);
            }
        }
    }

    setProperty("configurable", layout->count());

    QDialogButtonBox *buttons = new QDialogButtonBox(this);
    buttons->addButton(tr("Accept"), QDialogButtonBox::AcceptRole);
    buttons->addButton(QDialogButtonBox::Cancel);
    layout->addWidget(buttons);

    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
}

DialogPinConfig::~DialogPinConfig()
{
}

QScriptValue DialogPinConfig::idx() const
{
    QScriptValue idx = pIdx.engine()->newObject();
    foreach(EntryView *e, views) {
        QString name = e->property("configName").toString();
        QScriptValue val = e->value();
        idx.setProperty(name, val);
    }
    return idx;
}
