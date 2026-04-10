#ifndef ENTITY_DATA_BROWSER_H
#define ENTITY_DATA_BROWSER_H

#include <QWidget>

class QCheckBox;
class QComboBox;
class QDateTimeEdit;
class QLineEdit;
class QSqlTableModel;
class QTableView;
class QTabWidget;

class EntityDataBrowser : public QWidget
{
    Q_OBJECT

public:
    explicit EntityDataBrowser(QWidget *parent = nullptr);

private:
    void setupObjectsTab();
    void setupPeopleTab();
    void setupSpecialConditionsTab();
    void setupEventsTab();

    void setupModel(QSqlTableModel *model, const QString &tableName);
    void applyObjectsFilter();
    void applyPeopleFilter();
    void applySpecialConditionsFilter();
    void applyEventsFilter();

    void addRow(QSqlTableModel *model, QTableView *view);
    void removeSelectedRows(QSqlTableModel *model, QTableView *view, const QString &entityCaption);
    void saveModel(QSqlTableModel *model, const QString &entityCaption);
    void revertModel(QSqlTableModel *model);

    QString buildTextFilter(QSqlTableModel *model,
                            const QString &nameValue,
                            const QString &characteristicsValue,
                            const QStringList &preferredNameColumns) const;
    QStringList availableColumns(QSqlTableModel *model, const QStringList &columns) const;
    QStringList textColumns(QSqlTableModel *model) const;
    bool hasColumn(QSqlTableModel *model, const QString &column) const;

    static QString esc(const QString &value);

private:
    QTabWidget *m_tabs = nullptr;

    QComboBox *m_objectTypeCombo = nullptr;
    QLineEdit *m_objectNameFilter = nullptr;
    QLineEdit *m_objectCharFilter = nullptr;
    QTableView *m_objectView = nullptr;
    QSqlTableModel *m_objectModel = nullptr;

    QLineEdit *m_peopleNameFilter = nullptr;
    QLineEdit *m_peopleCharFilter = nullptr;
    QTableView *m_peopleView = nullptr;
    QSqlTableModel *m_peopleModel = nullptr;

    QLineEdit *m_specialNameFilter = nullptr;
    QLineEdit *m_specialCharFilter = nullptr;
    QTableView *m_specialView = nullptr;
    QSqlTableModel *m_specialModel = nullptr;

    QLineEdit *m_eventNameFilter = nullptr;
    QLineEdit *m_eventCharFilter = nullptr;
    QCheckBox *m_eventFromCheck = nullptr;
    QDateTimeEdit *m_eventFromEdit = nullptr;
    QCheckBox *m_eventToCheck = nullptr;
    QDateTimeEdit *m_eventToEdit = nullptr;
    QTableView *m_eventView = nullptr;
    QSqlTableModel *m_eventModel = nullptr;
};

#endif // ENTITY_DATA_BROWSER_H
