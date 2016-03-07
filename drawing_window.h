#pragma once

#include "ui/views/view.h"
#include "ui/views/controls/image_view.h"
#include "ui/views/widget/widget_delegate.h"
#include "drawing/show_animation.h"

class ConfigNode;
class DrawingView;

class DrawingWindow : public views::WidgetDelegateView,
                      public ShowAnimationViewObserver {
 public:
  DrawingWindow(const base::FilePath& config, int round);
  void ShowWindow();
  void Close();

  void Layout() override;
  bool CanResize() const override { return false; }
  bool CanMaximize() const override { return false; }
  bool CanMinimize() const override { return false; }
  base::string16 GetWindowTitle() const override;
  View* GetContentsView() override { return this; }
  void WindowClosing() override;
  bool OnKeyPressed(const ui::KeyEvent& event) override;
  bool OnKeyReleased(const ui::KeyEvent& event) override;
  void OnBoundsAnimatorDone(ShowAnimationView* view) override;
 private:
  enum {
    kInit,
    kShowAnimation,
    kStartDrawing,
    kEndDrawing,
  };
  int32 state_;
  DrawingView* drawing_view_;
  ShowAnimationView* animation_;
  DISALLOW_COPY_AND_ASSIGN(DrawingWindow);
};
