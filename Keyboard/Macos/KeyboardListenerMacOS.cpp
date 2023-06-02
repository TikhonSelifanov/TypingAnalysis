#include "KeyboardListenerMacOS.h"

#include "TimerAccess.h"
#include "Keyboard/KeyboardHandler.h"
#include "KeyIDMacOS.h"
#include "KeyPositionMacOS.h"

namespace NSApplication {
namespace NSKeyboard {
namespace NSMacOS {

// ########################## CKeyboardListenerMacOSImpl
CKeyboardListenerMacOSImpl::CKeyboardListenerMacOSImpl(
    CAnyKillerPromise KillerPromise, CKeyboardHandler* KeyboardHandler)
          : EventTap_(createEventTap()), MessagePort_(createMessagePort()) {
  Q_ASSERT(::CGEventTapIsEnabled(EventTap_.get()));
  addEventTapToRunLoop();

  Q_ASSERT(::CFMessagePortIsValid(MessagePort_.get()));
  addMessagePortToRunLoop();

  KillerPromise.set_value(CKiller(MessagePort_.get()));

  connect(this,             &CKeyboardListenerMacOSImpl::KeyPressing,
          KeyboardHandler,  &CKeyboardHandler::onKeyPressing);
  connect(this,             &CKeyboardListenerMacOSImpl::KeyReleasing,
          KeyboardHandler,  &CKeyboardHandler::onKeyReleasing);
}

// ########################## ~CKeyboardListenerMacOSImpl
CKeyboardListenerMacOSImpl::~CKeyboardListenerMacOSImpl() {
  disconnect(this, &CKeyboardListenerMacOSImpl::KeyPressing,  nullptr, nullptr);
  disconnect(this, &CKeyboardListenerMacOSImpl::KeyReleasing, nullptr, nullptr);
}


int CKeyboardListenerMacOSImpl::exec() {
  CMacOSKeyboardAPI::runRunLoop();
}


// ########################## createEventTap
CFMachPortRef CKeyboardListenerMacOSImpl::createEventTap() {
  CFMachPortRef EventTap =
          CMacOSKeyboardAPI::createEventTapForKeyboard(callbackEventTap, this);
  if (EventTap == NULL) {
    throw std::runtime_error("Failed to create EventTap");
  }
  return EventTap;
}

// ########################## addEventTapToRunLoop
void CKeyboardListenerMacOSImpl::addEventTapToRunLoop() {
  CMacOSKeyboardAPI::UniquePtr<CFRunLoopSourceRef> RunLoopSource(
                        CMacOSKeyboardAPI::createRunLoopSourceForEventTap(EventTap_.get()));
  if (RunLoopSource.get() == NULL) {
    throw std::runtime_error("Failed to create RunLoopSource for EventTap");
  }

  CMacOSKeyboardAPI::addSourceRunLoopТoCurrentRunLoop(RunLoopSource.get());
}

QString CKeyboardListenerMacOSImpl::getDeadKeyText(const CVKCode VKCode) {
  UniChar unicodeString[4];
  UniCharCount unicodeStringLength;
  CGEventRef event = CGEventCreateKeyboardEvent(NULL, VKCode, true);
  CGEventKeyboardGetUnicodeString(event, 4, &unicodeStringLength, unicodeString);
  CFRelease(event);
  return QString::fromUtf16(unicodeString, unicodeStringLength);
}

bool CKeyboardListenerMacOSImpl::isDeadKey(CKeyTextData& keyTextData) {
  return keyTextData.Size == 3;
}

CKeyboardListenerMacOSImpl::CType CKeyboardListenerMacOSImpl::getFlag(const CVKCode VKCode)
{
  switch (VKCode) {
    case ::kVK_Shift:
        return NSKernel::CKeyFlagsEnum::Shift;
    case ::kVK_RightShift:
        return NSKernel::CKeyFlagsEnum::Shift;
    case ::kVK_RightControl:
        return NSKernel::CKeyFlagsEnum::Ctrl;
    case ::kVK_Control:
        return NSKernel::CKeyFlagsEnum::Ctrl;
    case ::kVK_Option:
        return NSKernel::CKeyFlagsEnum::Alt;
    case ::kVK_RightOption:
        return NSKernel::CKeyFlagsEnum::Alt;
    case ::kVK_CapsLock:
        return NSKernel::CKeyFlagsEnum::Capslock;
    default:
        return NSKernel::CKeyFlagsEnum::BasicKey;
  }
}

// ########################## callbackEventTap
CGEventRef CKeyboardListenerMacOSImpl::callbackEventTap(
                                  [[maybe_unused]]CGEventTapProxy   _,
                                  CGEventType                       Type,
                                  CGEventRef                        Event,
                                  void*                             UserInfo) {
  CTimerAccess Timer;
  auto Time = Timer->get();

  Q_ASSERT(UserInfo != NULL);
  CKeyboardListenerMacOSImpl* Listener = getListener(UserInfo);

  CVKCode VKCode = CMacOSKeyboardAPI::getVKCode(Event);
  CKeyTextData keyTextData = Listener->KeyTextMaker_.get(VKCode);
  CLabelData labelData = getKeyLabel(VKCode, keyTextData);
  CKeyboardListenerMacOSImpl::CType flag;
  if (isDeadKey(keyTextData)) {
    flag = NSKernel::CKeyFlagsEnum::DeadKey;
  }
  else {
    flag = getFlag(VKCode);
  }

  if (CMacOSKeyboardAPI::isCapsLock(VKCode)) {
    Q_EMIT Listener->KeyPressing(
          { Time,
            CKeyPositionMacOS::make(VKCode),
            CKeyIDMacOS::make(VKCode),
            labelData,
            keyTextData,
            flag });
    Q_EMIT Listener->KeyReleasing(
          { Time,
            CKeyPositionMacOS::make(VKCode),
            CKeyIDMacOS::make(VKCode) } );
    return Event;
  }

  if (CMacOSKeyboardAPI::isPressing(Type, Listener->ShifterInfo_, VKCode)) {
    Q_EMIT Listener->KeyPressing(
          { Time,
            CKeyPositionMacOS::make(VKCode),
            CKeyIDMacOS::make(VKCode),
            labelData,
            keyTextData,
            flag });
  } else {
    Q_EMIT Listener->KeyReleasing(
          { Time,
            CKeyPositionMacOS::make(VKCode),
            CKeyIDMacOS::make(VKCode)});
  }

  return Event;
}


// ########################## createMessagePort
CFMessagePortRef CKeyboardListenerMacOSImpl::createMessagePort() {
  CFMessagePortRef MessagePort = CMacOSKeyboardAPI::createMessagePort(callbackMessagePort);
  if (MessagePort == NULL) {
    throw std::runtime_error("Failed to create MessagePort");
  }
  return MessagePort;
}

// ########################## addMessagePortToRunLoop
void CKeyboardListenerMacOSImpl::addMessagePortToRunLoop() {
  CMacOSKeyboardAPI::UniquePtr<CFRunLoopSourceRef> RunLoopSource(
                        CMacOSKeyboardAPI::createRunLoopSourceForMessagePort(MessagePort_.get()));
  if (RunLoopSource.get() == NULL) {
    throw std::runtime_error("Failed to create RunLoopSource for MessagePort");
  }

  CMacOSKeyboardAPI::addSourceRunLoopТoCurrentRunLoop(RunLoopSource.get());
}

// ########################## callbackMessagePort
CFDataRef
CKeyboardListenerMacOSImpl::callbackMessagePort(CFMessagePortRef, SInt32, CFDataRef, void*) {
  CMacOSKeyboardAPI::stopCurrentRunLoop();
  return NULL;
}

// ########################## getKeyLabel
CLabelData CKeyboardListenerMacOSImpl::getKeyLabel(const CVKCode VKCode, CKeyTextData& keyTextData) {
  if (isDeadKey(keyTextData) && getDeadKeyText(VKCode).size() != 0)
  {
    return {getDeadKeyText(VKCode)[0], QChar(), 1};
  }
  if (keyTextData.Size > 0)
  {
    return {keyTextData.Symbol[1], QChar(), 1};
  }
  return {QChar(), QChar(), 0};
}

// ########################## getListener
CKeyboardListenerMacOSImpl* CKeyboardListenerMacOSImpl::getListener(void* ListenerInfo) {
  return static_cast<CKeyboardListenerMacOSImpl*>(ListenerInfo);
}


//#####################################
/*              CKiller              */
//#####################################

CKiller::CKiller(CFMessagePortRef KillerPort) : KillerPort_(KillerPort) {}
void CKiller::stopListener() const {
  if (CMacOSKeyboardAPI::sendEmptyMessage(KillerPort_) != kCFMessagePortSuccess) {
    throw std::runtime_error("Failed to send message via CKiller");
  }
}


} // namespace NSMacOS
} // namespace NSKeyboard
} // namespace NSApplication
