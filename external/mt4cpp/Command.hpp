/**
 * \file Command.hpp
 * \brief The command module (Command base class, progress, command
 * description).
 */

#ifndef MT4CPP_COMMAND_HPP
#define MT4CPP_COMMAND_HPP

#include <boost/noncopyable.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
// pass not-fully initialized pointer in Command c-tor (pointer of this) in
// progress_ member Command() : counter_(0), haltFlag_(false), progress_(*this)
// this warning is useless here
#pragma warning(disable : 4355)
#endif

namespace mt4cpp {

  /** Command unique ID indentifier */
  typedef long CommandID;

  /** \brief the descriptor of command */
  struct CommandDesc {

    /** \brief available states of Command
        NONE - command created, but not put in activation queue
        QUEUED - command is waiting for execusion at the activation queue
        PENDING - command is being executed now
        INTERRUPTED - command execution has been interrupted
        EXCEPTION - during command execution an exception was catched
        DONE - command code has been done successfully
    */
    enum State { NONE, QUEUED, PENDING, INTERRUPTED, EXCEPTION, DONE };

    CommandID id_;    //!< command unique ID
    State state_;     //!< command state
    double progress_; //!< command progress

    /** c-tor */
    CommandDesc() : id_(0), state_(NONE), progress_(0.0) {}
  };

  /** forward declaration */
  class Command;

  /** abstract class of command observer */
  class CommandObserver {
  public:
    CommandObserver() = default;
    /** new progress */
    virtual void notifyProgress(const Command&, double progress) = 0;

    /** next step */
    virtual void notifyStep(const Command&) = 0;

    /** change command state */
    virtual void notifyState(const Command&, CommandDesc::State state) = 0;

    virtual ~CommandObserver() = default;
  };

  /** smart pointer to observer objects */
  typedef boost::shared_ptr<CommandObserver> PCommandObserver;

  /*! \brief
    class to set the progress of given command. Subject in Obverver design
    pattern. It also check the 'halt-command' flag and if it is set it throws
    exception UserBreakException.
  */
  class Progress {
  public:
    // constructor, progress from 0 to 100% for given command
    explicit Progress(Command& cmd);
    // copy constructor
    Progress(const Progress& p) = default;

    /** creates sub-progress, the notification of the same observers,
        but the output progress is recalulated (0% is the 'st', 100% is the
       'fi') */
    Progress(const Progress& parent, double st, double fi)
        : command_(parent.command_), observers_(parent.observers_) {
      double wsp = parent.finish_ - parent.start_;
      start_     = parent.start_ + st * wsp;
      finish_    = parent.start_ + fi * wsp;
    }

    /* \brief change the command progress (stored in
       Command::CommandDescription) Notify all observers about the progress. The
       parameter current from 0 to 1 (100%) is recalculated to notify about
       progress from 'start' to 'finish'.
    */
    void setProgress(double current);

    //! notify all observers, increase the progress
    void step();

    /* \brief notify all observers about the new state of command
     */
    void changeState(CommandDesc::State state) {
      std::for_each(
          observers_.begin(),
          observers_.end(),
          boost::bind(&CommandObserver::notifyState, _1, boost::cref(command_), state));
    }

    //! add observer
    void attach(PCommandObserver observer) { observers_.push_back(observer); }

  private:
    Progress& operator=(const Progress& prog); //<! assign not allowed

    Command& command_; //!< command for which the progress is calculated

    std::vector<PCommandObserver> observers_; //<! observers

    double start_;  //<! starting progress
    double finish_; //<! finishing progress
  };

  /** The basic class of command, which is executed by scheduler. */
  class Command : boost::noncopyable {
  public:
    Command() : counter_(0), haltFlag_(false), progress_(*this) {}
    /** destructor */
    virtual ~Command() {}

    /** the method called by scheduler. It calls the operator() of command. */
    void execute() {
      try {
        setState(CommandDesc::PENDING);
        operator()(progress_);
        setState(CommandDesc::DONE);
      } catch (boost::thread_interrupted&) {
        setState(CommandDesc::INTERRUPTED);
      } catch (...) { setState(CommandDesc::EXCEPTION); }
    }

    /** returns the command actual state and progress  */
    CommandDesc getDescriptor() const {
      boost::mutex::scoped_lock lock(m_);
      return CommandDesc(descriptor_);
    }

    /** mutator - change the command progress (called by Progress::setProgress()
     * )*/
    void setProgress(double new_progress) {
      boost::mutex::scoped_lock lock(m_);
      descriptor_.progress_ = new_progress;
    }

    /** mutator - change the command state (called by Scheduler ) */
    void setState(CommandDesc::State new_state) {
      {
        boost::mutex::scoped_lock lock(m_);
        descriptor_.state_ = new_state;
      }
      progress_.changeState(new_state);
    }

    /** mutator - change the commmand id (called by Scheduler ) */
    void setId(long new_id) {
      boost::mutex::scoped_lock lock(m_);
      descriptor_.id_ = new_id;
    }

    /** Suggest the command to break execution (ie sets the flags) */
    void halt() { haltFlag_ = true; }

    /** Check the halt flag and throws the exception if set */
    void checkHaltFlag() const {
      if (haltFlag_)
        throw boost::thread_interrupted();
    }

    /** the accessor to the counter */
    int getCounter() const { return counter_; }

    /** add observer (add observer to command) */
    void attach(PCommandObserver observer) { progress_.attach(observer); }

    /** for intrusive_ptr */
    friend void intrusive_ptr_add_ref(Command* cmd);
    friend void intrusive_ptr_release(Command* cmd);

  protected:
    mutable boost::mutex m_;

  private:
    /** the method which implements the specific task of command.
        The private members can be overlapped */
    virtual void operator()(Progress& p) = 0;

    CommandDesc descriptor_; //!< the command state, progress and id

    int counter_;            /** counter for intrusive ptr */
    volatile bool haltFlag_; //!< flag set if the command should stop its execution

    Progress progress_; //!< progress for given command
  };

  /** read the command state (helper) */
  inline CommandDesc::State getState(const Command& cmd) {
    return cmd.getDescriptor().state_;
  }

  inline void intrusive_ptr_add_ref(Command* cmd) {
    boost::mutex::scoped_lock lock(cmd->m_); // critical section
    ++(cmd->counter_);
  }
  inline void intrusive_ptr_release(Command* cmd) {
    bool del = false;
    {
      boost::mutex::scoped_lock lock(cmd->m_); // critical section
      del = !--(cmd->counter_);
    }
    if (del)
      delete cmd;
  }

  /** the smart pointer to command.
      intrusive_ptr because of multithread env (mutex blocks the counter )
  */
  typedef boost::intrusive_ptr<Command> PCommand;

  /**
     \brief holder with the type and pointer to command as well as the base
     class of command (smart pointer)
  */
  template <typename T> class CommandHolder {
  public:
    //! the command type
    typedef T CommandType;

    explicit CommandHolder(T* cmd) : obsCmd_(cmd), cmd_(cmd) {}

    T* getObsCmd() { return obsCmd_; }
    PCommand get() { return cmd_; }

  private:
    T* obsCmd_;
    PCommand cmd_;
  };

} // namespace mt4cpp

//-------------------------------------------------------------------------------------------
//
// implementation
//
//-------------------------------------------------------------------------------------------

namespace mt4cpp {

  // constructor, progress from 0 to 100% for given command
  inline Progress::Progress(Command& cmd) : command_(cmd), start_(0), finish_(1) {}

  /* \brief change the command progress (stored in Command::CommandDescription)
     Notify all observers about the progress.
     The parameter current from 0 to 1 (100%) is recalculated to notify about
     progress from 'start' to 'finish'.
  */
  inline void Progress::setProgress(double current) {
    command_.checkHaltFlag();
    command_.setProgress(start_ + (finish_ - start_) * current);
    std::for_each(observers_.begin(),
                  observers_.end(),
                  boost::bind(&CommandObserver::notifyProgress,
                              _1,
                              boost::cref(command_),
                              start_ + (finish_ - start_) * current));
  }

  //! notify all observers, increase the progress
  inline void Progress::step() {
    command_.checkHaltFlag();
    std::for_each(observers_.begin(),
                  observers_.end(),
                  boost::bind(&CommandObserver::notifyStep, _1, boost::cref(command_)));
  }

} // namespace mt4cpp

#endif // MT4CPP_COMMAND_HPP
