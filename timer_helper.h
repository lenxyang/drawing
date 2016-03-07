#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/bind.h"
#include "base/time/time.h"
#include "base/timer/timer.h"

class TimerHelper : public ::base::RefCounted<TimerHelper> {
 public:
  class Delegate {
   public:
    virtual void OnTimer() = 0;
  };

  TimerHelper(base::TimeDelta timedelta, bool repeat, Delegate* delegate);

  void Reset();
  void Recreate();
  void Stop();
 private:
  void OnTimer();
  base::TimeDelta timedelta_;
  bool repeat_;
  Delegate* delegate_;
  scoped_ptr<base::Timer> timer_;
  DISALLOW_COPY_AND_ASSIGN(TimerHelper);
};

