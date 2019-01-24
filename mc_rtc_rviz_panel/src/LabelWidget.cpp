#include "LabelWidget.h"

namespace mc_rtc_rviz
{

LabelWidget::LabelWidget(const ClientWidgetParam & param)
: ClientWidget(param)
{
  layout_ = new QHBoxLayout(this);
  layout_->addWidget(new QLabel(name().c_str(), this));
  label_ = new QLabel("", this);
  label_->setWordWrap(true);
  layout_->addWidget(label_);
}

void LabelWidget::update(const std::string & in)
{
  label_->setText(in.c_str());
}

}
