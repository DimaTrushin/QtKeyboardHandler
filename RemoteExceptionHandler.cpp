#include "RemoteExceptionHandler.h"
#include "KeyboardHandler.h"

CExceptionConnector::CExceptionConnector(CKeyboardHandler* KeyboardHandler) {
  connect(this, &CExceptionConnector::KeyboardException,
          KeyboardHandler, &CKeyboardHandler::onKeyboardException,
          Qt::ConnectionType::QueuedConnection);
}

CExceptionConnector::~CExceptionConnector() {
  disconnect(this, &CExceptionConnector::KeyboardException, nullptr, nullptr);
}


CRemoteExceptionHandler::CRemoteExceptionHandler(CKeyboardHandler* KeyboardHandler)
  : CExceptionConnector(KeyboardHandler) {
  try {
    throw;
  } catch (std::exception& Exception) {
    emit KeyboardException(Exception.what());
  } catch (...) {
    emit KeyboardException(kUnknownExceptionMsg_);
  }
}


