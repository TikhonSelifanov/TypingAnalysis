#include "Plotter.h"

#include <cassert>

#include "qwt_legend.h"
#include "qwt_legend_label.h"
#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_grid.h"

namespace NSApplication {
namespace NSInterface {

namespace NSPlotterDetail {

CSpeedPlotterImpl::CSpeedPlotterImpl(QwtPlot* Plot)
    : Plot_(Plot), SpeedDataInput_([this](const CPlotData& PlotData) {
        handlePlotData(PlotData);
      }),
      LocalizerInput_(
          [this](const CLocalizer& Localizer) { setLocale(Localizer); }) {
  assert(Plot_);
  adjustPlot();
  setCurves();
}

NSPlotterDetail::CSpeedPlotterImpl::CPlotDataObserver*
NSPlotterDetail::CSpeedPlotterImpl::speedDataInput() {
  return &SpeedDataInput_;
}

CSpeedPlotterImpl::CLocalizerObserver* CSpeedPlotterImpl::localizerInput() {
  return &LocalizerInput_;
}

void CSpeedPlotterImpl::legendChecked(const QVariant& itemInfo, bool on, int) {
  QwtPlotItem* plotItem = Plot_->infoToItem(itemInfo);
  if (plotItem)
    plotItem->setVisible(on);
  Plot_->replot();
}

CSpeedPlotterImpl::~CSpeedPlotterImpl() = default;

void CSpeedPlotterImpl::adjustPlot() {
  Plot_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  //  Plot_->setTitle("");
  std::unique_ptr<QwtLegend> Legend = std::make_unique<QwtLegend>();
  Legend->setDefaultItemMode(QwtLegendData::Checkable);
  QObject::connect(Legend.get(), &QwtLegend::checked, this,
                   &CSpeedPlotterImpl::legendChecked);
  Plot_->insertLegend(Legend.release(), QwtPlot::RightLegend);

  Plot_->setAxisVisible(QwtAxis::YRight);
  //  Plot_->setAxisTitle(QwtAxis::XBottom, "");

  std::unique_ptr<QwtPlotGrid> grid = std::make_unique<QwtPlotGrid>();
  grid->enableXMin(true);
  grid->setMajorPen(Qt::gray, 0, Qt::DotLine);
  grid->setMinorPen(Qt::gray, 0, Qt::DotLine);
  grid.release()->attach(Plot_);
}

void CSpeedPlotterImpl::setCurves() {

  std::unique_ptr<QwtPlotCurve> Speed0 = std::make_unique<QwtPlotCurve>("");
  Speed0->setLegendAttribute(QwtPlotCurve::LegendShowLine);
  Speed0->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
  Speed0->setCurveAttribute(QwtPlotCurve::Fitted, true);
  Speed0->setRenderHint(QwtPlotItem::RenderAntialiased);
  Speed0->setPen(QColor(200, 50, 50));
  Speed0_ = Speed0.release();
  Speed0_->attach(Plot_);
  checkItem(Speed0_, true);

  std::unique_ptr<QwtPlotCurve> Speed1 = std::make_unique<QwtPlotCurve>("");
  Speed1->setLegendAttribute(QwtPlotCurve::LegendShowLine);
  Speed1->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
  Speed1->setCurveAttribute(QwtPlotCurve::Fitted, true);
  Speed1->setRenderHint(QwtPlotItem::RenderAntialiased);
  Speed1->setPen(QColor(50, 50, 200));
  Speed1->setYAxis(QwtAxis::YRight);
  Speed1_ = Speed1.release();
  Speed1_->attach(Plot_);
  checkItem(Speed1_, true);

  std::unique_ptr<QwtPlotCurve> SpeedMB0 = std::make_unique<QwtPlotCurve>("");
  SpeedMB0->setLegendAttribute(QwtPlotCurve::LegendShowLine);
  SpeedMB0->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
  SpeedMB0->setCurveAttribute(QwtPlotCurve::Fitted, true);
  SpeedMB0->setRenderHint(QwtPlotItem::RenderAntialiased);
  SpeedMB0->setPen(QColor(50, 200, 50));
  SpeedMB0_ = SpeedMB0.release();
  SpeedMB0_->attach(Plot_);
  checkItem(SpeedMB0_, true);

  std::unique_ptr<QwtPlotCurve> SpeedMB1 = std::make_unique<QwtPlotCurve>("");
  SpeedMB1->setLegendAttribute(QwtPlotCurve::LegendShowLine);
  SpeedMB1->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
  SpeedMB1->setCurveAttribute(QwtPlotCurve::Fitted, true);
  SpeedMB1->setRenderHint(QwtPlotItem::RenderAntialiased);
  SpeedMB1->setPen(QColor(50, 50, 200));
  SpeedMB1->setYAxis(QwtAxis::YRight);
  SpeedMB1_ = SpeedMB1.release();
  SpeedMB1_->attach(Plot_);
  checkItem(SpeedMB1_, true);

  std::unique_ptr<QwtPlotCurve> SpeedR0 = std::make_unique<QwtPlotCurve>("");
  SpeedR0->setLegendAttribute(QwtPlotCurve::LegendShowLine);
  SpeedR0->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
  SpeedR0->setCurveAttribute(QwtPlotCurve::Fitted, true);
  SpeedR0->setRenderHint(QwtPlotItem::RenderAntialiased);
  SpeedR0->setPen(QColor(50, 200, 200));
  SpeedR0_ = SpeedR0.release();
  SpeedR0_->attach(Plot_);
  checkItem(SpeedR0_, true);

  std::unique_ptr<QwtPlotCurve> SpeedR1 = std::make_unique<QwtPlotCurve>("");
  SpeedR1->setLegendAttribute(QwtPlotCurve::LegendShowLine);
  SpeedR1->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
  SpeedR1->setCurveAttribute(QwtPlotCurve::Fitted, true);
  SpeedR1->setRenderHint(QwtPlotItem::RenderAntialiased);
  SpeedR1->setPen(QColor(50, 50, 200));
  SpeedR1->setYAxis(QwtAxis::YRight);
  SpeedR1_ = SpeedR1.release();
  SpeedR1_->attach(Plot_);
  checkItem(SpeedR1_, true);
}

void CSpeedPlotterImpl::checkItem(QwtPlotItem* item, bool on) {

  QwtLegend* lgd = qobject_cast<QwtLegend*>(Plot_->legend());

  QList<QWidget*> legendWidgets = lgd->legendWidgets(Plot_->itemToInfo(item));

  if (legendWidgets.size() == 1) {
    QwtLegendLabel* legendLabel =
        qobject_cast<QwtLegendLabel*>(legendWidgets[0]);

    if (legendLabel)
      legendLabel->setChecked(on);
  }
}

void NSPlotterDetail::CSpeedPlotterImpl::handlePlotData(
    const CPlotData& PlotData) {
  Speed0_->setRawSamples(PlotData.dataX(), PlotData.dataY0(),
                         PlotData.gridSize());
  Speed1_->setRawSamples(PlotData.dataX(), PlotData.dataY1(),
                         PlotData.gridSize());
  SpeedMB0_->setRawSamples(PlotData.dataX(), PlotData.dataYMB0(),
                           PlotData.gridSize());
  SpeedMB1_->setRawSamples(PlotData.dataX(), PlotData.dataYMB1(),
                           PlotData.gridSize());
  SpeedR0_->setRawSamples(PlotData.dataX(), PlotData.dataYR0(),
                          PlotData.gridSize());
  SpeedR1_->setRawSamples(PlotData.dataX(), PlotData.dataYR1(),
                          PlotData.gridSize());
  Plot_->replot();
}

void CSpeedPlotterImpl::setLocale(const CLocalizer& Localizer) {
  Plot_->setTitle(Localizer.plotterTitle());
  Plot_->setAxisTitle(QwtAxis::XBottom, Localizer.horizontalAxisTitle());
  Speed0_->setTitle(Localizer.density1Name());
  Speed1_->setTitle(Localizer.derivative1Name());
  SpeedMB0_->setTitle(Localizer.density2Name());
  SpeedMB1_->setTitle(Localizer.derivative2Name());
  SpeedR0_->setTitle(Localizer.density3Name());
  SpeedR1_->setTitle(Localizer.derivative3Name());
}

} // namespace NSPlotterDetail
} // namespace NSInterface
} // namespace NSApplication
