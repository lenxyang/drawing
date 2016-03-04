#include <memory>

#include "nelf/nelf.h"
#include "drawing/drawing_window.h"
                          
using views::Widget;
int main(int argc, char* argv[]) {
  scoped_refptr<nelf::Context> context(
      nelf::Context::CreateStandAloneContext(argc, argv));
  CHECK(context.get());

  DrawingWindow window;
  window.Show();
  base::RunLoop().Run();
  return 0;
}
