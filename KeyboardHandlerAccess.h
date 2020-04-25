#ifndef KEYBOARDHANDLERACCESS_H
#define KEYBOARDHANDLERACCESS_H

#include "AnyGlobalAccess.h"
#include "KeyboardHandler.h"

namespace NSDetail {

class CMainKeyboardHandler {};

class CKeyboardHandlerAccessible
  : public CAnyGlobalAccessible<CKeyboardHandler, CMainKeyboardHandler> {
};
} // NSDetail

class CKeyboardHandlerAccess
  : public CAnyGlobalAccess<CKeyboardHandler, NSDetail::CMainKeyboardHandler> {
};

class CKeyboardHandlerInitializer
  : public CAnyGlobalInitializer<CKeyboardHandler, NSDetail::CMainKeyboardHandler> {
  using CBase = CAnyGlobalInitializer<CKeyboardHandler, NSDetail::CMainKeyboardHandler>;
public:
  using CBase::CBase;
};

#endif // KEYBOARDHANDLERACCESS_H
