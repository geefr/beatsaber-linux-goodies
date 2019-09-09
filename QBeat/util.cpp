#include "util.h"

#include <QDebug>
#include <QDir>

#include <memory>

#include "minizip/unzip.h"

bool Util::extractArchive( QString archivePath, QString destDirectory )
{
  auto zFile = unzOpen(archivePath.toUtf8());
  if( !zFile )
  {
    qDebug() << "ERROR: Failed to open zip file: " << archivePath;
    return false;
  }

  unz_global_info info;
  if( unzGetGlobalInfo( zFile, &info ) != UNZ_OK )
  {
    qDebug() << "ERROR: Failed to get zip file info: " << archivePath;
    unzClose(zFile);
    return false;
  }

  const auto readBufSize = 32 * 1024 * 1024;
  std::unique_ptr<char[]> readBuf( new char[readBufSize] );

  // Loop over files in archive and extract
  for( auto i = 0u; i < info.number_entry; ++i )
  {
    unz_file_info fileInfo;
    const auto fileNameLen = 256;
    char fileName[fileNameLen];
    if( unzGetCurrentFileInfo(
          zFile,
          &fileInfo,
          fileName,
          fileNameLen,
          nullptr, 0, nullptr, 0 ) != UNZ_OK )
    {
      qDebug() << "ERROR: Failed to get file info from archive: " << i;
      unzClose(zFile);
      return false;
    }

    QString fileNameStr( fileName );
    if( fileNameStr.size() == 0 ) continue;

    if( fileNameStr[fileNameLen-1] == '/' )
    {
      // Directory
      QDir dir(fileNameStr);
      if (!dir.exists()){
        dir.mkpath(".");
      }
    }
    else {
      if( unzOpenCurrentFile( zFile ) != UNZ_OK ) {
        qDebug() << "ERROR: Failed to open file in archive: " << fileNameStr;
        unzClose(zFile);
        return false;
      }

      auto outFileName = destDirectory + "/" + fileNameStr;
      QFileInfo outFileInfo(outFileName);
      outFileInfo.absoluteDir().mkpath(".");
      if( !outFileInfo.isDir() ) {
        QFile outFile(QString(destDirectory) + "/" + fileNameStr);
        outFile.open(QFile::OpenModeFlag::ReadWrite);
        if( !outFile.isOpen() ) {
          qDebug() << "ERROR: Failed to open file: " << outFileName;
          unzClose(zFile);
          return false;
        }

        auto error = UNZ_OK;
        do {
          error = unzReadCurrentFile( zFile, readBuf.get(), readBufSize );
          if( error < 0 ) {
            qDebug() << "ERROR: Decompress failed for file: " << outFileName;
            unzCloseCurrentFile(zFile);
            unzClose(zFile);
            return false;
          }

          if( error > 0 ) {
            if( outFile.write(readBuf.get(), error) != error ) {
              qDebug() << "ERROR: Decompress failed for file: " << outFileName;
              unzCloseCurrentFile(zFile);
              unzClose(zFile);
              return false;
            }
          }
        } while (error > 0); // OK or EOF
      }

      unzCloseCurrentFile(zFile);

      if( (i + 1) < info.number_entry ) {
        if( unzGoToNextFile(zFile) != UNZ_OK ) {
          qDebug() << "ERROR: Failed to read next file in archive: " << fileNameStr;
          unzClose(zFile);
          return false;
        }
      }
    }
  }

  unzClose(zFile);

  return true;
}


