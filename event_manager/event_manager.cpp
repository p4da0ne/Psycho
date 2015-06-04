#include "event_manager.h"

EventManager::EventManager(QWidget *parent)
: QWidget(parent)
{
    QGridLayout * grid = new QGridLayout(this);
    tableView = new QTableView(this);
    grid->addWidget(tableView,0,0);
    this->setLayout(grid);
    eventsModel = new EventsModel();
    tableView->setModel(eventsModel);
}

EventManager::~EventManager(){

}
