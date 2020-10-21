//
// Created by przestaw on 20.10.2020.
//

#ifndef BGM_ASM_OBSERVER_H
#define BGM_ASM_OBSERVER_H

#include "../../external/mt4cpp/Command.hpp"
#include <mt4cpp.h>

namespace bgm {
  class Observer : public mt4cpp::CommandObserver {
  public:
    void notifyProgress(const mt4cpp::Command& cmd, double progress);
    void notifyStep(const mt4cpp::Command& cmd);
    void notifyState(const mt4cpp::Command&, mt4cpp::CommandDesc::State state);
  };
} // namespace bgm

#endif // BGM_ASM_OBSERVER_H
