/**
 * \file OstreamCommandObserver.hpp
 * \brief The command observer using standard std::ostream for notify.
 */

#ifndef MT4CPP_OSTREAM_COMMAND_OBSERVER_HPP
#define MT4CPP_OSTREAM_COMMAND_OBSERVER_HPP

#include <boost/thread.hpp>
#include <ostream>

#include "Command.hpp"

namespace mt4cpp {

  /** simple command observer, send notification to ostream */
  class OstreamCommandObserver : public CommandObserver {
  public:
    OstreamCommandObserver(std::ostream& out) : out_(out) {}
    virtual ~OstreamCommandObserver() {}

    /** new progress */
    virtual void notifyProgress(const Command&, double progress) {
      boost::mutex::scoped_lock scoped_lock(out_mutex);
      out_ << static_cast<int>(progress * 100) << '%' << std::endl;
    }

    /** next step */
    virtual void notifyStep(const Command&) {
      boost::mutex::scoped_lock scoped_lock(out_mutex);
      out_ << '.' << std::flush;
    }

    /** change command state */
    virtual void notifyState(const Command&, CommandDesc::State) {}

  private:
    OstreamCommandObserver& operator=(const OstreamCommandObserver&); //!< not allowed

    boost::mutex out_mutex;
    std::ostream& out_;
  };

} // namespace mt4cpp

#endif // MT4CPP_OSTREAM_COMMAND_OBSERVER_HPP
