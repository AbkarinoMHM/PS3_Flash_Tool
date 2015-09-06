#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QVector>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ps3nand.h"
#include "about.h"

//Public
QString strNAND0File;
QString strNAND1File;
QString strNANDFile;
QString strPatchFile;
int     intSelectedProcess = -1;

PS3NAND* PS3NandFlash = new PS3NAND();

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //ui->cboProcess->currentIndexChanged(0);

    QObject::connect(PS3NandFlash, SIGNAL(progressChanged(const QString ,const QString ,const int)), this, SLOT(setProgress(const QString ,const QString, const int)), Qt::QueuedConnection);
    QObject::connect(ui->mnuAbout, SIGNAL(triggered()), this, SLOT(ShowAbout()), Qt::QueuedConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setProgress(const QString processName,const QString status ,const int val)
{
    ui->txtProcessLog->append(tr("%1").arg(status));
    ui->proProcess->setValue(val);
}

void MainWindow::on_txtNAND0_textChanged(const QString &arg1)
{
    QString tempString = arg1;

    if(QGuiApplication::platformName() == "windows")
    {
        tempString.replace('/','\\');
        ui->txtNAND0->setText(tempString);
    }

}

void MainWindow::on_txtNAND1_textChanged(const QString &arg1)
{
    QString tempString = arg1;

    if(QGuiApplication::platformName() == "windows")
    {
        tempString.replace('/','\\');
        ui->txtNAND1->setText(tempString);
    }
}

void MainWindow::on_txtNAND_textChanged(const QString &arg1)
{
    QString tempString = arg1;

   if(QGuiApplication::platformName() == "windows")
   {
        tempString.replace('/','\\');
        ui->txtNAND->setText(tempString);
   }

    if((ui->cboProcess->currentIndex() == 1) && QFile::exists(ui->txtNAND->text()))
    {
        QMessageBox::StandardButton res = QMessageBox::warning(NULL, tr("File Exist"), tr("File already exist. Do you want to replace it?"),
                    QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

        if(res == QMessageBox::No)
        {
            ui->txtNAND->clear();
            strNANDFile = "";
        }

    }
}

void MainWindow::on_btnNAND0_clicked()
{
   strNAND0File = QFileDialog::getOpenFileName(this, tr("Please Select PS3 NAND Image 0 / Top"), 0, tr("PS3 NAND Image File (*.bin);;Any File (*.*)"));
   ui->txtNAND0->setText(strNAND0File);
}

void MainWindow::on_btnNAND1_clicked()
{
    strNAND1File = QFileDialog::getOpenFileName(this, tr("Please Select PS3 NAND Image 1 / Bottom"), 0, tr("PS3 NAND Image File (*.bin);;Any File (*.*)"));
    ui->txtNAND1->setText(strNAND1File);
}

void MainWindow::on_btnNAND_clicked()
{
    if(ui->cboProcess->currentIndex() == 1)
    {
        strNANDFile = QFileDialog::getSaveFileName(this, tr("Please Select Where To Save Unified PS3 NAND Image"), 0, tr("PS3 NAND Image File (*.bin);;Any File (*.*)"), 0, QFileDialog::ShowDirsOnly);
        ui->txtNAND->setText(strNANDFile);
    }else if(ui->cboProcess->currentIndex() == 2)
    {
        strNANDFile = QFileDialog::getOpenFileName(this, tr("Please Select Unified PS3 NAND Image"), 0, tr("PS3 NAND Image File (*.bin);;Any File (*.*)"));
        ui->txtNAND->setText(strNANDFile);
    }
}

void MainWindow::on_btnClearLog_clicked()
{
    ui->txtProcessLog->clear();
}

void MainWindow::on_cboProcess_currentIndexChanged(int index)
{
    switch(index)
    {
    case 0:
    {
        ui->btnNAND0->setEnabled(true);
        ui->btnNAND1->setEnabled(true);
        ui->btnNAND->setEnabled(false);
        ui->txtNAND0->setEnabled(true);
        ui->txtNAND1->setEnabled(true);
        ui->txtNAND->setEnabled(false);
        ui->cboPatches->setEnabled(true);
        ui->lblNAND->setText("Output Interleaved NAND:");
        ui->chkDiffFile->setEnabled(false);
        ui->chkExtractConsoleData->setEnabled(true);
        ui->chkKeepUnifiedDump->setEnabled(true);
        ui->mnuLoadInterleavedNANDDump->setEnabled(false);
        ui->mnuSaveInterleavedNANDDump->setEnabled(false);
        break;
    }
    case 1:
    {
        ui->btnNAND0->setEnabled(true);
        ui->btnNAND1->setEnabled(true);
        ui->btnNAND->setEnabled(true);
        ui->txtNAND0->setEnabled(true);
        ui->txtNAND1->setEnabled(true);
        ui->txtNAND->setEnabled(true);
        ui->cboPatches->setEnabled(false);
        ui->lblNAND->setText("Output Interleaved NAND:");
        ui->chkDiffFile->setEnabled(false);
        ui->chkExtractConsoleData->setEnabled(true);
        ui->chkKeepUnifiedDump->setEnabled(false);
        ui->mnuLoadInterleavedNANDDump->setEnabled(false);
        ui->mnuSaveInterleavedNANDDump->setEnabled(true);
        break;
    }
    case 2:
    {
        ui->btnNAND0->setEnabled(true);
        ui->btnNAND1->setEnabled(true);
        ui->btnNAND->setEnabled(true);
        ui->txtNAND0->setEnabled(true);
        ui->txtNAND1->setEnabled(true);
        ui->txtNAND->setEnabled(true);
        ui->cboPatches->setEnabled(false);
        ui->lblNAND->setText("Input Interleaved NAND:");
        ui->chkDiffFile->setEnabled(true);
        ui->chkExtractConsoleData->setEnabled(false);
        ui->chkKeepUnifiedDump->setEnabled(false);
        ui->mnuLoadInterleavedNANDDump->setEnabled(true);
        ui->mnuSaveInterleavedNANDDump->setEnabled(false);
        break;
    }
    case 3:
    {
        ui->btnNAND0->setEnabled(false);
        ui->btnNAND1->setEnabled(false);
        ui->btnNAND->setEnabled(true);
        ui->txtNAND0->setEnabled(false);
        ui->txtNAND1->setEnabled(false);
        ui->txtNAND->setEnabled(true);
        ui->cboPatches->setEnabled(false);
        ui->lblNAND->setText("Input Interleaved NAND:");
        ui->chkDiffFile->setEnabled(false);
        ui->chkExtractConsoleData->setEnabled(false);
        ui->chkKeepUnifiedDump->setEnabled(false);
        break;
    }
    case 4:
    {
        ui->btnNAND0->setEnabled(false);
        ui->btnNAND1->setEnabled(false);
        ui->btnNAND->setEnabled(true);
        ui->txtNAND0->setEnabled(false);
        ui->txtNAND1->setEnabled(false);
        ui->txtNAND->setEnabled(true);
        ui->cboPatches->setEnabled(true);
        ui->lblNAND->setText("Input Interleaved NAND:");
        ui->chkDiffFile->setEnabled(false);
        ui->chkExtractConsoleData->setEnabled(false);
        ui->chkKeepUnifiedDump->setEnabled(false);
        break;
    }
//    case 5:
//    {
//        ui->btnNAND0->setEnabled(true);
//        ui->btnNAND1->setEnabled(true);
//        ui->btnNAND->setEnabled(true);
//        ui->txtNAND0->setEnabled(true);
//        ui->txtNAND1->setEnabled(true);
//        ui->txtNAND->setEnabled(true);
//        ui->cboPatches->setEnabled(false);
//        ui->lblNAND->setText("Clean Donor NAND:");
//        ui->chkDiffFile->setEnabled(true);
//        ui->chkExtractConsoleData->setEnabled(false);
//        break;
//    }
    default:
    {
        break;
    }
    }

    intSelectedProcess = ui->cboProcess->currentIndex();
}

void MainWindow::on_btnStart_clicked()
{
    bool result;
    QVector<int> nand0SortedBlocks(NandBlockCount, -1);
    QVector<int> nand1SortedBlocks(NandBlockCount, -1);
    QVector<int> nand0BadBlocks(NandBlockCount, -1);
    QVector<int> nand1BadBlocks(NandBlockCount, -1);
    int nand0BadBlocksCount = 0;
    int nand1BadBlocksCount = 0;
    int nand0ModifiedBlocksCount = 0;
    int nand1ModifiedBlocksCount = 0;
    int selectedPatch = ui->cboPatches->currentIndex();
    bool createDiffFile = ui->chkDiffFile->isChecked();
    bool extractConsoleData = ui->chkExtractConsoleData->isChecked();
    bool keepUnifiedDump = ui->chkKeepUnifiedDump->isChecked();

    strNAND0File = ui->txtNAND0->text();
    strNAND1File = ui->txtNAND1->text();
    strNANDFile = ui->txtNAND->text();

    ui->btnStart->setEnabled(false);

    ui->txtProcessLog->clear();
    ui->proProcess->setMinimum(0);
    ui->proProcess->setMaximum(NandBlockCount);
    ui->proProcess->setValue(0);

    intSelectedProcess = ui->cboProcess->currentIndex();

    if(strNAND0File.isEmpty() && strNAND1File.isEmpty() && strNANDFile.isEmpty())
    {
        QMessageBox::critical(this, "Error you must select input/output files first.", "Error in input/output files, can not continue!");
        ui->btnStart->setEnabled(true);
        return;
    }

    switch(intSelectedProcess)
    {
    case 0:
    case 1:
    case 2:
    {
        if(intSelectedProcess == 0)
        {
            ui->txtProcessLog->append(tr("Auto patch raw nand flash dumps process started:"));
        }else if(intSelectedProcess == 1)
        {
            ui->txtProcessLog->append(tr("Interleaving process started:"));
        }else{
            ui->txtProcessLog->append(tr("Deinterleaving process started:"));
        }
        ui->txtProcessLog->append(tr("Sorting nand flash 0 blocks please wait..."));
        result = PS3NandFlash->SortNandBlocks(strNAND0File, &nand0SortedBlocks, &nand0BadBlocks, &nand0BadBlocksCount);
        if(!result)
        {
            QMessageBox::critical(this, "Error sorting nand 0 flash.", "Error sorting nand 0 flash, can not continue!");
            break;
        }

        ui->txtProcessLog->append(tr("Sorting nand flash 1 blocks please wait..."));
        result = PS3NandFlash->SortNandBlocks(strNAND1File, &nand1SortedBlocks, &nand1BadBlocks, &nand1BadBlocksCount);
        if(!result)
        {
            QMessageBox::critical(this, "Error sorting nand 1 flash.", "Error sorting nand 1 flash, can not continue!");
            break;
        }

        if((intSelectedProcess == 0) || (intSelectedProcess == 1))
        {
            if(intSelectedProcess == 0)
            {
                QFileInfo nand0FileInfo(strNAND0File);

                strNANDFile = QString("%1\\Unified.bin").arg(nand0FileInfo.path());

                if(QApplication::platformName() == "windows")
                {
                    strNANDFile.replace("/", "\\");
                }
            }

            ui->txtProcessLog->append(tr("Interleaving sorted flashes please wait..."));
            result = PS3NandFlash->InterleaveNandFlashes(strNAND0File, strNAND1File, strNANDFile, &nand0SortedBlocks, &nand1SortedBlocks);
            if(!result)
            {
                QMessageBox::critical(this,tr("Error in interleaving process!") ,tr("Error operation not completed successfully!"));
                break;
            }

            if(extractConsoleData)
            {
                result = PS3NandFlash->ExtractInterleavedNANDFlash(strNANDFile, true);
                if(!result)
                {
                    ui->txtProcessLog->append(tr("Error extracting interleaved nand flash."));

                    QMessageBox::critical(this, tr("Error extracting interleaved nand flash."), tr("Error operation not completed successfully!"));
                    break;
                }
            }

            if(intSelectedProcess == 1)
            {
                ui->txtProcessLog->append(tr("Interleaving process done successfully!"));

                QMessageBox::information(this,tr("Interleaving process done successfully!") ,tr("Operation Completed Successfully!\nNAND 0 bad block count: %1 block.\nNAND 1 bad block count: %2 block.").arg(nand0BadBlocksCount).arg(nand1BadBlocksCount));
                break;
            }
        }

        //patch unified dump here
        if(intSelectedProcess == 0)
        {
            result = PS3NandFlash->PatchInterleavedNANDFlash(strNANDFile, selectedPatch);
            if(!result)
            {
                break;
            }

            QFile::remove(strNANDFile);
        }

        if((intSelectedProcess == 0) || (intSelectedProcess == 2))
        {
            if(intSelectedProcess == 0)
            {
                 QFileInfo nand0FileInfo(strNAND0File);

                strNANDFile = QString("%1\\Unified-patched.bin").arg(nand0FileInfo.path());

                if(QApplication::platformName() == "windows")
                {
                    strNANDFile.replace("/", "\\");
                }
            }

            ui->txtProcessLog->append(tr("Deinterleaving sorted flashes please wait..."));

            result = PS3NandFlash->DeinterleaveNandFlashes(strNAND0File, strNAND1File, strNANDFile, \
                                                           &nand0SortedBlocks, &nand1SortedBlocks, \
                                                           &nand0BadBlocks, &nand1BadBlocks, \
                                                           &nand0ModifiedBlocksCount, &nand1ModifiedBlocksCount, createDiffFile);
            if(!result)
            {
                QMessageBox::critical(this,tr("Error in deinterleaving process!") ,tr("Error operation not completed successfully!"));
                break;
            }

            if(intSelectedProcess == 2)
            {
                ui->txtProcessLog->append(tr("Deinterleaving process done successfully!"));

                QMessageBox::information(this,tr("Denterleaving process done successfully!") ,tr("Operation Completed Successfully!\nNAND 0 bad block count: %1 block.\nNAND 1 bad block count: %2 block.\nNAND 0 modified blocks count: %3 block.\nNAND 1 modified blocks count: %4 block.").arg(nand0BadBlocksCount).arg(nand1BadBlocksCount).arg(nand0ModifiedBlocksCount).arg(nand1ModifiedBlocksCount));

                break;
            }

        }

        if(intSelectedProcess == 0)
        {
            //del unified dump here
            if(!keepUnifiedDump)
            {
                QFile::remove(strNANDFile);
            }

            ui->txtProcessLog->append(tr("Auto patch raw nand flash dumps process done successfully!"));

            QMessageBox::information(this, tr("Auto patch raw nand flash dumps process done successfully!"), tr("Operation Completed Successfully!\nNAND 0 bad block count: %1 block.\nNAND 1 bad block count: %2 block.\nNAND 0 modified blocks count: %3 block.\nNAND 1 modified blocks count: %4 block.").arg(nand0BadBlocksCount).arg(nand1BadBlocksCount).arg(nand0ModifiedBlocksCount).arg(nand1ModifiedBlocksCount));
        }

        break;
    }
    case 3: //extract interleaved nand flash.
    {
        ui->proProcess->setMaximum(9);

        ui->txtProcessLog->append(tr("Extracting interleaved nand flash data process started:"));
        result = PS3NandFlash->ExtractInterleavedNANDFlash(strNANDFile, true);
        if(!result)
        {
            ui->txtProcessLog->append(tr("Error extracting interleaved nand flash."));

            QMessageBox::critical(this, tr("Error extracting interleaved nand flash."), tr("Error operation not completed successfully!"));
            break;
        }

        ui->txtProcessLog->append(tr("Extracting interleaved nand flash data process done successfully!"));

        QMessageBox::information(this, tr("Extracting interleaved nand flash data process done successfully!"), tr("Operation Completed Successfully!"));
        break;
    }
    case 4: //patch interleaved nand flash
    {
        ui->proProcess->setMaximum(5);

        ui->txtProcessLog->append(tr("Patching interleaved nand flash process started:"));
        result = PS3NandFlash->PatchInterleavedNANDFlash(strNANDFile, selectedPatch);
        if(!result)
        {
            break;
        }

        ui->txtProcessLog->append(tr("Patching interleaved nand flash process done successfully!"));

        QMessageBox::information(this, tr("Patching interleaved nand flash process done successfully!"), tr("Operation Completed Successfully!"));
        break;
    }
//    case 5:
//    {
//        break;
//    }
    default:
    {
        break;
    }
    }

    ui->btnStart->setEnabled(true);
}

void MainWindow::ShowAbout()
{
    about *frmAbout = new about(this);
    frmAbout->exec();
}

