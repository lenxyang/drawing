#pragma once

#include "base/timer/timer.h"
#include "ui/views/controls/label.h"
#include "ui/views/controls/button/label_button.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/view.h"
#include "ui/views/widget/widget.h"
#include "ui/views/widget/widget_delegate.h"
#include "ui/gfx/font_list.h"
#include "drawing/timer_helper.h"
#include "drawing/drawing.h"

class ConfigNode;
class DrawingView : public views::View {
 public:
  DrawingView(int32 count, ConfigNode* node);
  void Layout() override;
  void SetContentsList(const std::vector<std::string>& list);
 private:
  std::vector<views::Label*> labels_;
  scoped_ptr<gfx::FontList> fontlist_;
  static const int32 kCellHeight = 32;
  DISALLOW_COPY_AND_ASSIGN(DrawingView);
};

class WindowContents : public views::WidgetDelegateView,
                       public views::ButtonListener,
                       public TimerHelper::Delegate {
 public:
  WindowContents(ConfigNode* node);
  bool CanResize() const override { return false; }
  bool CanMaximize() const override { return false; }
  bool CanMinimize() const override { return false; }
  base::string16 GetWindowTitle() const override;
  View* GetContentsView() override { return this; }
  void WindowClosing() override;
  void ButtonPressed(views::Button* sender, const ui::Event& event) override;
  bool OnKeyPressed(const ui::KeyEvent& event) override;
  bool OnKeyReleased(const ui::KeyEvent& event) override;
 private:
  void OnTimer() override;
  scoped_refptr<TimerHelper> timer_;
  bool drawing_;
  std::string start_title_;
  std::string stop_title_;
  views::LabelButton* button_;
  DrawingView* drawing_view_;
  scoped_ptr<gfx::FontList> button_frontlist_;
  scoped_ptr<Drawing> drawing_list_;
  DISALLOW_COPY_AND_ASSIGN(WindowContents);
};
class DrawingWindow {
 public:
  DrawingWindow();
  ~DrawingWindow();
  
  void Init(const base::FilePath& config);
  void Show();
  void Close();
 private:
  views::Widget* widget_;
  WindowContents* contents_;
  DISALLOW_COPY_AND_ASSIGN(DrawingWindow);
};
