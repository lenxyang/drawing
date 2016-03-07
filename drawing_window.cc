#include "drawing/drawing_window.h"

#include "base/files/file_util.h"
#include "base/strings/string_split.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/utf_string_conversions.h"
#include "ui/views/background.h"
#include "ui/views/color_constants.h"
#include "ui/views/widget/widget.h"
#include "ui/gfx/font_list.h"
#include "drawing/drawing.h"
#include "azer/util/xml/xml.h"
#include "drawing/config_node.h"
#include "drawing/drawing_view.h"

using base::UTF8ToUTF16;


DrawingWindow::DrawingWindow(const ::base::FilePath& path) {
  SetFocusable(true);

  std::string str;
  CHECK(::base::ReadFileToString(path, &str));
  ConfigNodePtr config = ConfigNode::InitFromXMLStr(str);
  CHECK(config.get());

  drawing_view_ = new DrawingView(config.get());
  AddChildView(drawing_view_);
}

void DrawingWindow::WindowClosing() {
}


base::string16 DrawingWindow::GetWindowTitle() const {
  return base::ASCIIToUTF16("Drawing");
}

bool DrawingWindow::OnKeyPressed(const ui::KeyEvent& event) {
  return true;
}

bool DrawingWindow::OnKeyReleased(const ui::KeyEvent& event) {
  if (event.key_code() == ui::VKEY_ESCAPE)
    GetWidget()->Close();
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

