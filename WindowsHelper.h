#ifndef WINDOWSHELPER_H
#define WINDOWSHELPER_H

#include <Windows.h>


class CWindowClassExRegister {
public:
  CWindowClassExRegister(LPCWSTR, WNDPROC);
  ~CWindowClassExRegister();
protected:
  LPCWSTR className();
  HINSTANCE& hInstance();
private:
  static WNDCLASSEX getWndClass(LPCWSTR, WNDPROC);

  static constexpr const char* kRegisterErrorMessage_ = "Cannot register Window Class";
  // Это по сути статическая информация для данного класса.
  // Так как она не статическая сейчас, то я не могу создать более одного объекта
  // данного класса, что требует сделать его singleton.
  // Проблема еще в том, что эта информация зависит от статической информации
  // 1) Имя класса, 2) windows procedure
  WNDCLASSEX WindowClass_;
};


class CMessageWindowCreator : public CWindowClassExRegister {
public:
  CMessageWindowCreator(LPCWSTR, WNDPROC);
  ~CMessageWindowCreator();
protected:
  HWND& hwnd();
private:
  static constexpr const char* kCreationErrorMessage_ = "Cannot create Window Class";
  HWND MessageWindow_;
};

#endif // WINDOWSHELPER_H
