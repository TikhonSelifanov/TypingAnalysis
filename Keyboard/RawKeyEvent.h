#ifndef NSAPPLICATION_NSKEYBOARD_CRAWKEYEVENT_H
#define NSAPPLICATION_NSKEYBOARD_CRAWKEYEVENT_H

#include "TimeApp.h"
#include "Keyboard/KeyID.h"
#include "Keyboard/KeyPosition.h"
#include "Keyboard/KeyTextData.h"
#include "Keyboard/KeyFlags.h"

#include <QChar>
#include <QObject>
#include <QString>

namespace NSApplication {
namespace NSKeyboard {

struct CKeyPressing {
  using CKeyFlags = NSKernel::CKeyFlags;
  CTime PressingTime;
  CKeyPosition KeyPosition;
  CKeyID KeyID;
  CLabelData KeyLabel;
  CKeyTextData KeyText;
  CKeyFlags Shifters;
};

struct CKeyReleasing {
  CTime ReleasingTime;
  CKeyPosition KeyPosition;
  CKeyID KeyID;
};

} // namespace NSKeyboard
} // namespace NSApplication

// This macro is required to use the CRawKeyEvent class in Qt message system
Q_DECLARE_METATYPE(NSApplication::NSKeyboard::CKeyPressing);
Q_DECLARE_METATYPE(NSApplication::NSKeyboard::CKeyReleasing);
#endif // NSAPPLICATION_NSKEYBOARD_CRAWKEYEVENT_H
