#include "WindowsHelper.h"


CWindowClassExRegister::CWindowClassExRegister(
  LPCWSTR WindowClassName, WNDPROC WndProc)
  : WindowClass_(getWndClass(WindowClassName, WndProc)) {
  if (!::RegisterClassEx(&WindowClass_))
    throw std::runtime_error(kRegisterErrorMessage_);
}

CWindowClassExRegister::~CWindowClassExRegister() {
  ::UnregisterClass(className(), hInstance());
}

LPCWSTR CWindowClassExRegister::className() {
  return WindowClass_.lpszClassName;
}

HINSTANCE& CWindowClassExRegister::hInstance() {
  return WindowClass_.hInstance;
}

WNDCLASSEX CWindowClassExRegister::getWndClass(
  LPCWSTR WindowClassName, WNDPROC WndProc) {
  WNDCLASSEX wc = {};
  wc.cbSize = sizeof(wc);
  wc.hInstance = ::GetModuleHandle(nullptr);
  wc.lpfnWndProc = WndProc;
  wc.lpszClassName = WindowClassName;

  return wc;
}


CMessageWindowCreator::CMessageWindowCreator(
  LPCWSTR WindowClassName, WNDPROC WndProc)
  : CWindowClassExRegister(WindowClassName, WndProc),
    MessageWindow_(::CreateWindowEx(NULL,
                                    className(),
                                    TEXT(""),
                                    NULL,
                                    0, 0, 0, 0,
                                    HWND_MESSAGE,
                                    NULL,
                                    hInstance(),
                                    NULL)) {
  if (!MessageWindow_)
    throw std::runtime_error(kCreationErrorMessage_);
}

CMessageWindowCreator::~CMessageWindowCreator() {
  ::DestroyWindow(MessageWindow_);
}

HWND& CMessageWindowCreator::hwnd() {
  return MessageWindow_;
}
