#pragma once

#include <vector>
#include "base/files/file_path.h"


class Drawing {
 public:
  Drawing();
  
  bool InitDrawingList(const ::base::FilePath& list, int32 round);
  void Shuff(int32 count, std::vector<std::string>* ret);
 private:
  std::vector<std::string> list_;
  DISALLOW_COPY_AND_ASSIGN(Drawing);
};
