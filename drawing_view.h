#pragma once

#include "base/timer/timer.h"
#include "ui/views/controls/label.h"
#include "ui/views/controls/button/label_button.h"
#include "ui/views/controls/button/button.h"
#include "drawing/timer_helper.h"

class Drawing;
class ConfigNode;
class DrawingListView : public views::View {
 public:
  DrawingListView(int32 count, ConfigNode* node);
  void Layout() override;
  void SetContentsList(const std::vector<std::string>& list);
 private:
  std::vector<views::Label*> labels_;
  scoped_ptr<gfx::FontList> fontlist_;
  static const int32 kCellHeight = 32;
  DISALLOW_COPY_AND_ASSIGN(DrawingListView);
};


class DrawingView : public views::View,
                    public views::ButtonListener,
                    public TimerHelper::Delegate {
 public:
  DrawingView(ConfigNode* node);
  ~DrawingView();
  void ButtonPressed(views::Button* sender, const ui::Event& event) override;
  void Layout() override;
 private:
  void OnTimer() override;
  scoped_refptr<TimerHelper> timer_;
  bool drawing_;
  std::string start_title_;
  std::string stop_title_;
  views::LabelButton* button_;
  DrawingListView* drawing_list_view_;
  scoped_ptr<gfx::FontList> button_frontlist_;
  scoped_ptr<Drawing> drawing_list_;
  float draw_list_bounds_[4];
  float button_bounds_[4];
  DISALLOW_COPY_AND_ASSIGN(DrawingView);
};
