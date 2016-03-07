#pragma once

#include "base/observer_list.h"
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
  void AddObserver(views::BoundsAnimatorObserver* obsever);
  void RemoveObserver(views::BoundsAnimatorObserver* obsever);
 private:
  PictureImageView* image_;
  scoped_ptr<views::BoundsAnimator> animator_;
  int64 duration_;
  float init_bounds_[4];
  float target_bounds_[4];
  DISALLOW_COPY_AND_ASSIGN(ImageAnimation);
};

class ShowAnimationView;
class ShowAnimationViewObserver {
 public:
  virtual void OnBoundsAnimatorDone(ShowAnimationView* view) {}
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
  void AddObserver(ShowAnimationViewObserver* observer);
  void RemoveObserver(ShowAnimationViewObserver* observer);
 private:
  void NextFrame();
  void InitAnimation(ConfigNode* node, PictureImageView* view);
  std::vector<ImageAnimationPtrs> animations_;
  int32 current_frame_;
  int32 completed_;
  ObserverList<ShowAnimationViewObserver> observers_;
  DISALLOW_COPY_AND_ASSIGN(ShowAnimationView);
};
