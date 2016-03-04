#include <memory>

#include "base/files/file_path.h"
#include "base/strings/utf_string_conversions.h"
#include "nelf/nelf.h"
#include "drawing/drawing_window.h"
                          
using views::Widget;
using base::FilePath;
using base::UTF8ToUTF16;

int main(int argc, char* argv[]) {
  scoped_refptr<nelf::Context> context(
      nelf::Context::CreateStandAloneContext(argc, argv));
  CHECK(context.get());

  DrawingWindow window;
  window.Init(::base::FilePath(UTF8ToUTF16("drawing/drawing.xml")));
  window.Show();
  base::RunLoop().Run();
  return 0;
}
