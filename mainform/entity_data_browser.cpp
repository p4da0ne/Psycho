#include "entity_data_browser.h"

#include <algorithm>
#include <functional>
#include <utility>

#include <QAbstractItemView>
#include <QCheckBox>
#include <QComboBox>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QMap>
#include <QMessageBox>
#include <QPushButton>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QSqlField>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QTabWidget>
#include <QTableView>
#include <QVBoxLayout>

namespace {

void configureTableView(QTableView *view)
{
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setSelectionMode(QAbstractItemView::ExtendedSelection);
    view->setEditTriggers(QAbstractItemView::DoubleClicked
                          | QAbstractItemView::EditKeyPressed
                          | QAbstractItemView::AnyKeyPressed
                          | QAbstractItemView::SelectedClicked);
    view->setSortingEnabled(true);
    view->horizontalHeader()->setStretchLastSection(true);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

QWidget *buildCrudButtons(const QString &entityCaption,
                          std::function<void()> onAdd,
                          std::function<void()> onDelete,
                          std::function<void()> onSave,
                          std::function<void()> onRevert)
{
    QWidget *container = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);

    QPushButton *addBtn = new QPushButton(QString::fromUtf8("Добавить %1").arg(entityCaption));
    QPushButton *delBtn = new QPushButton(QString::fromUtf8("Удалить выбранные"));
    QPushButton *saveBtn = new QPushButton(QString::fromUtf8("Сохранить изменения"));
    QPushButton *revertBtn = new QPushButton(QString::fromUtf8("Отменить несохраненное"));

    QObject::connect(addBtn, &QPushButton::clicked, std::move(onAdd));
    QObject::connect(delBtn, &QPushButton::clicked, std::move(onDelete));
    QObject::connect(saveBtn, &QPushButton::clicked, std::move(onSave));
    QObject::connect(revertBtn, &QPushButton::clicked, std::move(onRevert));

    layout->addWidget(addBtn);
    layout->addWidget(delBtn);
    layout->addWidget(saveBtn);
    layout->addWidget(revertBtn);
    layout->addStretch(1);

    return container;
}

} // namespace

EntityDataBrowser::EntityDataBrowser(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(8, 8, 8, 8);

    QLabel *title = new QLabel(QString::fromUtf8("Поиск, фильтрация и редактирование данных БД"), this);
    QFont f = title->font();
    f.setBold(true);
    title->setFont(f);
    rootLayout->addWidget(title);

    m_tabs = new QTabWidget(this);
    rootLayout->addWidget(m_tabs);

    setupObjectsTab();
    setupPeopleTab();
    setupSpecialConditionsTab();
    setupEventsTab();
}

void EntityDataBrowser::setupObjectsTab()
{
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);

    m_objectTypeCombo = new QComboBox(tab);
    m_objectTypeCombo->addItem(QString::fromUtf8("Формирования"), QStringLiteral("ls"));
    m_objectTypeCombo->addItem(QString::fromUtf8("Регионы"), QStringLiteral("region"));
    m_objectTypeCombo->addItem(QString::fromUtf8("Организации"), QStringLiteral("groups"));
    m_objectTypeCombo->addItem(QString::fromUtf8("СМИ"), QStringLiteral("smi"));
    m_objectTypeCombo->addItem(QString::fromUtf8("Средства"), QStringLiteral("mpo_pso"));

    m_objectNameFilter = new QLineEdit(tab);
    m_objectNameFilter->setPlaceholderText(QString::fromUtf8("Название (часть)") );
    m_objectCharFilter = new QLineEdit(tab);
    m_objectCharFilter->setPlaceholderText(QString::fromUtf8("Характеристики/описание (часть)") );

    QPushButton *applyBtn = new QPushButton(QString::fromUtf8("Применить фильтр"), tab);
    QPushButton *resetBtn = new QPushButton(QString::fromUtf8("Сбросить"), tab);

    QHBoxLayout *filters = new QHBoxLayout;
    filters->addWidget(new QLabel(QString::fromUtf8("Тип:"), tab));
    filters->addWidget(m_objectTypeCombo, 1);
    filters->addWidget(new QLabel(QString::fromUtf8("Название:"), tab));
    filters->addWidget(m_objectNameFilter, 2);
    filters->addWidget(new QLabel(QString::fromUtf8("Характеристики:"), tab));
    filters->addWidget(m_objectCharFilter, 2);
    filters->addWidget(applyBtn);
    filters->addWidget(resetBtn);

    layout->addLayout(filters);

    m_objectView = new QTableView(tab);
    configureTableView(m_objectView);
    layout->addWidget(m_objectView, 1);

    m_objectModel = new QSqlTableModel(this);
    m_objectModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    layout->addWidget(buildCrudButtons(
        QString::fromUtf8("объект"),
        [this]() { addRow(m_objectModel, m_objectView); },
        [this]() { removeSelectedRows(m_objectModel, m_objectView, QString::fromUtf8("объекты")); },
        [this]() { saveModel(m_objectModel, QString::fromUtf8("объекты")); },
        [this]() { revertModel(m_objectModel); }
    ));

    connect(m_objectTypeCombo, &QComboBox::currentIndexChanged, this, [this](int) {
        const QString tableName = m_objectTypeCombo->currentData().toString();
        setupModel(m_objectModel, tableName);
        m_objectView->setModel(m_objectModel);
        applyObjectsFilter();
    });
    connect(applyBtn, &QPushButton::clicked, this, [this]() { applyObjectsFilter(); });
    connect(resetBtn, &QPushButton::clicked, this, [this]() {
        m_objectNameFilter->clear();
        m_objectCharFilter->clear();
        applyObjectsFilter();
    });

    setupModel(m_objectModel, m_objectTypeCombo->currentData().toString());
    m_objectView->setModel(m_objectModel);

    m_tabs->addTab(tab, QString::fromUtf8("Объекты"));
}

void EntityDataBrowser::setupPeopleTab()
{
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);

    m_peopleNameFilter = new QLineEdit(tab);
    m_peopleNameFilter->setPlaceholderText(QString::fromUtf8("ФИО / имя (часть)"));
    m_peopleCharFilter = new QLineEdit(tab);
    m_peopleCharFilter->setPlaceholderText(QString::fromUtf8("Характеристики/описание (часть)"));

    QPushButton *applyBtn = new QPushButton(QString::fromUtf8("Применить фильтр"), tab);
    QPushButton *resetBtn = new QPushButton(QString::fromUtf8("Сбросить"), tab);

    QHBoxLayout *filters = new QHBoxLayout;
    filters->addWidget(new QLabel(QString::fromUtf8("Название:"), tab));
    filters->addWidget(m_peopleNameFilter, 2);
    filters->addWidget(new QLabel(QString::fromUtf8("Характеристики:"), tab));
    filters->addWidget(m_peopleCharFilter, 2);
    filters->addWidget(applyBtn);
    filters->addWidget(resetBtn);

    layout->addLayout(filters);

    m_peopleView = new QTableView(tab);
    configureTableView(m_peopleView);
    layout->addWidget(m_peopleView, 1);

    m_peopleModel = new QSqlTableModel(this);
    m_peopleModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    setupModel(m_peopleModel, QStringLiteral("persones"));
    m_peopleView->setModel(m_peopleModel);

    layout->addWidget(buildCrudButtons(
        QString::fromUtf8("персону"),
        [this]() { addRow(m_peopleModel, m_peopleView); },
        [this]() { removeSelectedRows(m_peopleModel, m_peopleView, QString::fromUtf8("персоны")); },
        [this]() { saveModel(m_peopleModel, QString::fromUtf8("персоны")); },
        [this]() { revertModel(m_peopleModel); }
    ));

    connect(applyBtn, &QPushButton::clicked, this, [this]() { applyPeopleFilter(); });
    connect(resetBtn, &QPushButton::clicked, this, [this]() {
        m_peopleNameFilter->clear();
        m_peopleCharFilter->clear();
        applyPeopleFilter();
    });

    m_tabs->addTab(tab, QString::fromUtf8("Люди"));
}

void EntityDataBrowser::setupSpecialConditionsTab()
{
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);

    m_specialNameFilter = new QLineEdit(tab);
    m_specialNameFilter->setPlaceholderText(QString::fromUtf8("Название (часть)"));
    m_specialCharFilter = new QLineEdit(tab);
    m_specialCharFilter->setPlaceholderText(QString::fromUtf8("Семантика/характеристики (часть)"));

    QPushButton *applyBtn = new QPushButton(QString::fromUtf8("Применить фильтр"), tab);
    QPushButton *resetBtn = new QPushButton(QString::fromUtf8("Сбросить"), tab);

    QHBoxLayout *filters = new QHBoxLayout;
    filters->addWidget(new QLabel(QString::fromUtf8("Название:"), tab));
    filters->addWidget(m_specialNameFilter, 2);
    filters->addWidget(new QLabel(QString::fromUtf8("Характеристики:"), tab));
    filters->addWidget(m_specialCharFilter, 2);
    filters->addWidget(applyBtn);
    filters->addWidget(resetBtn);

    layout->addLayout(filters);

    m_specialView = new QTableView(tab);
    configureTableView(m_specialView);
    layout->addWidget(m_specialView, 1);

    m_specialModel = new QSqlTableModel(this);
    m_specialModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    setupModel(m_specialModel, QStringLiteral("special_conditions"));
    m_specialView->setModel(m_specialModel);

    layout->addWidget(buildCrudButtons(
        QString::fromUtf8("условие"),
        [this]() { addRow(m_specialModel, m_specialView); },
        [this]() { removeSelectedRows(m_specialModel, m_specialView, QString::fromUtf8("особые условия")); },
        [this]() { saveModel(m_specialModel, QString::fromUtf8("особые условия")); },
        [this]() { revertModel(m_specialModel); }
    ));

    connect(applyBtn, &QPushButton::clicked, this, [this]() { applySpecialConditionsFilter(); });
    connect(resetBtn, &QPushButton::clicked, this, [this]() {
        m_specialNameFilter->clear();
        m_specialCharFilter->clear();
        applySpecialConditionsFilter();
    });

    m_tabs->addTab(tab, QString::fromUtf8("Спецусловия"));
}

void EntityDataBrowser::setupEventsTab()
{
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);

    m_eventNameFilter = new QLineEdit(tab);
    m_eventNameFilter->setPlaceholderText(QString::fromUtf8("Название события (часть)"));
    m_eventCharFilter = new QLineEdit(tab);
    m_eventCharFilter->setPlaceholderText(QString::fromUtf8("Описание/выводы/характеристики (часть)"));

    m_eventFromCheck = new QCheckBox(QString::fromUtf8("С даты/времени"), tab);
    m_eventFromEdit = new QDateTimeEdit(QDateTime::currentDateTime(), tab);
    m_eventFromEdit->setDisplayFormat(QStringLiteral("yyyy-MM-dd HH:mm:ss"));
    m_eventFromEdit->setCalendarPopup(true);

    m_eventToCheck = new QCheckBox(QString::fromUtf8("По дату/время"), tab);
    m_eventToEdit = new QDateTimeEdit(QDateTime::currentDateTime(), tab);
    m_eventToEdit->setDisplayFormat(QStringLiteral("yyyy-MM-dd HH:mm:ss"));
    m_eventToEdit->setCalendarPopup(true);

    QPushButton *applyBtn = new QPushButton(QString::fromUtf8("Применить фильтр"), tab);
    QPushButton *resetBtn = new QPushButton(QString::fromUtf8("Сбросить"), tab);

    QHBoxLayout *filters = new QHBoxLayout;
    filters->addWidget(new QLabel(QString::fromUtf8("Название:"), tab));
    filters->addWidget(m_eventNameFilter, 2);
    filters->addWidget(new QLabel(QString::fromUtf8("Характеристики:"), tab));
    filters->addWidget(m_eventCharFilter, 2);
    filters->addWidget(m_eventFromCheck);
    filters->addWidget(m_eventFromEdit);
    filters->addWidget(m_eventToCheck);
    filters->addWidget(m_eventToEdit);
    filters->addWidget(applyBtn);
    filters->addWidget(resetBtn);

    layout->addLayout(filters);

    m_eventView = new QTableView(tab);
    configureTableView(m_eventView);
    layout->addWidget(m_eventView, 1);

    m_eventModel = new QSqlTableModel(this);
    m_eventModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    setupModel(m_eventModel, QStringLiteral("events"));
    m_eventView->setModel(m_eventModel);

    layout->addWidget(buildCrudButtons(
        QString::fromUtf8("событие"),
        [this]() { addRow(m_eventModel, m_eventView); },
        [this]() { removeSelectedRows(m_eventModel, m_eventView, QString::fromUtf8("события")); },
        [this]() { saveModel(m_eventModel, QString::fromUtf8("события")); },
        [this]() { revertModel(m_eventModel); }
    ));

    connect(applyBtn, &QPushButton::clicked, this, [this]() { applyEventsFilter(); });
    connect(resetBtn, &QPushButton::clicked, this, [this]() {
        m_eventNameFilter->clear();
        m_eventCharFilter->clear();
        m_eventFromCheck->setChecked(false);
        m_eventToCheck->setChecked(false);
        applyEventsFilter();
    });

    m_tabs->addTab(tab, QString::fromUtf8("События"));
}

void EntityDataBrowser::setupModel(QSqlTableModel *model, const QString &tableName)
{
    if (!model) {
        return;
    }

    model->revertAll();
    model->setFilter(QString());
    model->setTable(tableName);
    model->select();

    QSqlRecord rec = model->record();
    for (int i = 0; i < rec.count(); ++i) {
        const QString fieldName = rec.fieldName(i);
        model->setHeaderData(i, Qt::Horizontal, fieldName);
    }
}

void EntityDataBrowser::applyObjectsFilter()
{
    if (!m_objectModel) {
        return;
    }
    const QString filter = buildTextFilter(
        m_objectModel,
        m_objectNameFilter->text().trimmed(),
        m_objectCharFilter->text().trimmed(),
        QStringList{QStringLiteral("name_ls"), QStringLiteral("name_region"), QStringLiteral("name_groups"),
                    QStringLiteral("name_smi"), QStringLiteral("name_mpo_pso")});

    m_objectModel->setFilter(filter);
    m_objectModel->select();
}

void EntityDataBrowser::applyPeopleFilter()
{
    if (!m_peopleModel) {
        return;
    }

    const QString filter = buildTextFilter(
        m_peopleModel,
        m_peopleNameFilter->text().trimmed(),
        m_peopleCharFilter->text().trimmed(),
        QStringList{QStringLiteral("name_persones")});

    m_peopleModel->setFilter(filter);
    m_peopleModel->select();
}

void EntityDataBrowser::applySpecialConditionsFilter()
{
    if (!m_specialModel) {
        return;
    }

    const QString filter = buildTextFilter(
        m_specialModel,
        m_specialNameFilter->text().trimmed(),
        m_specialCharFilter->text().trimmed(),
        QStringList{QStringLiteral("name_special_conditions")});

    m_specialModel->setFilter(filter);
    m_specialModel->select();
}

void EntityDataBrowser::applyEventsFilter()
{
    if (!m_eventModel) {
        return;
    }

    QStringList filters;
    const QString textFilter = buildTextFilter(
        m_eventModel,
        m_eventNameFilter->text().trimmed(),
        m_eventCharFilter->text().trimmed(),
        QStringList{QStringLiteral("name_event")});

    if (!textFilter.trimmed().isEmpty()) {
        filters << QStringLiteral("(%1)").arg(textFilter);
    }

    if (m_eventFromCheck->isChecked() && hasColumn(m_eventModel, QStringLiteral("time_event_end"))) {
        const QString dt = m_eventFromEdit->dateTime().toString(QStringLiteral("yyyy-MM-dd HH:mm:ss"));
        filters << QStringLiteral("time_event_end >= '%1'").arg(esc(dt));
    }

    if (m_eventToCheck->isChecked() && hasColumn(m_eventModel, QStringLiteral("time_event_start"))) {
        const QString dt = m_eventToEdit->dateTime().toString(QStringLiteral("yyyy-MM-dd HH:mm:ss"));
        filters << QStringLiteral("time_event_start <= '%1'").arg(esc(dt));
    }

    m_eventModel->setFilter(filters.join(QStringLiteral(" AND ")));
    m_eventModel->select();
}

void EntityDataBrowser::addRow(QSqlTableModel *model, QTableView *view)
{
    if (!model || !view) {
        return;
    }

    const int row = model->rowCount();
    if (!model->insertRow(row)) {
        QMessageBox::warning(this,
                             QString::fromUtf8("Ошибка"),
                             QString::fromUtf8("Не удалось добавить новую запись."));
        return;
    }

    view->selectRow(row);
    view->scrollTo(model->index(row, 0));
}

void EntityDataBrowser::removeSelectedRows(QSqlTableModel *model, QTableView *view, const QString &entityCaption)
{
    if (!model || !view || !view->selectionModel()) {
        return;
    }

    QModelIndexList indexes = view->selectionModel()->selectedRows();
    if (indexes.isEmpty()) {
        QMessageBox::information(this,
                                 QString::fromUtf8("Удаление"),
                                 QString::fromUtf8("Выберите минимум одну строку для удаления."));
        return;
    }

    const int answer = QMessageBox::question(
        this,
        QString::fromUtf8("Подтверждение"),
        QString::fromUtf8("Удалить выбранные записи (%1)?").arg(entityCaption),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);

    if (answer != QMessageBox::Yes) {
        return;
    }

    QList<int> rows;
    rows.reserve(indexes.size());
    for (const QModelIndex &index : indexes) {
        rows.append(index.row());
    }
    std::sort(rows.begin(), rows.end(), std::greater<int>());

    for (const int row : rows) {
        model->removeRow(row);
    }
}

void EntityDataBrowser::saveModel(QSqlTableModel *model, const QString &entityCaption)
{
    if (!model) {
        return;
    }

    if (!model->submitAll()) {
        QMessageBox::warning(
            this,
            QString::fromUtf8("Ошибка сохранения"),
            QString::fromUtf8("Не удалось сохранить изменения (%1).\n%2")
                .arg(entityCaption, model->lastError().text()));
        model->database().rollback();
        model->revertAll();
        model->select();
        return;
    }

    model->select();
}

void EntityDataBrowser::revertModel(QSqlTableModel *model)
{
    if (!model) {
        return;
    }

    model->revertAll();
    model->select();
}

QString EntityDataBrowser::buildTextFilter(QSqlTableModel *model,
                                           const QString &nameValue,
                                           const QString &characteristicsValue,
                                           const QStringList &preferredNameColumns) const
{
    if (!model) {
        return QString();
    }

    QStringList filters;

    QStringList nameColumns = availableColumns(model, preferredNameColumns);
    if (nameColumns.isEmpty()) {
        const QStringList txtCols = textColumns(model);
        for (const QString &col : txtCols) {
            if (col.startsWith(QStringLiteral("name_"))) {
                nameColumns << col;
            }
        }
        if (nameColumns.isEmpty() && !txtCols.isEmpty()) {
            nameColumns << txtCols.first();
        }
    }

    if (!nameValue.isEmpty() && !nameColumns.isEmpty()) {
        QStringList orParts;
        for (const QString &col : nameColumns) {
            orParts << QStringLiteral("LOWER(%1) LIKE LOWER('%%2%')").arg(col, esc(nameValue));
        }
        filters << QStringLiteral("(%1)").arg(orParts.join(QStringLiteral(" OR ")));
    }

    if (!characteristicsValue.isEmpty()) {
        QStringList charColumns = textColumns(model);
        for (const QString &nameCol : nameColumns) {
            charColumns.removeAll(nameCol);
        }

        if (!charColumns.isEmpty()) {
            QStringList orParts;
            for (const QString &col : charColumns) {
                orParts << QStringLiteral("LOWER(%1) LIKE LOWER('%%2%')").arg(col, esc(characteristicsValue));
            }
            filters << QStringLiteral("(%1)").arg(orParts.join(QStringLiteral(" OR ")));
        }
    }

    return filters.join(QStringLiteral(" AND "));
}

QStringList EntityDataBrowser::availableColumns(QSqlTableModel *model, const QStringList &columns) const
{
    QStringList result;
    for (const QString &column : columns) {
        if (hasColumn(model, column)) {
            result << column;
        }
    }
    return result;
}

QStringList EntityDataBrowser::textColumns(QSqlTableModel *model) const
{
    QStringList result;
    if (!model) {
        return result;
    }

    const QSqlRecord rec = model->record();
    for (int i = 0; i < rec.count(); ++i) {
        const QSqlField field = rec.field(i);
        const int mt = field.metaType().id();
        const QString fieldName = rec.fieldName(i);

        if (fieldName.startsWith(QStringLiteral("id_"))) {
            continue;
        }

        if (mt == QMetaType::QString || mt == QMetaType::QByteArray || mt == QMetaType::QChar) {
            result << fieldName;
        }
    }

    return result;
}

bool EntityDataBrowser::hasColumn(QSqlTableModel *model, const QString &column) const
{
    if (!model) {
        return false;
    }

    return model->record().indexOf(column) >= 0;
}

QString EntityDataBrowser::esc(const QString &value)
{
    QString escaped = value;
    escaped.replace("'", "''");
    return escaped;
}


