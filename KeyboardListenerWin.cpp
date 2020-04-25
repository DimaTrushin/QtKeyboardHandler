#include "KeyboardListenerWin.h"
#include "KeyboardHandler.h"
//#include "DebugHelper.h"

// Это пока что пометка для меня, чтобы я не забыл что и как надо делать.
/*
  wcx.cbSize = sizeof(wcx);          // size of structure
  wcx.style = CS_HREDRAW |
      CS_VREDRAW;                    // redraw if size changes
  wcx.lpfnWndProc = MainWndProc;     // points to window procedure
  wcx.cbClsExtra = 0;                // no extra class memory
  wcx.cbWndExtra = 0;                // no extra window memory
  wcx.hInstance = hinstance;         // handle to instance
  wcx.hIcon = LoadIcon(NULL,
      IDI_APPLICATION);              // predefined app. icon
  wcx.hCursor = LoadCursor(NULL,
      IDC_ARROW);                    // predefined arrow
  wcx.hbrBackground = GetStockObject(
      WHITE_BRUSH);                  // white background brush
  wcx.lpszMenuName =  "MainMenu";    // name of menu resource
  wcx.lpszClassName = "MainWClass";  // name of window class
  wcx.hIconSm = LoadImage(hinstance, // small class icon
      MAKEINTRESOURCE(5),
      IMAGE_ICON,
      GetSystemMetrics(SM_CXSMICON),
      GetSystemMetrics(SM_CYSMICON),
      LR_DEFAULTCOLOR);
*/


CRawInputHook::CRawInputHook(LPCWSTR WindowClassName, WNDPROC WndProc)
  : CMessageWindowCreator(WindowClassName, WndProc) {
  RAWINPUTDEVICE LowLevelKeyboard;
  LowLevelKeyboard.usUsagePage = 0x01;
  LowLevelKeyboard.usUsage = 0x06;
  LowLevelKeyboard.dwFlags = RIDEV_INPUTSINK;
  LowLevelKeyboard.hwndTarget = hwnd();
  if (RegisterRawInputDevices(&LowLevelKeyboard,
                              1,
                              sizeof(LowLevelKeyboard))
      == FALSE)
    throw std::runtime_error("Cannot register RAWINPUT");
}

CRawInputHook::~CRawInputHook() {
  RAWINPUTDEVICE LowLevelKeyboard;
  LowLevelKeyboard.usUsagePage = 0x01;
  LowLevelKeyboard.usUsage = 0x06;
  LowLevelKeyboard.dwFlags = RIDEV_REMOVE;
  LowLevelKeyboard.hwndTarget = nullptr;
  if (RegisterRawInputDevices(&LowLevelKeyboard,
                              1,
                              sizeof(LowLevelKeyboard))
      == FALSE)
    qDebug() << "Cannot unregister RAWINPUT";
}


CKeyboardListenerWin::CKeyboardListenerWin(
  CAnyKillerPromise killerPromise,
  CKeyboardHandler* KeyboardHandler)
  : CRawInputHook(kWindowClassName_, WndProc) {
  provideHookToListener();
  killerPromise.set_value(CKiller(hwnd()));
  // разбить на два layer-а. В начале делаем connect, а потом уже
  // устанавливаем promise или наоборот. Надо подумать, как обработать
  // ошибку соединения.
  connect(this, &CKeyboardListenerWin::KeyboardMessage,
          KeyboardHandler, &CKeyboardHandler::onKeyboardMessage,
          Qt::ConnectionType::QueuedConnection);
}

CKeyboardListenerWin::~CKeyboardListenerWin() {
  disconnect(this, &CKeyboardListenerWin::KeyboardMessage, nullptr, nullptr);
}

int CKeyboardListenerWin::exec() {
  CMessageStatus Status;
  MSG CurrentMessage;
  while ((Status = getMessage(&CurrentMessage)) != Quit) {
    if (Status == Error)
      handleError(&CurrentMessage);
    else
      dispatchMessage(&CurrentMessage);
  }
  return exitCode(&CurrentMessage);
}

CKeyboardListenerWin::CMessageStatus CKeyboardListenerWin::getMessage(MSG* CurrentMessage) {
  return ::GetMessage(CurrentMessage, hwnd(), 0, 0);
}

void CKeyboardListenerWin::dispatchMessage(MSG* CurrentMessage) {
  ::TranslateMessage(CurrentMessage);
  ::DispatchMessage(CurrentMessage);
}

void CKeyboardListenerWin::handleError(MSG* CurrentMessage) {
  (void)CurrentMessage;
  DWORD ErrorCode = GetLastError();
  std::string ErrorMessage = "KeyboardListener error with code = ";
  ErrorMessage += std::to_string(ErrorCode);
  throw std::runtime_error(ErrorMessage.c_str());
}

int CKeyboardListenerWin::exitCode(MSG* CurrentMessage) const {
  return static_cast<int>(CurrentMessage->wParam);
}

void CKeyboardListenerWin::provideHookToListener() {
  ::SetWindowLongPtr(hwnd(), GWLP_USERDATA, LONG_PTR(this));
}

CKeyboardListenerWin* CKeyboardListenerWin::getKeyboardListener(HWND hwnd) {
  return reinterpret_cast<CKeyboardListenerWin*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA));
}

LRESULT CKeyboardListenerWin::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
  switch (message) {
  case WM_STOP_LISTENING:
    ::PostQuitMessage(0);
    break;
  case WM_INPUT:
    qDebug() << "WM_INPUT";
    getKeyboardListener(hwnd)->HandleRawInput(lparam);
    break;
  default:
    ;
  }
  return ::DefWindowProc(hwnd, message, wparam, lparam);
}

void CKeyboardListenerWin::HandleRawInput(LPARAM lParam) {
  (void)lParam;
  // Translate the lParam to system independent representation
  // and send it to the required object
  qDebug() << "Handle lParam";
  emit KeyboardMessage("Message");
}


CKiller::CKiller(HWND MessageWindow)
  :  MessageWindow_(MessageWindow) {
  assert(MessageWindow_ != NULL);
}

void CKiller::stopListener() const {
  assert(MessageWindow_ != NULL);
  if (MessageWindow_ != NULL)
    ::PostMessage(MessageWindow_, WM_STOP_LISTENING, 0, 0);
}
