#include "drawing/drawing_window.h"

#include "base/files/file_util.h"
#include "base/strings/string_split.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/utf_string_conversions.h"
#include "ui/views/background.h"
#include "ui/views/color_constants.h"
#include "azer/util/xml/xml.h"
#include "drawing/config_node.h"

using base::UTF8ToUTF16;

namespace {
bool StringToIntVec(const std::string& text, std::vector<int>* vec) {
  std::vector<std::string> v;
  int iv;
  ::base::SplitString(text, ',', &v);
  for (auto iter = v.begin(); iter != v.end(); ++iter) {
    std::string str = *iter;
    if (str.empty())
      return false;
    if (str[str.length() - 1] == 'f')
      str = str.substr(0, str.length() - 1);
    if (!::base::StringToInt(str, &iv))
      return false;

    vec->push_back(iv);
  }
  return true;
}
gfx::Rect StringToRect(const std::string& str) {
  std::vector<int32> vec;
  CHECK(StringToIntVec(str, &vec));
  return gfx::Rect(vec[0], vec[1], vec[2], vec[3]);
}

SkColor StringToColor(const std::string& str) {
  std::vector<int32> vec;
  CHECK(StringToIntVec(str, &vec));
  return SkColorSetARGB(vec[0], vec[1], vec[2], vec[3]);
}
}

class DrawingView : public views::View {
 public:
  DrawingView(int32 count, ConfigNode* node) {
   ConfigNode* cfg = node->GetNodeFromPath("//draw_window/draw_view");
    fontlist_.reset(new gfx::FontList(cfg->GetChildTextString("font")));
    gfx::Rect button_bounds = StringToRect(cfg->GetChildTextString("bounds"));

    for (int32 i = 0; i < count; ++i) {
      labels_.push_back(new views::Label());
      AddChildView(labels_.back());
      labels_.back()->SetFontList(*fontlist_.get());
    }
  }

  void Layout() override {
    gfx::Rect bounds = std::move(GetContentsBounds());
    int y = bounds.y();
    for (auto iter = labels_.begin(); iter != labels_.end(); ++iter) {
      views::Label* label = *iter;
      label->SetBounds(bounds.x(), y, bounds.width(), kCellHeight);
      y += kCellHeight;
    }
  }

  void SetContentsList(const std::vector<std::string>& list) {
    int32 i = 0;
    for (auto iter = list.begin(); iter != list.end() && i < labels_.size();
         ++iter, ++i) {
      labels_[i]->SetText(UTF8ToUTF16(*iter));
    }
  }
 private:
  std::vector<views::Label*> labels_;
  scoped_ptr<gfx::FontList> fontlist_;
  static const int32 kCellHeight = 32;
  DISALLOW_COPY_AND_ASSIGN(DrawingView);
};

WindowContents::WindowContents(ConfigNode* node) 
    : drawing_(false),
      button_(NULL),
      drawing_view_(NULL) {
  SetFocusable(true);

  ConfigNode* buttoncfg = node->GetNodeFromPath("//draw_window/draw_button");
  button_frontlist_.reset(new gfx::FontList(buttoncfg->GetChildTextString("font")));
  gfx::Rect bounds = StringToRect(buttoncfg->GetChildTextString("bounds"));
  start_title_ = buttoncfg->GetChildTextString("start_title");
  stop_title_ = buttoncfg->GetChildTextString("stop_title");
  button_ = new views::LabelButton(this, ::base::UTF8ToUTF16(start_title_));
  AddChildView(button_);
  button_->SetFontList(*button_frontlist_.get());
  button_->SetBoundsRect(bounds);

  ConfigNode* dview = node->GetNodeFromPath("//draw_window/draw_view");
  drawing_view_ = new DrawingView(20, node);
  AddChildView(drawing_view_);
  bounds = StringToRect(dview->GetChildTextString("bounds"));
  drawing_view_->SetBoundsRect(bounds);

  ConfigNode* cfg = node->GetNodeFromPath("//draw_window");
  SkColor bgcol = StringToColor(cfg->GetChildTextString("background"));
  set_background(views::Background::CreateSolidBackground(bgcol));
}

void WindowContents::WindowClosing() {
  base::MessageLoopForUI::current()->Quit();
}

void WindowContents::ButtonPressed(views::Button* sender, const ui::Event& event) {
  button_->SetText(::base::UTF8ToUTF16(stop_title_));
}

base::string16 WindowContents::GetWindowTitle() const {
  return base::ASCIIToUTF16("Drawing");
}

bool WindowContents::OnKeyPressed(const ui::KeyEvent& event) {
  return true;
}

bool WindowContents::OnKeyReleased(const ui::KeyEvent& event) {
  if (event.key_code() == ui::VKEY_ESCAPE)
    GetWidget()->Close();
  return true;
}

// class DrawingWindow
DrawingWindow::DrawingWindow() 
    : widget_(NULL),
      contents_(NULL) {
}

DrawingWindow::~DrawingWindow() {
}

void DrawingWindow::Init(const base::FilePath& path) {
  std::string str;
  CHECK(::base::ReadFileToString(path, &str));
  ConfigNodePtr config = ConfigNode::InitFromXMLStr(str);
  CHECK(config.get());
  
  // delegate and views must be create by "new"
  contents_ = new WindowContents(config.get());
  widget_ = new views::Widget;
  views::Widget::InitParams wparams;
  wparams.parent = NULL;
  wparams.type = views::Widget::InitParams::TYPE_WINDOW;
  wparams.delegate = contents_;
  wparams.context  = NULL;
  wparams.bounds = gfx::Rect(0, 0, 800, 600);
  widget_->Init(wparams);
  widget_->SetFullscreen(true);
  contents_->RequestFocus();

  views::FocusManager* fmgr = widget_->GetFocusManager();
  DCHECK(fmgr);
}

void DrawingWindow::Show() {
  widget_->Show();
}
