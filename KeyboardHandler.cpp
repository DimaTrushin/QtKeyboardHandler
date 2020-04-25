#include "KeyboardHandler.h"
#include "RemoteExceptionHandler.h"
#include "DebugHelper.h"


#ifdef Q_OS_WIN
#include "KeyboardListenerWin.h"
using CKeyboardListener = CKeyboardListenerWin;
#endif // Q_OS_WIN

#ifdef Q_OS_MACOS
#include "KeyboardListenerMac.h"
using CKeyboardListener = CKeyboardListenerMac;
#endif // Q_OS_MACOS

#ifdef Q_OS_LINUX
#include "KeyboardListenerLinux.h"
using CKeyboardListener = CKeyboardListenerLinux;
#endif // Q_OS_LINUX


CQtMessagesRegistrator::CQtMessagesRegistrator() {
  // This registrations are required to use the messages in QueuedConnections
  // across different threads
  qRegisterMetaType<CKeyboardMessage>();
  qRegisterMetaType<CKeyboardExceptionMessage>();
}


CKeyboardHandler::CKeyboardHandler() {
  debugNotify("ctor");

  CAnyKillerPromise killerPromise;
  CAnyKillerFuture killerFuture = killerPromise.get_future();
  ListenerThread_ = CWorkerThread(run, std::move(killerPromise), this);
  // Нужен таймаут на ожидание и бросание исключения, если таймаут наступил
  KeyboardKiller_ = killerFuture.get();
}

CKeyboardHandler::~CKeyboardHandler() {
  debugNotify("dtor");
  stopListener();
}

void CKeyboardHandler::activate() {
  isActive_ = true;
}

void CKeyboardHandler::deactivate() {
  isActive_ = false;
}

void CKeyboardHandler::onKeyboardMessage(const CKeyboardMessage& message) {
  qDebug() << "slotOnKeyboardMessage";
  if (isActive_)
    qDebug() << "KeyboardMessage() = " << message.Text.c_str();
}

void CKeyboardHandler::onKeyboardException(const CKeyboardExceptionMessage& message) {
  qDebug() << "slotOnKeyboardException";
  qDebug() << "ExceptionMessage() = " << message.Text.c_str();
}

void CKeyboardHandler::stopListener() const noexcept {
  assert(KeyboardKiller_.isDefined());
  debugNotify("stopListener()");
  KeyboardKiller_->stopListener();
}

void CKeyboardHandler::run(CAnyKillerPromise killerPromise,
                           CKeyboardHandler* KeyboardHandler) {
  debugNotify("run");
  try {
    CKeyboardListener KeyboardListener(std::move(killerPromise), KeyboardHandler);
    KeyboardListener.KeyboardMessage("Listener in action!");
    qDebug() << "Kb.exec()";
    KeyboardListener.exec();
  } catch (...) {
    CRemoteExceptionHandler React(KeyboardHandler);
  }
}

