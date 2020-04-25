#ifndef ANYKEYBOARDKILLER_H
#define ANYKEYBOARDKILLER_H

#include "AnyObject.h"

namespace NSDetail {

template<class TBase>
class IAnyKeyboardKiller : public TBase {
public:
  virtual void stopListener() const = 0;
protected:
  virtual ~IAnyKeyboardKiller() = default;
};


template<class TBase, class TObject>
class CAnyKeyboardKillerImpl : public TBase {
  using CBase = TBase;
public:
  using CBase::CBase;
  void stopListener() const {
    CBase::Object().stopListener();
  }
};


using CAnyKeyboardKillerT = CAnyObject<IAnyKeyboardKiller, CAnyKeyboardKillerImpl>;
} // NSDetail


class CAnyKeyboardKiller : public NSDetail::CAnyKeyboardKillerT {
  using CBase = NSDetail::CAnyKeyboardKillerT;
public:
  using CBase::CBase;
};

#endif // ANYKEYBOARDKILLER_H
