/*
 * Filename:
 * ---------
 * read_write_lock.h
 *
 * Project:
 * --------
 * read write lock
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
class read_write_mutex
{
public:
    inline read_write_mutex(){
    }
    inline virtual ~read_write_mutex() {}

public:
    void lock_read(){
        std::unique_lock<std::mutex> lock(this->m_mtx);
        while ( true == this->m_write_used )
        {
           this->m_cv.wait(lock);
        }
        this->m_read_count++;
    }
    void unlock_read(){
        std::unique_lock<std::mutex> lock(this->m_mtx);
        this->m_read_count--;
        if ( 0 == this->m_read_count)
        {
           this->m_cv.notify_all();
        }
    }
    void lock_write(){
        std::unique_lock<std::mutex> lock(this->m_mtx);
        while ( 0 < this->m_read_count ||
                true == this->m_write_used )
        {
           this->m_cv.wait(lock);
        }
        this->m_write_used = true;
    }
    void unlock_write(){
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
class lock_reader
{
public:
    typedef _Mutex mutex_type;

    explicit lock_reader(mutex_type& __m) : _M_device(__m)
    { _M_device.lock_read(); }

    virtual ~lock_reader()
    { _M_device.unlock_read(); }

    lock_reader(const lock_reader&) = delete;
    lock_reader& operator=(const lock_reader&) = delete;

private:
    mutex_type&  _M_device;
};



// write lock
template<typename _Mutex>
class lock_writer
{
public:
    typedef _Mutex mutex_type;

    explicit lock_writer(mutex_type& __m) : _M_device(__m)
    { _M_device.lock_write(); }

    virtual ~lock_writer()
    { _M_device.unlock_write(); }

    lock_writer(const lock_writer&) = delete;
    lock_writer& operator=(const lock_writer&) = delete;

private:
    mutex_type&  _M_device;
};







}

#endif //READWRITELOCK_H









