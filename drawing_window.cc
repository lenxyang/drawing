#include "drawing/drawing_window.h"

#include "base/files/file_util.h"
#include "base/strings/string_split.h"
#include "base/strings/stringprintf.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/utf_string_conversions.h"
#include "ui/views/background.h"
#include "ui/views/color_constants.h"
#include "ui/views/widget/widget.h"
#include "ui/gfx/font_list.h"
#include "drawing/drawing.h"
#include "azer/util/xml/xml.h"
#include "drawing/show_animation.h"
#include "drawing/config_node.h"
#include "drawing/drawing_view.h"

using base::UTF8ToUTF16;


DrawingWindow::DrawingWindow(const ::base::FilePath& path, int32 round)
    : state_(kInit) {
  SetFocusable(true);

  std::string str;
  CHECK(::base::ReadFileToString(path, &str));
  ConfigNodePtr config = ConfigNode::InitFromXMLStr(str);
  CHECK(config.get());

  drawing_view_ = new DrawingView(config.get());
  AddChildView(drawing_view_);

  ConfigNode* cfg = config->GetNodeFromPath("//draw_window");
  SkColor bgcol = StringToColor(cfg->GetChildTextString("background"));
  set_background(views::Background::CreateSolidBackground(bgcol));

  std::string roundname = ::base::StringPrintf("//rounds/round%d", round);
  std::string animname = ::base::StringPrintf("//rounds/round%d/animations", round);
  ConfigNode* animcfg = config->GetNodeFromPath(animname);
  animation_ = new ShowAnimationView(animcfg);
  AddChildView(animation_);
}

void DrawingWindow::WindowClosing() {
}

void DrawingWindow::Layout() {
  drawing_view_->SetBoundsRect(GetContentsBounds());
  animation_->SetBoundsRect(GetContentsBounds());
}


base::string16 DrawingWindow::GetWindowTitle() const {
  return base::ASCIIToUTF16("Drawing");
}

bool DrawingWindow::OnKeyPressed(const ui::KeyEvent& event) {
  return true;
}

bool DrawingWindow::OnKeyReleased(const ui::KeyEvent& event) {
  if (state_ == kEndDrawing || state_ == kInit) {
    if (event.key_code() == ui::VKEY_ESCAPE)
      GetWidget()->Close();
  }

  if (event.key_code() == ui::VKEY_RETURN
      || event.key_code() == ui::VKEY_SPACE) {
    if (state_ == kInit) {
      state_ = kShowAnimation;
      animation_->Start();
    } else if (state_ == kStartDrawing) {
    } else {
    }
  }

  return true;
}

void DrawingWindow::ShowWindow() {
  // delegate and views must be create by "new"
  views::Widget* widget = new views::Widget;
  views::Widget::InitParams wparams;
  wparams.parent = NULL;
  wparams.type = views::Widget::InitParams::TYPE_WINDOW;
  wparams.delegate = this;
  wparams.context  = NULL;
  wparams.bounds = gfx::Rect(0, 0, 800, 600);
  widget->Init(wparams);
  widget->SetFullscreen(true);
  RequestFocus();

  views::FocusManager* fmgr = widget->GetFocusManager();
  DCHECK(fmgr);

  widget->Show();
}

