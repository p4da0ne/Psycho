#ifndef MAPSETTINGSDIALOG_H
#define MAPSETTINGSDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QToolButton>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

class MapSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MapSettingsDialog(QWidget *parent = 0);
    
signals:

private slots:
    void slotAccepted();
	void slotGetMapFilePath();
	void slotGetRscFilePath();

public:
    QLabel mapPathLabel, rscPathLabel;
    QLineEdit mapPathLineEdit, rscPathLineEdit;
	QToolButton mapPathButton, rscPathButton;
	QPushButton okButton, cancelButton;
    QHBoxLayout mapPathLay,rscPathLay,buttonLay;
    QVBoxLayout dlgLay;
private:
    QString mapPath;
	QString rscPath;
	QSettings *settings;
    void messageToUser(QString message);
	void getMapSettings();
	void saveSettings();
};

#endif // MAPSETTINGSDIALOG_H
