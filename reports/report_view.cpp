#include "report_view.h"
#include <QVBoxLayout>
#include <QPushButton>

ReportView::ReportView(QObject *parent)
    : QObject(parent)
{
    textprinter_ = new TextPrinter(this);
    report_window1 = new QTextDocument;
    
}

ReportView::~ReportView()
{
    
}
//=========== вывод формуляра на экран (предпросмотр)=============
void ReportView::preview_dialog(QString html)
{
	preview_dlg = new QDialog;
    preview_dlg->setWindowTitle(tr("Вывод формуляра"));
    preview_dlg-> setFixedSize(800,600);
	report_window = new QTextBrowser;
	report_window->setHtml(html);

    QPushButton *printButton = new QPushButton("Печать");
    connect(printButton, SIGNAL(clicked()), this, SLOT(slot_print_formul()));

    QPushButton *PDFButton = new QPushButton("PDF");
    connect(PDFButton, SIGNAL(clicked()), this, SLOT(slot_print_pdf_formul()));

    QHBoxLayout *h_layout = new QHBoxLayout();
    h_layout->addWidget(printButton);
    h_layout->addWidget(PDFButton);

	QVBoxLayout *v_lay = new QVBoxLayout;
	v_lay->addWidget(report_window);
    v_lay->addLayout(h_layout);

	preview_dlg->setLayout(v_lay);
	if(preview_dlg->exec() == QDialog::Accepted)
	{

    }

}
void ReportView::slot_print_formul()
{

    print_formul(report_window->toHtml());

}
void ReportView::slot_print_pdf_formul()
{

    print_pdf_formul(report_window->toHtml());

}
void ReportView::print_formul(QString html)
{
    preview_dlg = new QDialog;
    preview_dlg->setWindowTitle(tr("Вывод формуляра"));

    report_window1->setHtml(html);

    textprinter_->setOrientation(QPrinter::Portrait);
    textprinter_->print(report_window1, tr("Печать документа"));
}

void ReportView::print_pdf_formul(QString html)
{
    QFile filepdf;
    QString filename;
        //fd = new QFileDialog;
        //filename = fd -> getSaveFileName(this, tr("Сохранить"),"c:/test/test.pdf", tr("PDF (*.pdf)"));

    preview_dlg = new QDialog;
    preview_dlg->setWindowTitle(tr("Вывод формуляра"));
    report_window1->setHtml(html);

    filepdf.setFileName(filename);
    filepdf.open(QIODevice::WriteOnly);
    filepdf.close();

    textprinter_->setOrientation(QPrinter::Portrait);
    textprinter_->exportPdf(report_window1,"Сохранить",filename);
}
