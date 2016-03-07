#include <memory>

#include "base/files/file_path.h"
#include "base/strings/stringprintf.h"
#include "base/strings/utf_string_conversions.h"
#include "nelf/nelf.h"
#include "ui/views/controls/button/button.h"
#include "drawing/drawing_window.h"
                          
using views::Widget;
using base::FilePath;
using base::UTF8ToUTF16;

class MainContents : public views::View,
                     public views::ButtonListener {
 public:
  MainContents() {
    for (int32 i = 0; i < kRoundCount; ++i) {
      std::string title = ::base::StringPrintf("Round %d", i);
      buttons_.push_back(new nelf::Button(this, UTF8ToUTF16(title)));
      AddChildView(buttons_.back());
      buttons_.back()->set_tag(i);

      title = ::base::StringPrintf("Result %d", i);
      views_buttons_.push_back(new nelf::Button(this, UTF8ToUTF16(title)));
      AddChildView(views_buttons_.back());
      views_buttons_.back()->set_tag(kViewtButtonBase + i);

      title = ::base::StringPrintf("Reset %d", i);
      reset_buttons_.push_back(new nelf::Button(this, UTF8ToUTF16(title)));
      AddChildView(reset_buttons_.back());
      reset_buttons_.back()->set_tag(kResetButtonBase + i);
    }

    Layout();
  }

  gfx::Insets GetInsets() const override {
    return gfx::Insets(10, 10, 10, 10);
  }

  void Layout() override {
    gfx::Rect bounds = GetContentsBounds();
    int y = bounds.y();
    int32 width = 80;
    int32 height = 30;
    int32 margin = 15;
    for (int32 i = 0; i < kRoundCount; ++i) {
      gfx::Rect rect(bounds.x(), y, width, 30);
      buttons_[i]->SetBoundsRect(rect);
      y = rect.bottom();
      y += margin;
    }

    y = bounds.y();
    for (int32 i = 0; i < kRoundCount; ++i) {
      int x = bounds.x() + (margin + width);
      gfx::Rect rect(x, y, width, 30);
      views_buttons_[i]->SetBoundsRect(rect);
      y = rect.bottom();
      y += margin;
    }

    y = bounds.y();
    for (int32 i = 0; i < kRoundCount; ++i) {
      int x = bounds.x() + (margin + width) * 2;
      gfx::Rect rect(x, y, width, 30);
      reset_buttons_[i]->SetBoundsRect(rect);
      y = rect.bottom();
      y += margin;
    }
  }


  void ButtonPressed(views::Button* sender, const ui::Event& event) override {
    if (sender->tag() < kResetButtonBase) {
      ::base::FilePath path(UTF8ToUTF16("drawing/drawing.xml"));
      DrawingWindow* window = new DrawingWindow(path);
      window->ShowWindow();
    }
  }

 private:
  std::vector<nelf::Button*> buttons_;
  std::vector<nelf::Button*> reset_buttons_;
  std::vector<nelf::Button*> views_buttons_;
  static const int32 kResetButtonBase = 1000;
  static const int32 kViewtButtonBase = 100;
  static const int32 kRoundCount = 4;
};

int main(int argc, char* argv[]) {
  scoped_refptr<nelf::Context> context(
      nelf::Context::CreateStandAloneContext(argc, argv));
  CHECK(context.get());

  nelf::MainFrame* mainframe = new nelf::MainFrame(
      gfx::Rect(100, 100, 320, 220), context.get());
  mainframe->SetTitle(::base::UTF8ToUTF16("Drawing"));
  mainframe->SetShowTitle(true);
  mainframe->Init();
  MainContents* contents = new MainContents;
  mainframe->AddChildView(contents);
  mainframe->SetLayoutManager(new views::FillLayout);
  mainframe->Show();


  base::RunLoop().Run();
  return 0;
}
