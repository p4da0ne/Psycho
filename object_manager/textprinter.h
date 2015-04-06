///////////////////////////////////////////////////////////////////////////////
// textprinter.h
// -------------------
///////////////////////////////////////////////////////////////////////////////

#ifndef TEXTPRINTER_H
#define TEXTPRINTER_H

#include <QObject>
#include <QPrinter>

class QTextDocument;
class QWidget;


class TextPrinter : public QObject
{
    Q_OBJECT
public:
    /// Default constructor
    explicit TextPrinter(QObject *parent=0);
    /// Destructor
    ~TextPrinter();

    /// Print the document
    void print(const QTextDocument *document,
               const QString &caption=QString());
    /// Export the document to PDF
    void exportPdf(const QTextDocument *document,
                   const QString &caption=QString(),
                   const QString &filename=QString());
	void exportPdf(const QTextDocument *document,
					const QString &caption,
					const QString &filename,
					int save);
    /// Display the document in a preview dialog
    void preview(const QTextDocument *document,
                 const QString &caption=QString());

    /// Get page size
    QPrinter::PageSize pageSize() const;
    /// Set page size
    void setPageSize(QPrinter::PageSize size);
    /// Get page orientation
    QPrinter::Orientation orientation() const;
    /// Set page orientation
    void setOrientation(QPrinter::Orientation orientation);

    /// Get left margin width
    double leftMargin() const;
    /// Set left margin width
    void setLeftMargin(double margin);
    /// Get right margin width
    double rightMargin() const;
    /// Set right margin width
    void setRightMargin(double margin);
    /// Get top margin width
    double topMargin() const;
    /// Set top margin width
    void setTopMargin(double margin);
    /// Get bottom margin width
    double bottomMargin() const;
    /// Set bottom margin width
    void setBottomMargin(double margin);
    /// Set all margins
    void setMargins(double margin);

    /// Get spacing between content and header and footer
    double spacing() const;
    /// Set spacing between content and header and footer
    void setSpacing(double spacing);

    /// Get header size
    double headerSize() const;
    /// Set header size
    void setHeaderSize(double size);
    /// Set header rule size
    double headerRule() const;
    /// Get header rule size
    void setHeaderRule(double pointsize);
    /// Get header text
    const QString &headerText() const;
    /// Set header text
    void setHeaderText(const QString &text);

    /// Get footer size
    double footerSize() const;
    /// Set footer size
    void setFooterSize(double size);
    /// Get footer rule size
    double footerRule() const;
    /// Set footer rule size
    void setFooterRule(double pointsize);
    /// Get footer text
    const QString &footerText() const;
    /// Set footer text
    void setFooterText(const QString &text);

    /// Get date format
    const QString &dateFormat() const;
    /// Set date format
    void setDateFormat(const QString &format);

private:
    // not copyable
    TextPrinter(const TextPrinter&);
    TextPrinter &operator=(const TextPrinter&);

    // return paper rect
    QRectF paperRect(QPaintDevice *device);
    // return printable rects
    QRectF contentRect(QPaintDevice *device);
    QRectF headerRect(QPaintDevice *device);
    QRectF footerRect(QPaintDevice *device);

    // paint specific page
    void paintPage(QPainter *painter,
                   QTextDocument *document,
                   int pagenum);
private slots:
    // common print routine
    void print(QPrinter *printer);

private:
    QWidget *parent_;
    QPrinter *printer_;

    QTextDocument *tempdoc_;

    double leftmargin_;
    double rightmargin_;
    double topmargin_;
    double bottommargin_;
    double spacing_;

    double headersize_;
    double headerrule_;
    QString headertext_;
    double footersize_;
    double footerrule_;
    QString footertext_;

    QString dateformat_;
};

#endif // TEXTPRINTER_H
