//  MeCab -- Yet Another Part-of-Speech and Morphological Analyzer
//
//
//  Copyright(C) 2001-2006 Taku Kudo <taku@chasen.org>
//  Copyright(C) 2004-2006 Nippon Telegraph and Telephone Corporation
#ifndef MECAB_THREAD_H
#define MECAB_THREAD_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#else
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX 1
#endif
#include <windows.h>
#include <process.h>
#endif
#endif

#if defined HAVE_CXX11_ATOMIC_OPS
#  include <thread>
#  include <atomic>
#elif defined HAVE_OSX_ATOMIC_OPS
#  include <libkern/OSAtomic.h>
#  include <sched.h>
#elif defined HAVE_GCC_ATOMIC_OPS
#  include <sched.h>
#endif

#if defined HAVE_PTHREAD_H
#define MECAB_USE_THREAD 1
#endif

#if (defined(_WIN32) && !defined(__CYGWIN__))
#define MECAB_USE_THREAD 1
#define BEGINTHREAD(src, stack, func, arg, flag, id)                    \
  (HANDLE)_beginthreadex((void *)(src), (unsigned)(stack),              \
                         (unsigned(_stdcall *)(void *))(func), (void *)(arg), \
                         (unsigned)(flag), (unsigned *)(id))
#endif

namespace MeCab {

#if defined(HAVE_CXX11_ATOMIC_OPS)
class atomic_int {
  public:
    atomic_int(): val_(0) {}
    atomic_int(int init): val_(init) {}
    int add(int a) {
      return val_.fetch_add(a);
    }
    bool compare_and_swap(int a, int b) {
      return val_.compare_exchange_strong(a, b);
    }
    int load() {
      return val_.load();
    }
  private:
    std::atomic<int> val_;
};
inline void yield_processor(void) {
  std::this_thread::yield();
}

#elif defined(HAVE_OSX_ATOMIC_OPS)
class atomic_int {
  public:
    atomic_int(): val_(0) {}
    atomic_int(int init): val_(init) {}
    int add(int a) {
      return OSAtomicAdd32(a, &val_);
    }
    bool compare_and_swap(int a, int b) {
      return OSAtomicCompareAndSwapInt(a, b, &val_);
    }
    int load() {
      return OSAtomicCompareAndSwapInt(0, 0, &val_);
    }
  private:
    volatile int val_;
};
inline void yield_processor(void) {
  sched_yield();
}
#define HAVE_ATOMIC_OPS 1

#elif defined(HAVE_GCC_ATOMIC_OPS)
class atomic_int {
  public:
    atomic_int(): val_(0) {}
    atomic_int(int init): val_(init) {}
    int add(int a) {
      return __sync_add_and_fetch(&val_, a);
    }
    int compare_and_swap(int a, int b) {
      return __sync_val_compare_and_swap(&val_, a, b);
    }
    int load() {
      return __sync_val_compare_and_swap(&val_, 0, 0);
    }
  private:
    volatile int val_;
};
inline void yield_processor(void) {
  sched_yield();
}
#define HAVE_ATOMIC_OPS 1

#elif (defined(_WIN32) && !defined(__CYGWIN__))
class atomic_int {
  public:
    atomic_int(): val_(0) {}
    atomic_int(int init): val_(init) {}
    int add(int a) {
      long ret = ::InterlockedExchangeAdd(&val_, static_cast<long>(a));
      return static_cast<int>(ret);
    }
    int compare_and_swap(int a, int b) {
      long ret = ::InterlockedCompareExchange(&val_, static_cast<long>(a), static_cast<long>(b));
      return static_cast<int>(ret);
    }
    int load() {
      long ret = ::InterlockedCompareExchange(&val_, 0, 0);
      return static_cast<int>(ret);
    }
  private:
    volatile long val_;
};
inline void yield_processor(void) {
  YieldProcessor();
}
#define HAVE_ATOMIC_OPS 1
#endif

#ifdef HAVE_ATOMIC_OPS
// This is a simple non-scalable writer-preference lock.
// Slightly modified the following paper.
// "Scalable Reader-Writer Synchronization for Shared-Memory Multiprocessors"
// PPoPP '91. John M. Mellor-Crummey and Michael L. Scott. T
class read_write_mutex {
 public:
  inline void write_lock() {
    write_pending_.add(1);
    while (l_.compare_and_swap(0, kWaFlag)) {
      yield_processor();
    }
  }
  inline void read_lock() {
    while (write_pending_.load() > 0) {
      yield_processor();
    }
    l_.add(kRcIncr);
    while ((l_.load() & kWaFlag) != 0) {
      yield_processor();
    }
  }
  inline void write_unlock() {
    l_.add(-kWaFlag);
    write_pending_.add(-1);
  }
  inline void read_unlock() {
    l_.add(-kRcIncr);
  }

  read_write_mutex(): l_(0), write_pending_(0) {}

 private:
  static const int kWaFlag = 0x1;
  static const int kRcIncr = 0x2;
  atomic_int l_;
  atomic_int write_pending_;
};

class scoped_writer_lock {
 public:
  scoped_writer_lock(read_write_mutex *mutex) : mutex_(mutex) {
    mutex_->write_lock();
  }
  ~scoped_writer_lock() {
    mutex_->write_unlock();
  }
 private:
  read_write_mutex *mutex_;
};

class scoped_reader_lock {
 public:
  scoped_reader_lock(read_write_mutex *mutex) : mutex_(mutex) {
    mutex_->read_lock();
  }
  ~scoped_reader_lock() {
    mutex_->read_unlock();
  }
 private:
  read_write_mutex *mutex_;
};
#endif  // HAVE_ATOMIC_OPS

class thread {
 private:
#ifdef HAVE_PTHREAD_H
  pthread_t hnd;
#else
#ifdef _WIN32
  HANDLE  hnd;
#endif
#endif

 public:
  static void* wrapper(void *ptr) {
    thread *p = static_cast<thread *>(ptr);
    p->run();
    return 0;
  }

  virtual void run() {}

  void start() {
#ifdef HAVE_PTHREAD_H
    pthread_create(&hnd, 0, &thread::wrapper,
                   static_cast<void *>(this));

#else
#ifdef _WIN32
    DWORD id;
    hnd = BEGINTHREAD(0, 0, &thread::wrapper, this, 0, &id);
#endif
#endif
  }

  void join() {
#ifdef HAVE_PTHREAD_H
    pthread_join(hnd, 0);
#else
#ifdef _WIN32
    WaitForSingleObject(hnd, INFINITE);
    CloseHandle(hnd);
#endif
#endif
  }

  virtual ~thread() {}
};
}
#endif
