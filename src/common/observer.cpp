//
// Created by przestaw on 20.10.2020.
//
#include <common/observer.h>
#include <iostream>

using namespace bgm;
using namespace mt4cpp;

void Observer::notifyProgress(const Command& cmd, double progress) {
  boost::mutex::scoped_lock scoped_lock(outMutex);
  const int barWidth = 40;

  std::cout << "[";
  int pos = static_cast<int>(progress * barWidth);
  for (int i = 0; i < barWidth; ++i) {
    if (i < pos)
      std::cout << "=";
    else if (i == pos)
      std::cout << ">";
    else
      std::cout << " ";
  }

  std::cout << "] " << static_cast<int>(progress * 100.) << " %\r";
  std::cout.flush();
}

void Observer::notifyStep(const Command& cmd) {
  boost::mutex::scoped_lock scoped_lock(outMutex);
  std::cout << '#' << std::flush;
}

void Observer::notifyState(const Command&, mt4cpp::CommandDesc::State) {}
