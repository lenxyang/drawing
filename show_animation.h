#pragma once

#include "ui/views/controls/image_view.h"
#include "ui/views/view.h"
#include "ui/views/animation/bounds_animator.h"
#include "ui/views/animation/bounds_animator_observer.h"
#include "nelf/controls/image_view.h"

class PictureImageView;
class ConfigNode;
class ImageAnimation;
typedef scoped_refptr<ImageAnimation> ImageAnimationPtr;
typedef std::vector<ImageAnimationPtr> ImageAnimationPtrs;

class ImageAnimation : public base::RefCounted<ImageAnimation> {
 public:
  ImageAnimation(views::View* parent, ConfigNode* node);

  void Start();
  void Reset(bool b);
 private:
  PictureImageView* image_;
  scoped_ptr<views::BoundsAnimator> animator_;
  int64 duration_;
  gfx::Rect init_bounds_;
  gfx::Rect target_bounds_;
  DISALLOW_COPY_AND_ASSIGN(ImageAnimation);
};

class ShowAnimationView : public views::View,
                          public views::BoundsAnimatorObserver {
 public:
  ShowAnimationView(ConfigNode* node);
  ~ShowAnimationView();

  void Start();
  void Reset();
  void OnBoundsAnimatorDone(views::BoundsAnimator* animator) override;
  void OnBoundsAnimatorProgressed(views::BoundsAnimator* animator) override;
 private:
  void NextFrame();
  void InitAnimation(ConfigNode* node, PictureImageView* view);
  std::vector<ImageAnimationPtrs> animations_;
  int32 current_frame_;
  int32 completed_;
  DISALLOW_COPY_AND_ASSIGN(ShowAnimationView);
};
