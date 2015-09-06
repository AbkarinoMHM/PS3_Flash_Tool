#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnNAND0_clicked();

    void on_btnNAND1_clicked();

    void on_btnNAND_clicked();

    void on_btnClearLog_clicked();

    void on_cboProcess_currentIndexChanged(int index);

    void on_btnStart_clicked();

    void on_txtNAND0_textChanged(const QString &arg1);

    void on_txtNAND1_textChanged(const QString &arg1);

    void on_txtNAND_textChanged(const QString &arg1);

    void setProgress(const QString processName,const QString status ,const int val);

    void ShowAbout();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
