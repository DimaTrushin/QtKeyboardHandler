#ifndef CKEYBOARDLISTENERWIN_H
#define CKEYBOARDLISTENERWIN_H

#include "AnyKeyboardKiller.h"
#include "KeyboardExceptionMessage.h"
#include "KeyboardMessage.h"

#include "WindowsHelper.h"


class CRawInputHook : public CMessageWindowCreator {
public:
  CRawInputHook(LPCWSTR, WNDPROC);
  ~CRawInputHook();
};


class CKeyboardHandler;


// Этот объект должен быть адресуемым. Потому его надо завернуть
// в умный указатель как минимум.
// Сейчас это не сильно актуально. Так как два объекта этого класса
// невозможно создать физически -- ругнется низкоуровневое api
// операционной системы. Надо менять дизайн глобально.
class CKeyboardListenerWin : public QObject, public CRawInputHook {
  Q_OBJECT

  friend class CKiller;
  using CMessageStatus = BOOL;
  using CAnyKillerPromise = std::promise<CAnyKeyboardKiller>;
public:
  CKeyboardListenerWin(CAnyKillerPromise, CKeyboardHandler*);
  ~CKeyboardListenerWin();

signals:
  void KeyboardMessage(const CKeyboardMessage);
public:
  int exec();
private:
  CMessageStatus getMessage(MSG* CurrentMessage);
  void dispatchMessage(MSG* CurrentMessage);
  void handleError(MSG* CurrentMessage);
  int exitCode(MSG* CurrentMessage) const;

  void provideHookToListener();
  static CKeyboardListenerWin* getKeyboardListener(HWND);
  static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
  void HandleRawInput(LPARAM lParam);

  static constexpr CMessageStatus Error = -1;
  static constexpr CMessageStatus Quit = 0;
  static constexpr UINT WM_STOP_LISTENING = WM_APP;
  static constexpr LPCWSTR kWindowClassName_ = L"KeyboardListenerWndClass";
};


// The object provides a way to shut down the listener
class CKiller {
public:
  CKiller() = default;
  CKiller(HWND);
  void stopListener() const;

private:
  static constexpr UINT WM_STOP_LISTENING = CKeyboardListenerWin::WM_STOP_LISTENING;
  HWND MessageWindow_;
};

#endif // CKEYBOARDLISTENERWIN_H
