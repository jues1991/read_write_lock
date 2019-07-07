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
    }
    inline virtual ~mutex() {}

public:
    void readLock(){
        std::unique_lock<std::mutex> lock(this->m_mtx);
        while ( true == this->m_write_used )
        {
           this->m_cv.wait(lock);
        }
        this->m_read_count++;
    }
    void readUnlock(){
        std::unique_lock<std::mutex> lock(this->m_mtx);
        this->m_read_count--;
        if ( 0 == this->m_read_count)
        {
           this->m_cv.notify_all();
        }
    }
    void writeLock(){
        std::unique_lock<std::mutex> lock(this->m_mtx);
        while ( 0 < this->m_read_count ||
                true == this->m_write_used )
        {
           this->m_cv.wait(lock);
        }
        this->m_write_used = true;
    }
    void writeUnlock(){
        std::unique_lock<std::mutex> lock(this->m_mtx);
        this->m_write_used = false;
        this->m_cv.notify_all();
    }


protected:
    size_t m_read_count = 0;
    bool m_write_used = false;
    std::mutex m_mtx;
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









