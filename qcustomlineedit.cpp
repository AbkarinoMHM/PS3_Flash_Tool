#include <QtGui>
#include "qcustomlineedit.h"

QCustomLineEdit::QCustomLineEdit(QWidget *parent) : QLineEdit(parent)
{

}

QCustomLineEdit::~QCustomLineEdit()
{

}

void QCustomLineEdit::dragEnterEvent(QDragEnterEvent *event)
{
    // accept just text/uri-list mime format
    if (event->mimeData()->hasFormat("text/uri-list"))
    {
        event->acceptProposedAction();
    }
}


void QCustomLineEdit::dropEvent(QDropEvent *event)
{
    QList<QUrl> urlList;
    QString fName;
    QFileInfo info;

    if(event->mimeData()->hasUrls())
    {
        urlList = event->mimeData()->urls(); // returns list of QUrls

        // if just text was dropped, urlList is empty (size == 0)
        if(urlList.size() > 0) // if at least one QUrl is present in list
        {
            fName = urlList[0].toLocalFile(); // convert first QUrl to local path
            info.setFile(fName); // information about file
            if(info.isFile())
            {
                setText(fName); // if is file, setText
            }

        }
    }

    event->acceptProposedAction();
}
