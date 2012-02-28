#ifndef FOUG_ABSTRACT_TASK_PROGRESS_H
#define FOUG_ABSTRACT_TASK_PROGRESS_H

namespace foug {

namespace internal { class AbstractTaskProgressPrivate; }

class AbstractTaskProgress
{
public:
  AbstractTaskProgress();
  virtual ~AbstractTaskProgress();

  double rangeMin() const;
  double rangeMax() const;
  void setRange(double min, double max);

  int stepId() const;
  void setStepId(int id);

  double progress() const;
  double value() const;
  void setValue(double v);

  double progressUpdateThreshold() const;
  void setProgressUpdateThreshold(double v);

  virtual bool isStopRequested() const;
  virtual void handleProgressUpdate() = 0;
  virtual void reset();

private:
  internal::AbstractTaskProgressPrivate* const d;
};

} // namespace foug

#endif // FOUG_ABSTRACT_TASK_PROGRESS_H
