#ifndef UTIL_H
#define UTIL_H

#include <QObject>

class Util : public QObject
{
  Q_OBJECT
public:
  explicit Util(QObject *parent = nullptr);

  Q_INVOKABLE bool isBeatSaberModded( QString path );
  Q_INVOKABLE bool isWinePrefixValue( QString prefix );

signals:

public slots:
};

#endif // UTIL_H
