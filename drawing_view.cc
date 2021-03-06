#include "drawing/drawing_view.h"

#include "base/strings/utf_string_conversions.h"
#include "ui/views/background.h"
#include "drawing/config_node.h"
#include "drawing/drawing.h"

using ::base::UTF8ToUTF16;

DrawingListView::DrawingListView(int32 count, ConfigNode* node) {
  ConfigNode* cfg = node->GetNodeFromPath("//draw_window/draw_view");
  fontlist_.reset(new gfx::FontList(cfg->GetChildTextString("font")));
  SkColor color = StringToColor(cfg->GetChildTextString("text_color"));
  SkColor bgcolor = StringToColor(cfg->GetChildTextString("background_color"));
  for (int32 i = 0; i < count; ++i) {
    labels_.push_back(new views::Label());
    AddChildView(labels_.back());
    labels_.back()->SetFontList(*fontlist_.get());
    labels_.back()->SetEnabledColor(color);
    labels_.back()->SetBackgroundColor(bgcolor);
    labels_.back()->SetAutoColorReadabilityEnabled(false);
  }
}

void DrawingListView::Layout() {
  gfx::Rect bounds = std::move(GetContentsBounds());
  int y = bounds.y();
  int32 height = bounds.height() / labels_.size();
  for (auto iter = labels_.begin(); iter != labels_.end(); ++iter) {
    views::Label* label = *iter;
    gfx::Rect rect(bounds.x(), y, bounds.width(), height);
    label->SetBoundsRect(rect);
    y = rect.bottom();
  }
}

void DrawingListView::SetContentsList(const std::vector<std::string>& list) {
  int32 i = 0;
  for (auto iter = list.begin(); iter != list.end() && i < labels_.size();
       ++iter, ++i) {
    labels_[i]->SetText(UTF8ToUTF16(*iter));
  }
}


DrawingView::DrawingView(ConfigNode* node) 
    : drawing_(false),
      button_(NULL),
      drawing_list_view_(NULL) {
  SetFocusable(true);

  ConfigNode* dview = node->GetNodeFromPath("//draw_window/draw_view");
  drawing_list_view_ = new DrawingListView(10, node);
  AddChildView(drawing_list_view_);
  CHECK(StringToFloat4(dview->GetChildTextString("relbounds"), draw_list_bounds_));

  ConfigNode* buttoncfg = node->GetNodeFromPath("//draw_window/draw_button");
  button_frontlist_.reset(new gfx::FontList(buttoncfg->GetChildTextString("font")));
  start_title_ = buttoncfg->GetChildTextString("start_title");
  stop_title_ = buttoncfg->GetChildTextString("stop_title");
  button_ = new views::LabelButton(this, ::base::UTF8ToUTF16(start_title_));
  button_->SetHorizontalAlignment(gfx::ALIGN_CENTER);
  AddChildView(button_);
  button_->SetFontList(*button_frontlist_.get());
  CHECK(StringToFloat4(dview->GetChildTextString("relbounds"), button_bounds_));

  drawing_list_.reset(new Drawing);
  CHECK(drawing_list_->InitDrawingList(
      base::FilePath(UTF8ToUTF16("d://chrome/src")), 4));
}

DrawingView::~DrawingView() {
}

void DrawingView::Layout() {
  gfx::Rect bounds = this->bounds();
  gfx::Rect list_bounds(bounds.width() * draw_list_bounds_[0],
                        bounds.height() * draw_list_bounds_[1],
                        bounds.width() * draw_list_bounds_[2],
                        bounds.height() * draw_list_bounds_[3]);
  drawing_list_view_->SetBoundsRect(list_bounds);

  gfx::Rect button_bounds(bounds.width() * button_bounds_[0],
                            bounds.height() * button_bounds_[1],
                            bounds.width() * button_bounds_[2],
                            bounds.height() * button_bounds_[3]);
  button_->SetBoundsRect(list_bounds);
}

void DrawingView::OnTimer() {
  std::vector<std::string> ret;
  drawing_list_->Shuff(10, &ret);
  drawing_list_view_->SetContentsList(ret);
}

void DrawingView::ButtonPressed(views::Button* sender, const ui::Event& event) {
  if (!timer_.get()) {
    timer_ = new TimerHelper(::base::TimeDelta::FromMilliseconds(30), true, this);
    timer_->Reset();
    button_->SetText(::base::UTF8ToUTF16(stop_title_));
  } else {
    timer_->Stop();
  }
}

