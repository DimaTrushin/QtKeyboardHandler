#ifndef CKEYBOARDHANDLER_H
#define CKEYBOARDHANDLER_H


#include "KeyboardExceptionMessage.h"
#include "KeyboardMessage.h"
#include "AnyKeyboardKiller.h"
#include "ThreadDetachable.h"


// A system dependent keyboard listener lives in an independent thread
// all exceptions of the listener are sent to corresponding slot
// The listener dies on any exception or error

class CQtMessagesRegistrator : public QObject {
  Q_OBJECT
public:
  CQtMessagesRegistrator();
protected:
  ~CQtMessagesRegistrator() = default;
};


class CKeyboardHandler : public CQtMessagesRegistrator {
  Q_OBJECT

  using CAnyKillerPromise = std::promise<CAnyKeyboardKiller>;
  using CAnyKillerFuture = std::future<CAnyKeyboardKiller>;
  using CWorkerThread = CThreadDetachable;
public:
  CKeyboardHandler();
  ~CKeyboardHandler();

  void activate();
  void deactivate();

public slots:
  void onKeyboardMessage(const CKeyboardMessage&);
  void onKeyboardException(const CKeyboardExceptionMessage&);

private:
  void stopListener() const noexcept;
  static void run(CAnyKillerPromise, CKeyboardHandler*);

  CWorkerThread ListenerThread_;
  CAnyKeyboardKiller KeyboardKiller_;
  bool isActive_ = true;
};


#endif // CKEYBOARDHANDLER_H
