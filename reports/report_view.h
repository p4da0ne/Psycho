#ifndef REPORT_VIEW_H
#define REPORT_VIEW_H

#include <QObject>
#include <QDialog>
#include <QTextBrowser>

class ReportView : public QObject
{
    Q_OBJECT
public:
    /// Default constructor
    explicit ReportView(QObject *parent=0);
    /// Destructor
    ~ReportView();
	 void preview_dialog(QString html);
	
private:
	 QDialog *preview_dlg;
	 QTextBrowser *report_window;
};

#endif // REPORT_VIEW_H
