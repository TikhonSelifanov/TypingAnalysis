#ifndef NSAPPLICATION_NSLOCAL_CRUSLOCALE_H
#define NSAPPLICATION_NSLOCAL_CRUSLOCALE_H

namespace NSApplication {
namespace NSLocal {

struct CRusLocale {
  struct CSeanceView {
    static constexpr const char* Description = "Описание";
    static constexpr const char* Size = "Размер";
    static constexpr const char* CurrentSeance = "Текущий сеанс";
  };

  struct CTextModeView {
    static constexpr const char* TextMode = "Тексторвый режим";
    static constexpr const char* Raw = "Сырой";
    static constexpr const char* Full = "Полный";
    static constexpr const char* Printed = "Напечатанный";
    static constexpr const char* Non = "Нет";
    static constexpr const char* All = "Все";
    static constexpr const char* Essential = "Существенные";
    static constexpr const char* Shift = "Shift";
    static constexpr const char* Ctrl = "Ctrl";
    static constexpr const char* Alt = "Alt";
  };

  struct CSpeedPlotter {
    static constexpr const char* PlotterTitle = "Плотность скорости";
    static constexpr const char* HorizontalAxisTitle = "Скорость, знаки/минута";
    static constexpr const char* Density1Name = "Плотность";
    static constexpr const char* Derivative1Name = "Производная";
    static constexpr const char* Density2Name = "Плотность М-Б";
    static constexpr const char* Derivative2Name = "Производная М-Б";
    static constexpr const char* Density3Name = "Плотность Р";
    static constexpr const char* Derivative3Name = "Производная Р";
  };
};

} // namespace NSLocal
} // namespace NSApplication

#endif // NSAPPLICATION_NSLOCAL_CRUSLOCALE_H
