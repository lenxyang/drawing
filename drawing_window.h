#pragma once

#include "nelf/nelf.h"

class DrawingWindow {
 public:
  DrawingWindow();
  void Show();
 private:
  scoped_ptr<views::Widget> widget_;
  DISALLOW_COPY_AND_ASSIGN(DrawingWindow);
};
