#ifndef UTIL_H
#define UTIL_H

#include <QString>

class Util
{
public:
  /// Extract a zip archive to the specified directory
  static bool extractArchive( QString archivePath, QString destDirectory );

  /**
   * Fixup a path such as plugins -> Plugins
   * Should be called on all file paths to correct issues in the mod manifest/archive contents
   */
  static void fixPath( QString& path );
private:

  /**
   * A list of paths known to be wrong in some mod archives
   *
   * Not a problem on Windows as the filesystem is case insensitive
   * but on anything else we end up with files in the wrong folders
   *
   * Paths will be checked against these and updated to the correct case
   * if needed
   */
  static QString mBSPathsToFix[];
};

#endif
