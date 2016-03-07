#include "drawing/timer_helper.h"

#include "base/bind.h"
#include "base/bind_helpers.h"
#include "base/callback.h"
#include "base/location.h"
#include "base/time/time.h"

TimerHelper::TimerHelper(base::TimeDelta time, bool repeat, Delegate* delegate)
    : timedelta_(time),
      repeat_(repeat),
      delegate_(delegate) {
  Recreate();
}

void TimerHelper::Stop() {
  timer_->Stop();
}

void TimerHelper::Reset() {
  timer_->Reset();
}

void TimerHelper::OnTimer() {
  delegate_->OnTimer();
}

void TimerHelper::Recreate() {
  timer_.reset(new base::Timer(true, repeat_));
  ::base::Callback<void()> task = ::base::Bind(&TimerHelper::OnTimer, this);
  timer_->Start(FROM_HERE, timedelta_, task);
}
