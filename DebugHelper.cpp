#include "DebugHelper.h"
#include <sstream>

void debugNotify(const char* message) {
  std::stringstream os;
  os << std::this_thread::get_id();
  qDebug() << message << " thread::id() = " << os.str().c_str();
}
