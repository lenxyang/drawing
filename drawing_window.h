#pragma once

#include "ui/views/controls/label.h"
#include "ui/views/controls/button/label_button.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/view.h"
#include "ui/views/widget/widget.h"
#include "ui/views/widget/widget_delegate.h"
#include "ui/gfx/font_list.h"

class ConfigNode;
class DrawingView;
class WindowContents : public views::WidgetDelegateView,
                       public views::ButtonListener {
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
  bool drawing_;
  std::string start_title_;
  std::string stop_title_;
  views::LabelButton* button_;
  DrawingView* drawing_view_;
  scoped_ptr<gfx::FontList> button_frontlist_;
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
