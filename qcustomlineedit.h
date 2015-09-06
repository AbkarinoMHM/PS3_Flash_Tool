#ifndef QCUSTOMLINEEDIT_H
#define QCUSTOMLINEEDIT_H

#include <QWidget>
#include <QLineEdit>

class QCustomLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit QCustomLineEdit(QWidget *parent = 0);
    ~QCustomLineEdit();
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
signals:

public slots:
};

#endif // QCUSTOMLINEEDIT_H
