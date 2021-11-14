#ifndef NSAPPLICATION_CAPPLICATIONGUI_H
#define NSAPPLICATION_CAPPLICATIONGUI_H

#include "Interface/Plotter.h"
#include "Interface/SeanceView.h"
#include "Interface/StatisticsView.h"
#include "Interface/TextModeView.h"
#include "Interface/TextPrinter.h"
#include "Qt/MainWindow.h"

#include <memory>

#ifdef KEYBOARD_HANDLER_DEBUG
#include "AppDebug/KeyboardHandlerDebugOut.h"
#endif

namespace NSApplication {

class CApplicationGUI {
public:
  CApplicationGUI();

protected:
  NSQt::CMainWindow* mainWindow();

private:
  NSQt::CMainWindow AppWindow_;

protected:
  NSInterface::CSeanceView MainSeanceView_;
  NSInterface::CTextModeView TextModeView_;
  NSInterface::CTextPrinter MainTextPrinter_;
  NSInterface::CPlotter SpeedPlotter_;
  NSInterface::CStatisticsView StatisticsView_;

#ifdef KEYBOARD_HANDLER_DEBUG
  NSAppDebug::CKeyboardHandlerDebugOut KeyboardHandlerOut_ = mainWindow();
#endif
};

} // namespace NSApplication

#endif // NSAPPLICATION_CAPPLICATIONGUI_H
