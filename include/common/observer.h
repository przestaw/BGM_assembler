//
// Created by przestaw on 20.10.2020.
//

#ifndef BGM_ASM_OBSERVER_H
#define BGM_ASM_OBSERVER_H

#include <mt4cpp.h>

namespace bgm {
  class Observer : public mt4cpp::CommandObserver {
  public:
    void notifyProgress(const mt4cpp::Command& cmd, double progress) override;
    void notifyStep(const mt4cpp::Command& cmd) override;
    void notifyState(const mt4cpp::Command& cmd,
                     mt4cpp::CommandDesc::State state) override;

  private:
    boost::mutex outMutex;
  };
} // namespace bgm

#endif // BGM_ASM_OBSERVER_H
