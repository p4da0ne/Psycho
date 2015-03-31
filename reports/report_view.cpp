#include "report_view.h"
#include <QVBoxLayout>


ReportView::ReportView(QObject *parent)
    : QObject(parent)
{
  
    
}

ReportView::~ReportView()
{
    
}

void ReportView::preview_dialog(QString html)
{
	preview_dlg = new QDialog;
	preview_dlg->setWindowTitle(tr("Report dialog"));

	report_window = new QTextBrowser;
	report_window->setHtml(html);

	QVBoxLayout *v_lay = new QVBoxLayout;
	v_lay->addWidget(report_window);

	preview_dlg->setLayout(v_lay);
	if(preview_dlg->exec() == QDialog::Accepted)
	{
	
	}

}