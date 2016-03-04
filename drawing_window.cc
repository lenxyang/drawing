#include "drawing/drawing_window.h"

#include "base/strings/utf_string_conversions.h"
#include "azer/util/xml/xml.h"

namespace {
const char* kFontName = "Microsoft MHei, 24px";
const char* kCaptionTitle = "Start";

class WindowContents : public views::WidgetDelegateView,
                       public views::ButtonListener {
 public:
  WindowContents() 
      : fontlist_(kFontName) {
    button_ = new views::LabelButton(this, ::base::UTF8ToUTF16(kCaptionTitle));
    AddChildView(button_);
    button_->SetFontList(fontlist_);
    button_->SetBoundsRect(gfx::Rect(100, 600, 300, 40));
  }
  bool CanResize() const override { return true; }
  bool CanMaximize() const override { return true; }
  bool CanMinimize() const override { return true; }
  base::string16 GetWindowTitle() const override {
    return base::ASCIIToUTF16("Views Examples");
  }
  View* GetContentsView() override { return this; }
  void WindowClosing() override {
    base::MessageLoopForUI::current()->Quit();
  }

  void ButtonPressed(views::Button* sender, const ui::Event& event) override {
    button_->SetText(::base::UTF8ToUTF16("Stop"));
  }

 private:
  views::LabelButton* button_;
  gfx::FontList fontlist_;
  DISAcLOW_COPY_AND_ASSIGN(WindowContents);
};
     
}  // namespace


DrawingWindow::DrawingWindow() {
}

void DrawingWindow::Show() {
  // delegate and views must be create by "new"
  WindowContents* contents = new WindowContents;
  views::Widget* widget = new views::Widget;
  views::Widget::InitParams wparams;
  wparams.parent = NULL;
  wparams.type = views::Widget::InitParams::TYPE_WINDOW;
  wparams.delegate = contents;
  wparams.context  = NULL;
  wparams.bounds = gfx::Rect(0, 0, 800, 600);
  widget->Init(wparams);
  widget->SetFullscreen(true);
  widget->Show();
}
