#include "drawing/show_animation.h"

#include "base/files/file_path.h"
#include "base/files/file_util.h"
#include "base/strings/utf_string_conversions.h"
#include "base/strings/string_number_conversions.h"
#include "ui/views/layout/fill_layout.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/image/image.h"
#include "drawing/config_node.h"

using base::UTF8ToUTF16;

class PictureImageView : public views::View {
 public:
  PictureImageView() {}
  ~PictureImageView() {}
  void SetImage(const gfx::ImageSkia* img) { image_ = *img;}
  void OnPaint(gfx::Canvas* canvas) override {
    gfx::Rect bounds = GetContentsBounds();
    SkPaint paint;
    paint.setFilterLevel(SkPaint::kHigh_FilterLevel);
    canvas->DrawImageInt(image_, 0, 0, image_.width(), image_.height(),
        bounds.x(), bounds.y(), bounds.width(), bounds.height(), true, paint);
  }
 private:
  gfx::ImageSkia image_;
  DISALLOW_COPY_AND_ASSIGN(PictureImageView);
};

ImageAnimation::ImageAnimation(views::View* parent, ConfigNode* node) {
  std::string contents;
  ::base::FilePath path(UTF8ToUTF16(node->GetChildTextString("path")));
  CHECK(::base::ReadFileToString(path, &contents));
  gfx::Image img = gfx::Image::CreateFrom1xPNGBytes(
      (uint8*)contents.c_str(), contents.length());
  const gfx::ImageSkia* imgskia = img.ToImageSkia();
  image_->SetImage(imgskia);
  parent->AddChildView(image_);

  init_bounds_ = StringToRect(node->GetChildTextString("init_bounds"));
  target_bounds_ = StringToRect(node->GetChildTextString("target_bounds"));
  CHECK(::base::StringToInt64(node->GetChildTextString("duration"), &duration_));
  animator_.reset(new views::BoundsAnimator(parent));
  animator_->SetAnimationDuration(duration_);
}

void ImageAnimation::Start() {
  image_->SetBoundsRect(init_bounds_);
  image_->SetVisible(true);
  animator_->AnimateViewTo(image_, target_bounds_);
}

void ImageAnimation::Reset(bool visible) {
  image_->SetBoundsRect(init_bounds_);
  image_->SetVisible(visible);
}

// class ShowAnimationView
ShowAnimationView::ShowAnimationView(ConfigNode* node) {
  int64 frame_count = 0;
  CHECK(::base::StringToInt64(node->GetAttr("frame_count"), &frame_count));
  animations_.resize(frame_count);
  ConfigNodes nodes = node->children();
  for (auto iter = nodes.begin(); iter != nodes.end(); ++iter) {
    ConfigNode* child = iter->get();
    DCHECK(child->tagname() == "animation");
    int64 id;
    CHECK(::base::StringToInt64(node->GetAttr("frame_count"), &id));
    ImageAnimationPtr anim(new ImageAnimation(this, child));
    animations_[id].push_back(anim);
  }
}

ShowAnimationView::~ShowAnimationView() {
}

void ShowAnimationView::Start() {
  current_frame_ = -1;
  NextFrame();
}

void ShowAnimationView::Reset() {
  for (uint32 frame = 0; frame < animations_.size(); ++frame) {
    for (auto iter = animations_[frame].begin();
         iter != animations_[frame].end(); ++iter) {
      (*iter)->Reset(false);
    }
  }
}

void ShowAnimationView::NextFrame() {
  current_frame_++;
  completed_++;
  for (auto iter = animations_[current_frame_].begin();
       iter != animations_[current_frame_].end(); ++iter) {
    (*iter)->Start();
  }
}

void ShowAnimationView::OnBoundsAnimatorDone(views::BoundsAnimator* animator) {
  if (++completed_ == static_cast<int32>(animations_[current_frame_].size())) {
    if (current_frame_ < animations_.size() - 1) {
      NextFrame();
    }
  }
}

void ShowAnimationView::OnBoundsAnimatorProgressed(views::BoundsAnimator* animator) {
}
