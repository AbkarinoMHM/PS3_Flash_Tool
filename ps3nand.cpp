#include <QCoreApplication>
#include <QByteArray>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QFileInfo>
#include <QMessageBox>
#include <QList>
#include "ps3nand.h"
#include "misc.h"


void PS3NAND::EncodeECC(char* buffer, unsigned int sectorIndex, int encodeBit, bool encodeBitSet)
{
    encodeBit ^= 0x7;

    if(encodeBitSet)
    {
        for(int counter = 0; counter < 10; counter++)
        {
            buffer[counter] = (buffer[counter] ^ PS3NANDECCLookupTableBitSet[((sectorIndex * 80) + (encodeBit * 10)) + counter]);
        }
    }else{
        for(int counter = 0; counter < 10; counter++)
        {
             buffer[counter] = (buffer[counter] ^ PS3NANDECCLookupTableBitSet[counter]);
        }
    }
}

QByteArray PS3NAND::CalculateECC(char* bufferSector)
{
    QByteArray buffer(10, 0);

    for (short i = 0; i < NandSectorSize; i++)
    {
        char SectorByteData = bufferSector[i];
        SectorByteData = (char) (SectorByteData ^ 0xff);

        if ((SectorByteData & 0x80) == 0)
        {
            EncodeECC(buffer.data(), i, 7, false);
        }
        else
        {
            EncodeECC(buffer.data(), i, 7, true);
        }
        if ((SectorByteData & 0x40) == 0)
        {
            EncodeECC(buffer.data(), i, 6, false);
        }
        else
        {
            EncodeECC(buffer.data(), i, 6, true);
        }
        if ((SectorByteData & 0x20) == 0)
        {
            EncodeECC(buffer.data(), i, 5, false);
        }
        else
        {
            EncodeECC(buffer.data(), i, 5, true);
        }
        if ((SectorByteData & 0x10) == 0)
        {
            EncodeECC(buffer.data(), i, 4, false);
        }
        else
        {
            EncodeECC(buffer.data(), i, 4, true);
        }
        if ((SectorByteData & 8) == 0)
        {
            EncodeECC(buffer.data(), i, 3, false);
        }
        else
        {
            EncodeECC(buffer.data(), i, 3, true);
        }
        if ((SectorByteData & 4) == 0)
        {
            EncodeECC(buffer.data(), i, 2, false);
        }
        else
        {
            EncodeECC(buffer.data(), i, 2, true);
        }
        if ((SectorByteData & 2) == 0)
        {
            EncodeECC(buffer.data(), i, 1, false);
        }
        else
        {
            EncodeECC(buffer.data(), i, 1, true);
        }
        if ((SectorByteData & 1) == 0)
        {
            EncodeECC(buffer.data(), i, 0, false);
        }
        else
        {
            EncodeECC(buffer.data(), i, 0, true);
        }
    }

    for(int j = 0; j < 10; j++)
    {
        buffer.data()[j] = (buffer.data()[j] ^ 0xff);
    }

    return buffer;
}

QByteArray PS3NAND::CalculateECC(char* buffer, unsigned long offset)
{
    char sector[NandSectorSize];

    memcpy(sector, &buffer[offset], NandSectorSize);

    return CalculateECC(sector);
}

void PS3NAND::EncodeECC(QByteArray* buffer, unsigned int sectorIndex, int encodeBit, bool encodeBitSet)
{
    encodeBit ^= 0x7;

    if(encodeBitSet)
    {
        for(int counter = 0; counter < 10; counter++)
        {
            buffer->data()[counter] = buffer->data()[counter] ^ PS3NANDECCLookupTableBitSet[(sectorIndex * 80) + (encodeBit * 10) + counter];
        }
    }else{
        for(int counter = 0; counter < 10; counter++)
        {
            buffer->data()[counter] = buffer->data()[counter] ^ PS3NANDECCLookupTableBitUnset[counter];
        }
    }
}

void PS3NAND::CalculateECC(QByteArray* sector, unsigned long offset, QByteArray* ecc)
{
    int eccOffset = 0;
    QByteArray buffer(10, 0);
    QByteArray sectorBuffer(NandSectorSize, 0);

    memcpy(sectorBuffer.data(), &sector->data()[offset], NandSectorSize);

    while(eccOffset < NandSectorSize)
    {
        unsigned char SectorByteData = sectorBuffer[eccOffset] ^ 0xff;

        if((SectorByteData & 0x80) == 0)
        {
            EncodeECC(&buffer, eccOffset, 7, false);
        }
        else
        {
            EncodeECC(&buffer, eccOffset, 7, true);
        }
        if((SectorByteData & 0x40) == 0)
        {
            EncodeECC(&buffer, eccOffset, 6, false);
        }
        else
        {
            EncodeECC(&buffer, eccOffset, 6, true);
        }
        if((SectorByteData & 0x20) == 0)
        {
            EncodeECC(&buffer, eccOffset, 5, false);
        }
        else
        {
            EncodeECC(&buffer, eccOffset, 5, true);
        }
        if((SectorByteData & 0x10) == 0)
        {
            EncodeECC(&buffer, eccOffset, 4, false);
        }
        else
        {
            EncodeECC(&buffer, eccOffset, 4, true);
        }
        if((SectorByteData & 8) == 0)
        {
            EncodeECC(&buffer, eccOffset, 3, false);
        }
        else
        {
            EncodeECC(&buffer, eccOffset, 3, true);
        }
        if((SectorByteData & 4) == 0)
        {
            EncodeECC(&buffer, eccOffset, 2, false);
        }
        else
        {
            EncodeECC(&buffer, eccOffset, 2, true);
        }
        if((SectorByteData & 2) == 0)
        {
            EncodeECC(&buffer, eccOffset, 1, false);
        }
        else
        {
            EncodeECC(&buffer, eccOffset, 1, true);
        }
        if((SectorByteData & 1) == 0)
        {
            EncodeECC(&buffer, eccOffset, 0, false);
        }
        else
        {
            EncodeECC(&buffer, eccOffset, 0, true);
        }

        eccOffset++;
    }


    for(int counter = 0; counter < 10; counter++)
    {
        (buffer.data()[counter]) = buffer.data()[counter] ^ 0xff;
    }

    memcpy(ecc->data(), buffer.data(), 10);

}

PS3SpareData PS3NAND::GetNANDSpareData(QFile* flashFile, int pageNo)
{
    bool result, bolIsFileOpened = false;
    QByteArray      pageSpareData;
    PS3SpareData    spareData;

    try {
        if(!flashFile->isOpen())
        {
            result = flashFile->open(QIODevice::ReadOnly);
            bolIsFileOpened = true;
        }

        result = flashFile->seek((pageNo * NandPageRawSize) + NandPageDataSize);
        pageSpareData = flashFile->read(NandPageSpareDataSize);

        if((pageSpareData.isNull()) | (pageSpareData.length() != NandPageSpareDataSize))
        {
            //return NULL;
        }

        if(bolIsFileOpened)
        {
           flashFile->close();
        }

        memcpy(&spareData, pageSpareData.data(), NandPageSpareDataSize);

        return spareData;
    } catch (...) {
        //return NULL;
    }
}

bool PS3NAND::SortNandBlocks(QString flashFilePath, QVector<int>* sortedBlocks, QVector<int>* nandBadBlocks, int* badBlocksCount)
{
    QList<PS3NandBlockInfo> mappedBlocks;
    QList<int> unmappedBlocks;
    QList<int> badBlocks;
    QVector<int> nandBlockInfo(NandBlockCount, -1);

    PS3NandSortingAlgo sortingAlgo = Algo_0000;

    QFile flashFile(flashFilePath);

    int intMappingVal = -1;
    int intCalculatedMappingVal = -1;

    if(!flashFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(NULL, tr("%1 error.").arg("PlayStation 3 Flash Tool"), tr("Flash file %1 can not be opened!").arg(flashFilePath));
        return false;
    }

    for(int counter = 0; counter < NandBlockCount; counter++)
    {
        PS3SpareData spareData;
        PS3NandBlockInfo block;

        spareData = GetNANDSpareData(&flashFile, (counter * NandPagePerBlock));

        block.phyBlockIdx = counter;
        block.spareData = spareData;

        if(block.spareData.valid == 0xFF)
        {
            if((block.spareData.map[0] == 0xFF) && (block.spareData.map[1] == 0xFF) && (block.spareData.unknown[0] == 0xFF) && (block.spareData.unknown[1] == 0xFF) )
            {
                unmappedBlocks.append(block.phyBlockIdx);
            }else{
                mappedBlocks.append(block);

                //getting bootloader block 0 to determine the sorting algorithm used.
                if((block.spareData.map[0] == 0x0) && (block.spareData.map[1] == 0x2))
                {
                    sortingAlgo = (PS3NandSortingAlgo)block.phyBlockIdx;
                }
            }
        }else{
            badBlocks.append(block.phyBlockIdx);
        }
    }

    flashFile.close();

    //copy bad blocks here
    *badBlocksCount = badBlocks.count();
    nandBadBlocks->fromList(badBlocks);

    //mapping all mapped blocks
    foreach (PS3NandBlockInfo block, mappedBlocks)
    {
        intMappingVal = (((block.spareData.map[0] / 8) * 8) + block.spareData.map[0]);

        if((block.spareData.map[1] & 0x1))
        {
            intMappingVal |= 0x200;
        }

        intCalculatedMappingVal = ((((block.spareData.map[0] & 0x7) << 0x6) | ((block.spareData.map[1] >> 0x1))) * sortingAlgo);

        if(intCalculatedMappingVal > 0x3FF)
        {
            intCalculatedMappingVal = ((intCalculatedMappingVal ^ 0x3FF) & 0x3FF);
        }

        switch(sortingAlgo)
        {
        case Algo_0001:
        {
            if(block.phyBlockIdx == (intCalculatedMappingVal | 0x200))
            {
                intMappingVal |= 0x8;
            }
            break;
        }
        case Algo_0002:
        {
            if(block.phyBlockIdx != intCalculatedMappingVal)
            {
                intMappingVal |= 0x8;
            }
            break;
        }
        case Algo_0004:
        {
            if(block.phyBlockIdx != intCalculatedMappingVal)
            {
                intMappingVal |= 0x8;
            }
            break;
        }
        case Algo_0008:
        {
            if((block.phyBlockIdx = (intCalculatedMappingVal + 2)) || (block.phyBlockIdx = (intCalculatedMappingVal - 2))
                || (block.phyBlockIdx = (0x400 - intCalculatedMappingVal + 2)) || (block.phyBlockIdx = (intCalculatedMappingVal - 3)))
            {
                intMappingVal |=  0x8;
            }
            break;
        }
        case Algo_0016:
        {
            if(!((block.phyBlockIdx = intCalculatedMappingVal) || (block.phyBlockIdx = (0x400 - intCalculatedMappingVal))
                 || (block.phyBlockIdx = (intCalculatedMappingVal - 1)) || (block.phyBlockIdx = (0x400 - intCalculatedMappingVal + 1))
                 || (block.phyBlockIdx = (intCalculatedMappingVal - 2)) || (block.phyBlockIdx = (0x400 - intCalculatedMappingVal + 2))
                 || (block.phyBlockIdx = (intCalculatedMappingVal - 3)) || (block.phyBlockIdx = (0x400 - intCalculatedMappingVal + 3))))
            {
                intMappingVal |=  0x8;
            }

            break;
        }
        case Algo_0032:
        {
            if((((block.phyBlockIdx >= 0x200) && (intCalculatedMappingVal >= 0x200)) || ((block.phyBlockIdx < 0x200) && (intCalculatedMappingVal < 0x200)))
               && ((intCalculatedMappingVal - block.phyBlockIdx) >= 8) && ((intCalculatedMappingVal - block.phyBlockIdx) < 16))
            {
                 intMappingVal |=  0x8;
            }else if(((block.phyBlockIdx - 0x400 + intCalculatedMappingVal) >= 8) && ((block.phyBlockIdx - 0x400 + intCalculatedMappingVal) < 16))
            {
                 intMappingVal |=  0x8;
            }

            break;
        }
        case Algo_0064:
        {
            if((((block.phyBlockIdx >= 0x200) && (intCalculatedMappingVal >= 0x200)) || ((block.phyBlockIdx < 0x200) && (intCalculatedMappingVal < 0x200)))
               && ((intCalculatedMappingVal - block.phyBlockIdx) >= 16) && ((intCalculatedMappingVal - block.phyBlockIdx) < 32))
            {
                 intMappingVal |=  0x8;
            }else if(((block.phyBlockIdx - 0x400 + intCalculatedMappingVal) >= 16) && ((block.phyBlockIdx - 0x400 + intCalculatedMappingVal) < 32))
            {
                 intMappingVal |=  0x8;
            }

            break;
        }
        case Algo_0128:
        {
            if((((block.phyBlockIdx >= 0x200) && (intCalculatedMappingVal >= 0x200)) || ((block.phyBlockIdx < 0x200) && (intCalculatedMappingVal < 0x200)))
               && ((intCalculatedMappingVal - block.phyBlockIdx) >= 32) && ((intCalculatedMappingVal - block.phyBlockIdx) < 64))
            {
                 intMappingVal |=  0x8;
            }else if(((block.phyBlockIdx - 0x400 + intCalculatedMappingVal) >= 32) && ((block.phyBlockIdx - 0x400 + intCalculatedMappingVal) < 64))
            {
                 intMappingVal |=  0x8;
            }

            break;
        }
        case Algo_0256:
        {
            if((((block.phyBlockIdx >= 0x200) && (intCalculatedMappingVal >= 0x200)) || ((block.phyBlockIdx < 0x200) && (intCalculatedMappingVal < 0x200)))
               && ((intCalculatedMappingVal - block.phyBlockIdx) >= 64) && ((intCalculatedMappingVal - block.phyBlockIdx) < 128))
            {
                 intMappingVal |=  0x8;
            }else if(((block.phyBlockIdx - 0x400 + intCalculatedMappingVal) >= 64) && ((block.phyBlockIdx - 0x400 + intCalculatedMappingVal) < 128))
            {
                 intMappingVal |=  0x8;
            }

            break;
        }
        case Algo_0512:
        {
            if((((block.phyBlockIdx >= 0x200) && (intCalculatedMappingVal >= 0x200)) || ((block.phyBlockIdx < 0x200) && (intCalculatedMappingVal < 0x200)))
               && ((intCalculatedMappingVal - block.phyBlockIdx) >= 128) && ((intCalculatedMappingVal - block.phyBlockIdx) < 256))
            {
                 intMappingVal |=  0x8;
            }else if(((block.phyBlockIdx - 0x400 + intCalculatedMappingVal) >= 128) && ((block.phyBlockIdx - 0x400 + intCalculatedMappingVal) < 256))
            {
                 intMappingVal |=  0x8;
            }

            break;
        }
        case Algo_1023:
        {
            if((((block.phyBlockIdx >= 0x200) && (intCalculatedMappingVal >= 0x200)) || ((block.phyBlockIdx < 0x200) && (intCalculatedMappingVal < 0x200)))
               && ((intCalculatedMappingVal - block.phyBlockIdx) >= 256) && ((intCalculatedMappingVal - block.phyBlockIdx) < 512))
            {
                 intMappingVal |=  0x8;
            }else if(((block.phyBlockIdx - 0x400 + intCalculatedMappingVal) >= 256) && ((block.phyBlockIdx - 0x400 + intCalculatedMappingVal) < 512))
            {
                 intMappingVal |=  0x8;
            }

            break;
        }
        default:
        {
            break;
        }
        }

        if(nandBlockInfo.at(intMappingVal) == -1)
        {
            nandBlockInfo.replace(intMappingVal, block.phyBlockIdx);
        }else{
            intMappingVal |= 0x8;
            if(nandBlockInfo.at(intMappingVal) == -1)
            {
                nandBlockInfo.replace(intMappingVal, block.phyBlockIdx);
            }else{
                unmappedBlocks.append(block.phyBlockIdx);
            }
        }

        QCoreApplication::processEvents();
    }

    //map bad blocks here
    if(badBlocks.count() > 0)
    {
        for(int counter = 0; counter < NandBlockCount; counter++)
        {
            if(badBlocks.count() == 0)
            {
                break;
            }
            if(nandBlockInfo.at(counter) == -1)
            {
                nandBlockInfo.replace(counter, badBlocks.first());
                badBlocks.removeFirst();
            }

            QCoreApplication::processEvents();
        }
    }

    //map unmapped blocks here
    if(unmappedBlocks.count() > 0)
    {
        for(int counter = 0; counter < NandBlockCount; counter++)
        {
            if(unmappedBlocks.count() == 0)
            {
                break;
            }

            if((nandBlockInfo.at(counter) == -1)) //need to add additional check from .NET version
            {
                nandBlockInfo.replace(counter, unmappedBlocks.first());
                unmappedBlocks.removeFirst();
            }

            QCoreApplication::processEvents();
        }
    }

    //memcpy(sortedBlocks, nandBlockInfo, sizeof(nandBlockInfo[0]) * NandBlockCount);
    memcpy(sortedBlocks->data(), nandBlockInfo.data(), sizeof(nandBlockInfo.at(0)) * nandBlockInfo.length());

    return true;
}

bool PS3NAND::InterleaveNandFlashes(QString flash0, QString flash1, QString flash, QVector<int>* flash0Blocks, QVector<int>* flash1Blocks)
{
    int progress = 0;

    QByteArray NAND0Block(NandrawBlockSize, 0);
    QByteArray NAND0Page(NandPageRawSize,0);
    QByteArray NAND0PageData(NandPageDataSize,0);
    QByteArray NAND0PageSpareData(NandPageSpareDataSize,0);
    QByteArray NAND0SectorData(NandSectorSize,0);
    QByteArray NAND1Block(NandrawBlockSize, 0);
    QByteArray NAND1Page(NandPageRawSize,0);
    QByteArray NAND1PageData(NandPageDataSize,0);
    QByteArray NAND1PageSpareData(NandPageSpareDataSize,0);
    QByteArray NAND1SectorData(NandSectorSize,0);

    QFile nand0File(flash0);
    QFile nand1File(flash1);
    QFile nandFile(flash);

    Misc misc;

    try
    {
        if(nandFile.exists())
        {
            nandFile.remove();
        }

        if(!nand0File.open(QIODevice::ReadOnly) || !nand1File.open(QIODevice::ReadOnly))
        {
            return false;
        }

        if(!nandFile.open(QIODevice::WriteOnly))
        {
            return false;
        }

        for(int blockCounter = 0; blockCounter < NandBlockCount; blockCounter++)
        {
            progress++;

            progressChanged("InterleaveNANDFlashes()", QString("Interleaving block no: %1.").arg(progress - 1), progress);

            nand1File.seek((flash1Blocks->at(blockCounter) * NandrawBlockSize));
            NAND1Block = nand1File.read(NandrawBlockSize);

            nand0File.seek((flash0Blocks->at(blockCounter) * NandrawBlockSize));
            NAND0Block = nand0File.read(NandrawBlockSize);

            for(int pageCount = 0; pageCount < NandPagePerBlock; pageCount++)
            {
                memcpy(NAND1Page.data(), &NAND1Block.data()[pageCount * NandPageRawSize], NandPageRawSize);
                memcpy(NAND1PageData.data(), NAND1Page.data(), NandPageDataSize);
                memcpy(NAND1PageSpareData.data(), &NAND1Page.data()[NandPageDataSize], NandPageSpareDataSize);

                memcpy(NAND0Page.data(), &NAND0Block.data()[pageCount * NandPageRawSize], NandPageRawSize);
                memcpy(NAND0PageData.data(), NAND0Page.data(), NandPageDataSize);
                memcpy(NAND0PageSpareData.data(), &NAND0Page.data()[NandPageDataSize], NandPageSpareDataSize);

                for(int sectorCount = 0; sectorCount < NandSectorPerPage; sectorCount++)
                {
                    char temp0[NandSectorSize];
                    char temp1[NandSectorSize];

                    //NandSectorPerPage
                    memcpy(temp1, &NAND1PageData.data()[sectorCount * NandSectorSize], NandSectorSize);
                    memcpy(temp0, &NAND0PageData.data()[sectorCount * NandSectorSize], NandSectorSize);

                    //swap byte array here
                    memcpy(NAND1SectorData.data(), misc.ByteSwap(temp1, NandSectorSize), NandSectorSize);
                    memcpy(NAND0SectorData.data(), misc.ByteSwap(temp0, NandSectorSize), NandSectorSize);

                    nandFile.write(NAND1SectorData.data(), NandSectorSize);
                    nandFile.write(NAND0SectorData.data(), NandSectorSize);
                }
            }

            QCoreApplication::processEvents();
        }

        progressChanged("InterleaveNANDFlashes()", QString("Interleaving process finished successfully!"), progress);

        nandFile.flush();
        nandFile.close();
        nand0File.close();
        nand1File.close();
    } catch (...) {
        return false;
    }

    return true;
}

bool PS3NAND::DeinterleaveNandFlashes(QString flash0, QString flash1, QString interleavedFlash, \
                             QVector<int>* flash0Blocks, QVector<int>* flash1Blocks, \
                             QVector<int>* flash0BadBlocks, QVector<int>* flash1BadBlocks, \
                             int* flash0ModifiedBlocksCount, int* flash1ModifiedBlocksCount, bool diffFile)
{
     int progress = 0;
     long nand0PhyBlockOffset = 0;
     long nand1PhyBlockOffset = 0;

     QList<int> NAND0ModifiedBlocksInfo;
     QList<int> NAND1ModifiedBlocksInfo;

     QFileInfo nand0FlashFile(flash0);
     QFileInfo nand1FlashFile(flash1);
     QFileInfo nandFlashFile(interleavedFlash);

     QFile nand0File(flash0);
     QFile nand1File(flash1);
     QFile nandFile(interleavedFlash);
     QFile modifiedNand0(QString("%1\\%2-patched.bin").arg(nand0FlashFile.path(), nand0FlashFile.baseName()));
     QFile modifiedNand1(QString("%1\\%2-patched.bin").arg(nand1FlashFile.path(), nand1FlashFile.baseName()));

     QByteArray NAND0ModifiedRAWBlock(NandrawBlockSize, 0);
     QByteArray NAND1ModifiedRAWBlock(NandrawBlockSize, 0);

     Misc misc;

     progressChanged("DeinterleaveNANDFlashes()", "Deinterleaving process started.", progress);

     try
     {
         if(modifiedNand0.exists())
         {
             modifiedNand0.remove();
         }

         if(modifiedNand1.exists())
         {
             modifiedNand1.remove();
         }

         if(!nand0File.open(QIODevice::ReadOnly) || !nand1File.open(QIODevice::ReadOnly) || !nandFile.open(QIODevice::ReadOnly))
         {
             return false;
         }

         if(!modifiedNand0.open(QIODevice::WriteOnly) || !modifiedNand1.open(QIODevice::WriteOnly))
         {
             return false;
         }


         for(int blockCounter = 0; blockCounter < NandBlockCount; blockCounter++)
         {
            progress++;

            progressChanged("DeinterleaveNANDFlashes()", QString("Deinterleaving block no: %1.").arg(progress - 1), progress);

            QByteArray nand1Buffer((NandPageDataSize * NandPageSpareDataSize * 2), 0);
            QByteArray nand0Buffer((NandPageDataSize * NandPageSpareDataSize * 2), 0);

            nand1PhyBlockOffset = (((flash1Blocks->indexOf(blockCounter) * NandPageDataSize) * NandPageSpareDataSize) * 2);
            nand0PhyBlockOffset = (((flash0Blocks->indexOf(blockCounter) * NandPageDataSize) * NandPageSpareDataSize) * 2);

            nandFile.seek(nand1PhyBlockOffset);

            QByteArray temp1 = nandFile.read(NandPageDataSize * NandPageSpareDataSize * 2);

            memcpy(nand1Buffer.data(), misc.ByteSwap(temp1.data(), NandPageDataSize * NandPageSpareDataSize * 2), (NandPageDataSize * NandPageSpareDataSize * 2));

            if(nand1PhyBlockOffset != nand0PhyBlockOffset)
            {
                nandFile.seek(nand0PhyBlockOffset);

                QByteArray temp0 = nandFile.read(NandPageDataSize * NandPageSpareDataSize * 2);

                memcpy(nand0Buffer.data(), misc.ByteSwap(temp0.data(), NandPageDataSize * NandPageSpareDataSize * 2), (NandPageDataSize * NandPageSpareDataSize * 2));
            }else{
                memcpy(nand0Buffer.data(), nand1Buffer.data(), (NandPageDataSize * NandPageSpareDataSize * 2));
            }

            for(int pageCounter = 0; pageCounter < NandPagePerBlock; pageCounter++)
            {
                QByteArray rawBlockBuffer(NandPageSpareDataSize, 0);

                long pageOffset = ((((blockCounter * NandPageRawSize) * NandPageSpareDataSize) + (NandPageRawSize * pageCounter)) + NandPageDataSize);

                nand1File.seek(pageOffset);
                rawBlockBuffer = nand1File.read(rawBlockBuffer.length());
                memcpy(&NAND1ModifiedRAWBlock.data()[(pageCounter * NandPageRawSize) + NandPageDataSize], rawBlockBuffer, rawBlockBuffer.length());

                nand0File.seek(pageOffset);
                rawBlockBuffer = nand0File.read(rawBlockBuffer.length());
                memcpy(&NAND0ModifiedRAWBlock.data()[(pageCounter    * NandPageRawSize) + NandPageDataSize], rawBlockBuffer, rawBlockBuffer.length());

                int offset = 0;

                for(int sectorCounter = 0; sectorCounter < NandSectorPerPage; sectorCounter++)
                {
                    QByteArray sectorBuffer(NandSectorSize, 0);

                    long sectorOffset = ((((blockCounter * NandPageRawSize) * NandPageSpareDataSize) + (pageCounter * NandPageRawSize)) + (sectorCounter * NandSectorSize));

                    nand1File.seek(sectorOffset);
                    sectorBuffer = nand1File.read(NandSectorSize);
                    memcpy(&NAND1ModifiedRAWBlock.data()[((pageCounter * NandPageRawSize) + (sectorCounter * NandSectorSize))], \
                           &nand1Buffer.data()[(((pageCounter * NandPageDataSize) * 2) + (offset * NandSectorSize))] , NandSectorSize);

                    if((flash1BadBlocks->at(blockCounter) == -1) && memcmp(&NAND1ModifiedRAWBlock.data()[(pageCounter * NandPageRawSize) + (sectorCounter * NandSectorSize)], \
                            sectorBuffer.data(), NandSectorSize))
                    {

                        QByteArray ecc(10, 0);
                        CalculateECC(&NAND1ModifiedRAWBlock, ((pageCounter * NandPageRawSize) + (sectorCounter * NandSectorSize)), &ecc);
                        memcpy(&NAND1ModifiedRAWBlock.data()[((((pageCounter * NandPageRawSize) + NandPageDataSize) + 8) + (sectorCounter * 14))], \
                               ecc.data(), 10);

                        int modifiedNand1Block = ((blockCounter * NandPageRawSize) * NandPageSpareDataSize);
                        if(!NAND1ModifiedBlocksInfo.contains(modifiedNand1Block))
                        {
                            NAND1ModifiedBlocksInfo.append(modifiedNand1Block);
                        }
                    }

                    offset++;

                    nand0File.seek(sectorOffset);
                    sectorBuffer = nand0File.read(NandSectorSize);
                    memcpy(&NAND0ModifiedRAWBlock.data()[((pageCounter * NandPageRawSize) + (sectorCounter * NandSectorSize))], \
                           &nand0Buffer.data()[(((pageCounter * NandPageDataSize) * 2) + (offset * NandSectorSize))] , NandSectorSize);

                    if((flash0BadBlocks->at(blockCounter) == -1) && memcmp(&NAND0ModifiedRAWBlock.data()[(pageCounter * NandPageRawSize) + (sectorCounter * NandSectorSize)], \
                            sectorBuffer.data(), NandSectorSize))
                    {
                        QByteArray ecc(10, 0);
                        CalculateECC(&NAND0ModifiedRAWBlock, ((pageCounter * NandPageRawSize) + (sectorCounter * NandSectorSize)), &ecc);
                        memcpy(&NAND0ModifiedRAWBlock.data()[((((pageCounter * NandPageRawSize) + NandPageDataSize) + 8) + (sectorCounter * 14))], \
                               ecc.data(), 10);

                        int modifiedNand0Block = ((blockCounter * NandPageRawSize) * NandPageSpareDataSize);
                        if(!NAND0ModifiedBlocksInfo.contains(modifiedNand0Block))
                        {
                            NAND0ModifiedBlocksInfo.append(modifiedNand0Block);
                        }
                    }

                    offset++;
                }
            }

            modifiedNand1.write(NAND1ModifiedRAWBlock);
            modifiedNand0.write(NAND0ModifiedRAWBlock);

            QCoreApplication::processEvents();
         }

         modifiedNand1.flush();
         modifiedNand0.flush();
         modifiedNand1.close();
         modifiedNand0.close();
         nandFile.close();
         nand1File.close();
         nand0File.close();

         progressChanged("DeinterleaveNANDFlashes()", QString("Deinterleaving process finished successfully!"), progress);

         *flash1ModifiedBlocksCount = NAND1ModifiedBlocksInfo.count();
         *flash0ModifiedBlocksCount = NAND0ModifiedBlocksInfo.count();

         if(diffFile && ((NAND1ModifiedBlocksInfo.count() > 0) || (NAND0ModifiedBlocksInfo.count())))
         {
             QString diffFilePath = QString("%1\\Differential Flashing").arg(nandFlashFile.path());
             QString modifiedNand0FileLogPath = QString("%1\\DifferenceFileFlash0.txt").arg(diffFilePath);
             QString modifiedNand1FileLogPath = QString("%1\\DifferenceFileFlash1.txt").arg(diffFilePath);

             QDir diffFileDir(diffFilePath);
             QFile modifiedNand0FileLog(modifiedNand0FileLogPath);
             QFile modifiedNand1FileLog(modifiedNand1FileLogPath);

             diffFileDir.removeRecursively();
             diffFileDir.mkdir(diffFilePath);

             if(!modifiedNand0FileLog.open(QIODevice::Text | QIODevice::WriteOnly) || !modifiedNand1FileLog.open(QIODevice::Text | QIODevice::WriteOnly))
             {
                 return true;
             }

             QTextStream modifiedNand0Log(&modifiedNand0FileLog);
             QTextStream modifiedNand1Log(&modifiedNand1FileLog);

             foreach (int block, NAND0ModifiedBlocksInfo)
             {
                 modifiedNand0Log << QString("0x") << QString("%1\n").arg(block, 8, 16, QChar('0')).toUpper();
             }

             modifiedNand0Log.flush();
             modifiedNand0FileLog.flush();
             modifiedNand0FileLog.close();

             foreach (int block, NAND1ModifiedBlocksInfo)
             {
                 modifiedNand1Log << QString("0x") << QString("%1\n").arg(block, 8, 16, QChar('0')).toUpper();
             }

             modifiedNand1Log.flush();
             modifiedNand1FileLog.flush();
             modifiedNand1FileLog.close();
         }

     } catch (...) {
         return false;
     }

    return true;
}

bool PS3NAND::PatchInterleavedNANDFlash(QString flashFile, int patchIndex)
{
    QFileInfo originalFileInfo(flashFile);
    QFile originalFile(flashFile);
    QString patchedFilePath = QString("%1\\%2-patched.bin").arg(originalFileInfo.path(), originalFileInfo.baseName());

    QByteArray coreOSPatch;
    QByteArray trvkPatch;
    QFile coreOSPatchFile;
    QFile trvkPatchFile;

    progressChanged("PatchInterleavedNANDFlash()", "Patch interleaved nand flash process started.", 1);

    try
    {

        progressChanged("PatchInterleavedNANDFlash()", "Copying source file to destination file.", 2);

        if(!originalFile.exists())
        {   
            return false;
        }

        QFile patchedFile(patchedFilePath);

        if(patchedFile.exists())
        {
            patchedFile.remove();
        }

        if(!QFile::copy(flashFile, patchedFilePath))
        {
            return false;
        }

        if(!patchedFile.open(QIODevice::ReadWrite))
        {
            return false;
        }

        if(patchIndex == 0)
        {
            coreOSPatchFile.setFileName(":/patch/470/res/Patches/CoreOS-4.70-Darkjiros-CFW-noFSM.bin");
            trvkPatchFile.setFileName(":/patch/355/res/Patches/NAND-RVKList-3.55.bin");
        }else{
            coreOSPatchFile.setFileName(":/patch/355/res/Patches/CoreOS-3.55.bin");
            trvkPatchFile.setFileName(":/patch/355/res/Patches/NAND-RVKList-3.55.bin");
        }

        if(!coreOSPatchFile.open(QIODevice::ReadOnly) || !trvkPatchFile.open(QIODevice::ReadOnly))
        {
            return false;
        }

        coreOSPatch = coreOSPatchFile.readAll();
        trvkPatch = trvkPatchFile.readAll();

        coreOSPatchFile.close();
        trvkPatchFile.close();

        progressChanged("PatchInterleavedNANDFlash()", "Patching coreos_0 area.", 2);

        //patching coreos_0 area
        patchedFile.seek(CoreOS0Offset);
        patchedFile.write(coreOSPatch.data(), coreOSPatch.length());

        progressChanged("PatchInterleavedNANDFlash()", "Patching coreos_1 area.", 3);

        //patching coreos_1 area
        patchedFile.seek(CoreOS1Offset);
        patchedFile.write(coreOSPatch.data(), coreOSPatch.length());

        progressChanged("PatchInterleavedNANDFlash()", "Patching trvk area.", 4);

        //patching trvk area
        patchedFile.seek(TRVKOffset);
        patchedFile.write(trvkPatch.data(), trvkPatch.length());

        patchedFile.flush();
        patchedFile.close();

        progressChanged("PatchInterleavedNANDFlash()", "Patching interleaved nand flash process done successfully!.", 5);

    } catch (...) {
        return false;
    }

    return true;

}

bool PS3NAND::ExtractInterleavedNANDFlash(QString flashFile, bool saveFiles)
{
    QFile nandFlash(flashFile);
    QFileInfo nandFlashInfo(flashFile);
    QString extractDataPath = QString("%1%2%3_data%2").arg(nandFlashInfo.path(), QDir::separator(), nandFlashInfo.baseName());
    QDir extractDataDir(extractDataPath);
    QByteArray data;
    int dataFound = 0;

    progressChanged("ExtractInterleavedNANDFlash()", "Extract interleaved nand flash process started.", dataFound);

    try
    {
        if(!nandFlash.exists() || !nandFlash.open(QIODevice::ReadOnly))
        {
            return false;
        }

        if(saveFiles)
        {
            if(extractDataDir.exists())
            {
                extractDataDir.removeRecursively();
            }

            extractDataDir.mkdir(extractDataPath);
        }

        for(int counter = 0; counter < 9; counter++)
        {
            PS3NandPerConsoleData nandFlashData = consoleData[counter];

            nandFlash.seek(nandFlashData.offset);
            data = nandFlash.read(nandFlashData.size);

            if(saveFiles)
            {
                QFile consoleDataFile(QString("%1%2.bin").arg(extractDataPath, nandFlashData.name));
                if(!consoleDataFile.open(QIODevice::ReadWrite))
                {
                    continue;
                }

                consoleDataFile.write(data.data(), data.length());
                consoleDataFile.flush();
                consoleDataFile.close();

                dataFound++;

                progressChanged("ExtractInterleavedNANDFlash()", tr("[%1] %2 found at offset: 0x%3").arg(dataFound).arg(nandFlashData.name).arg(QString("%1").arg(nandFlashData.offset, 8, 16, QChar('0')).toUpper()) , dataFound);

            }

        }

    } catch (...) {
        return false;
    }

    return true;
}
