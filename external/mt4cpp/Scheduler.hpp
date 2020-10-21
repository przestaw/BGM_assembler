/**
 * \file Scheduler.hpp
 * \brief The active object scheduler module (Scheduler, CommandQueue and
 * working thread main function)
 */

#ifndef MT4CPP_SHEDULER_HPP
#define MT4CPP_SHEDULER_HPP

#include <queue>

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>

#include "Command.hpp"

namespace mt4cpp {

  /* The command queue used by scheduler. Queue methods are thread-safe.*/
  class CommandQueue : boost::noncopyable {
  public:
    /** constructor */
    CommandQueue() : quitFlag_(false), currentCommandId_(0L) {}
    ~CommandQueue() {}

    /** read the command to execute. Suspend the thread if the queue is empty.
        Returns not empty optional if the queue is destroyed. */
    PCommand read() {
      boost::mutex::scoped_lock lock(m_);
      while (queue_.empty()) {
        cond.wait(lock); // wait if the empty queue

        if (quitFlag_)       // if resumed because the quitFlag is set
          return PCommand(); // empty command means finalize queue and finish
                             // thread
      }
      // here non-empty queue
      PCommand f = queue_.front();
      queue_.pop();
      return f;
    }

    /** write the command into queue, returns the new command ID */
    CommandID write(PCommand cmd) {
      // unique_lock<boost::mutex> lock(m_);
      // to samo ale krocej zapisane
      boost::mutex::scoped_lock lock(m_);

      queue_.push(cmd);
      CommandID command_id = ++currentCommandId_;
      cmd->setId(command_id);
      cmd->setState(CommandDesc::QUEUED);
      cond.notify_all();
      return command_id;
    }

    /** accessor */
    bool getQuitFlag() const {
      boost::mutex::scoped_lock lock(m_);
      return quitFlag_;
    }
    /** mutator */
    void setQuitFlag() {
      boost::mutex::scoped_lock lock(m_);
      quitFlag_ = true;
      cond.notify_all(); // notify all waiting thread that the quit flag is set
    }

    long generateCommandId() {
      boost::mutex::scoped_lock lock(m_);
      return ++currentCommandId_;
    }

  private:
    std::queue<PCommand> queue_;
    mutable boost::mutex m_;
    /** the variable to notify if the queue is not empty */
    boost::condition_variable cond;

    bool quitFlag_;

    /** the ID for the next command */
    CommandID currentCommandId_;
  };

  /**
   *  Sheduler, the execution queue with and thread pool.
   *  it executes commands (synchronically or asynchronically)
   */
  class Scheduler : boost::noncopyable {
  public:
    Scheduler(int); // the constructor
    ~Scheduler();   // the destructor
    /**
       Executes the command in one of threads from threads pool. Do not break
       execution of the calling thread. \return  CommandId of command
    */
    CommandID executeAsynchronously(PCommand cmd);

    /**
       Executes the command in one of threads from threads pool.
       Command is inserted into command queue, it waits till free working thread
       is found, then it is executed. The calling thread waits (on condition
       variable) till the command is finished. \return  CommandId of command.
    */
    CommandID executeSynchronously(PCommand cmd);

    /** wait for ending threads in pool */
    void join();

    /** signal to finish all threads after current command and wait for ending
     * threads in pool */
    void finishAndJoin();

  private:
    int num_;
    /** the activation queue. Contains every commands executed asynchronically
     */
    CommandQueue queue_;

    typedef std::list<boost::thread*> Threads;
    Threads threads_;        // pool of threads
    mutable boost::mutex m_; // access to threads_

    Scheduler(const Scheduler&);            // noncopyable
    Scheduler& operator=(const Scheduler&); // noncopyable
  };
} // namespace mt4cpp

//-------------------------------------------------------------------------------------------
//
// implementation
//
//-------------------------------------------------------------------------------------------

namespace mt4cpp {

  namespace {

    /** the function which run in thread in scheduler thread pools.
        The main loops check if there is any command in queue, if so it executes
       one of them. When quitFlag is set in queue finish the execution
    */
    void threadFunction(CommandQueue& queue) {
      while (!queue.getQuitFlag()) { // check if the thread should finish execution
        PCommand cmd = queue.read(); // reads command from queue,
        // it can block the thread until the queue is not empty or until the
        // queue is destroyed

        if (!cmd)
          break; // command object is null - finish the thread
        cmd->execute();
      }
    }
  } // namespace

  // constructor
  inline Scheduler::Scheduler(int num_threads) : num_(num_threads) {

    for (int i = 0; i < num_; ++i) { // starts NUM_THREADS threads
      boost::lock_guard<boost::mutex> guard(m_);
      boost::thread* new_thread_ptr =
          new boost::thread(boost::bind(threadFunction, boost::ref(queue_)));
      // std::auto_ptr<boost::thread> new_thread());
      threads_.push_back(new_thread_ptr);
      // new_thread.release();
    }
  }

  // descructor
  inline Scheduler::~Scheduler() {
    finishAndJoin(); // signal to finish all threads and wait (join) fo them
    boost::lock_guard<boost::mutex> guard(m_);
    for (Threads::iterator it = threads_.begin(), end = threads_.end(); it != end; ++it) {
      delete *it;
    }
  }

  /**
     asynchronically executes the command. Do not break execution of the calling
     thread.
  */
  inline CommandID Scheduler::executeAsynchronously(PCommand cmd) {
    return queue_.write(cmd);
  }

  namespace {

    struct FinishObserver : public CommandObserver {

      FinishObserver() : done(false) {}

      virtual void notifyProgress(const Command&, double) {}
      virtual void notifyStep(const Command&) {}
      virtual void notifyState(const Command&, CommandDesc::State s) {
        if (s == CommandDesc::DONE || s == CommandDesc::INTERRUPTED
            || s == CommandDesc::EXCEPTION) {
          boost::mutex::scoped_lock lock(mut);
          done = true;
          cond.notify_one();
        }
      }
      boost::mutex mut;
      boost::condition_variable cond;
      bool done;
    };

  } // namespace

  /**
     asynchronically executes the command. Command is inserted into command
     queue, it waits till free working thread is found, then it is executed. The
     calling thread waits (on condition variable) untill the command is not
     finished.
  */
  inline CommandID Scheduler::executeSynchronously(PCommand cmd) {
    FinishObserver* obs = new FinishObserver();
    cmd->attach(PCommandObserver(obs));

    CommandID command_id = executeAsynchronously(cmd);

    boost::mutex::scoped_lock lock(obs->mut);
    while (!obs->done) { obs->cond.wait(lock); }
    return command_id;
  }

  /** wait for ending threads in pool */
  inline void Scheduler::join() {
    boost::lock_guard<boost::mutex> guard(m_);
    for (Threads::iterator it = threads_.begin(), end = threads_.end(); it != end; ++it) {
      if ((*it)->joinable())
        (*it)->join();
    }
  }

  /** signal to finish all threads after current command, and wait for endint
   * the threads in pool */
  inline void Scheduler::finishAndJoin() {
    queue_.setQuitFlag();
    join();
  }

} // namespace mt4cpp

#endif // MT4CPP_SHEDULER_HPP
