#include "drawing/drawing.h"

#include <random>
#include <algorithm>
#include <set>

#include "base/logging.h"
#include "base/files/file_util.h"
#include "base/strings/stringprintf.h"
#include "base/strings/string_split.h"
#include "base/strings/string_util.h"
#include "base/strings/utf_string_conversions.h"

namespace {
bool ReadFileToLines(const base::FilePath& path, std::vector<std::string>* lines) {
  std::string str;
  if (!::base::ReadFileToString(path, &str)) {
    LOG(ERROR) << "Failed to read file: " << path.value();
    return false;
  }

  std::vector<std::string> vec;
  ::base::SplitStringUsingSubstr(str, std::string("\r\n"), &vec);
  for (auto iter = vec.begin(); iter != vec.end(); ++iter) {
    std::string str;
    ::base::TrimWhitespaceASCII(*iter, ::base::TRIM_ALL, &str);
    if (!str.empty()) {
      lines->push_back(str);
    }
  }
  return true;
}

bool ReadFileToSetsAppend(const base::FilePath& path, std::set<std::string>* set) {
  std::vector<std::string> lines;
  if (!ReadFileToLines(path, &lines)) {
    return false;
  }

  for (auto iter = lines.begin(); iter != lines.end(); ++iter) {
    std::string str;
    ::base::TrimWhitespaceASCII(*iter, ::base::TRIM_ALL, &str);
    if (str.empty()) {
      set->insert(str);
    }
  }
  return true;
}
}

Drawing::Drawing() {}
void Drawing::Shuff(int32 count, std::vector<std::string>* ret) {
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(list_.begin(), list_.end(), g);

  ret->clear();
  for (int32 i = 0; i < count; ++i) {
    ret->push_back(list_[i]);
  }
}

bool Drawing::InitDrawingList(const ::base::FilePath& dir, int32 round) {
  list_.clear();
  std::set<std::string> sets;
  for (int32 i = 0; i < round; ++i) {
  std::string filename = base::StringPrintf("%d.txt", round);
    ::base::FilePath path = dir.Append(::base::UTF8ToUTF16(filename));
    if (base::PathExists(path)) {
      CHECK(ReadFileToSetsAppend(path, &sets));
    }
  }

  std::vector<std::string> lines;
  std::string filename = base::StringPrintf("all.txt");
  ::base::FilePath path = dir.Append(::base::UTF8ToUTF16(filename));
  CHECK(ReadFileToLines(path, &lines));
  for (auto iter = lines.begin(); iter != lines.end(); ++iter) {
    std::string item;
    ::base::TrimWhitespaceASCII(*iter, ::base::TRIM_ALL, &item);
    if (item.empty()) {
      continue;
    }

    if (sets.end() != sets.find(item)) {
      continue;
    }

    list_.push_back(item);
  }
  return true;
}
