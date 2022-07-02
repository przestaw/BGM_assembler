//
// Created by przestaw on 20.10.2020.
//

#ifndef BGM_ASM_OBSERVER_H
#define BGM_ASM_OBSERVER_H

#include <mt4cpp.h>

namespace bgm {
  /**
   * Observer printing progress and state updates of assembly tasks to stdout
   */
  class Observer : public mt4cpp::CommandObserver {
  public:
    /**
     * Notifies progress of running command
     * @param cmd observed command
     * @param progress progress value in range [0.0, 1.0]
     */
    void notifyProgress(const mt4cpp::Command& cmd, double progress) override;

    /**
     * Notifies completion of a command step
     * @param cmd observed command
     */
    void notifyStep(const mt4cpp::Command& cmd) override;

    /**
     * Notifies state change of running command
     * @param cmd observed command
     * @param state new command state
     */
    void notifyState(const mt4cpp::Command& cmd,
                     mt4cpp::CommandDesc::State state) override;

  private:
    boost::mutex outMutex;
  };
} // namespace bgm

#endif // BGM_ASM_OBSERVER_H
