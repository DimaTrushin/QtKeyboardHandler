#ifndef CREMOTEEXCEPTIONHANDLER_H
#define CREMOTEEXCEPTIONHANDLER_H

#include "KeyboardExceptionMessage.h"

class CKeyboardHandler;


class CExceptionConnector : public QObject {
  Q_OBJECT
public:
  CExceptionConnector(CKeyboardHandler*);
  ~CExceptionConnector();

signals:
  void KeyboardException(const CKeyboardExceptionMessage);
};


class CRemoteExceptionHandler : public CExceptionConnector {
  Q_OBJECT
public:
  CRemoteExceptionHandler(CKeyboardHandler*);

private:
  static constexpr const char* kUnknownExceptionMsg_ = "Unknown Exception!";
};

#endif // CREMOTEEXCEPTIONHANDLER_H
