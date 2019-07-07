/*
 * Filename:
 * ---------
 * readwritelock.h
 *
 * Project:
 * --------
 * ReadWriteLock
 *
 * Description:
 * ------------
 *
 * C++ read write lock.
 *
 * Author:
 * ------------
 *
 * jues
 *
 * Email:
 * ------------
 *
 * jues1991@163.com
 *
 * Home:
 * ------------
 *
 * http://note.jues.org.cn/node/xxx
*/

#ifndef READWRITELOCK_H
#define READWRITELOCK_H
#include <mutex>
#include <atomic>
#include <condition_variable>


namespace jues
{





// mutex
class mutex
{
public:
    inline mutex(){
        this->m_read = 0;
        this->m_write = 0;
    }
    inline virtual ~mutex() {}

public:
    void readLock(){
        std::mutex mtx;
        while(0 < this->m_write)
        {
            std::unique_lock<std::mutex> lock(mtx);
            //
            this->m_cv.wait(lock);
        }
        this->m_read++;
    }
    void readUnlock(){
        if ( 0 == this->m_read )
        {
            return;
        }
        this->m_read--;
        this->m_cv.notify_all();
    }
    void writeLock(){
        std::mutex mtx;
        while(0 < this->m_write ||
              0 < this->m_read )
        {
            std::unique_lock<std::mutex> lock(mtx);
            //
            this->m_cv.wait(lock);
        }
        this->m_write++;
    }
    void writeUnlock(){
        if ( 0 == this->m_write )
        {
            return;
        }
        this->m_write--;
        this->m_cv.notify_all();
    }


protected:
    std::atomic<size_t> m_read;
    std::atomic<size_t> m_write;

    std::condition_variable m_cv;
};



// read lock
template<typename _Mutex>
class ReadLock
{
public:
    typedef _Mutex mutex_type;

    explicit ReadLock(mutex_type& __m) : _M_device(__m)
    { _M_device.readLock(); }

    virtual ~ReadLock()
    { _M_device.readUnlock(); }

    ReadLock(const ReadLock&) = delete;
    ReadLock& operator=(const ReadLock&) = delete;

private:
    mutex_type&  _M_device;
};



// write lock
template<typename _Mutex>
class WriteLock
{
public:
    typedef _Mutex mutex_type;

    explicit WriteLock(mutex_type& __m) : _M_device(__m)
    { _M_device.writeLock(); }

    virtual ~WriteLock()
    { _M_device.writeUnlock(); }

    WriteLock(const WriteLock&) = delete;
    WriteLock& operator=(const WriteLock&) = delete;

private:
    mutex_type&  _M_device;
};







}

#endif //READWRITELOCK_H









