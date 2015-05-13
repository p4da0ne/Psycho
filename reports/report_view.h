#ifndef REPORT_VIEW_H
#define REPORT_VIEW_H

#include <QObject>
#include <QDialog>
#include <QTextBrowser>
#include "textprinter.h"
#include <QPrinter>
#include <QPrintDialog>
#include <QFile>
#include <QFileDialog>
#include "textedit.h"

class ReportView : public QObject
{
    Q_OBJECT
public:
    /// Default constructor
    explicit ReportView(QObject *parent=0);
    /// Destructor
    ~ReportView();

	 void preview_dialog(QString html);
     void print_formul(QString html);
     void print_pdf_formul(QString html);

public slots:
     void slot_print_formul();
     void slot_print_pdf_formul();

private:
	 QDialog *preview_dlg;
	 QTextBrowser *report_window;
     TextPrinter *textprinter_;
     QTextDocument *report_window1;
     QFileDialog *fd;
     TextEdit *textedit;
};

#endif // REPORT_VIEW_H
