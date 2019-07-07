#include <iostream>
#include <thread>
#include <list>
#include <chrono>
#include <jues/readwritelock.h>

using namespace std;
static jues::mutex m;


static void test_read_lock( const int &d )
{
    jues::ReadLock<jues::mutex> read(m);
    //
    cout<<"pid="<<this_thread::get_id()<<", test_read_lock d="<<d<<"."<<endl;
    this_thread::sleep_for(chrono::milliseconds(1000));
}

static void test_write_lock( const int &d )
{
    jues::WriteLock<jues::mutex> write(m);
    //
    cout<<"pid="<<this_thread::get_id()<<", test_write_lock d="<<d<<"."<<endl;
    this_thread::sleep_for(chrono::milliseconds(1000));
}


int main()
{
    list<thread*> pool;
    int i = 0;

    //read
    for (;10>i;i++)
    {
        pool.push_back(new thread(test_read_lock,i));
    }

    //write
    for (;15>i;i++)
    {
        pool.push_back(new thread(test_write_lock,i));
    }

    //
    while (0<pool.size())
    {
        pool.front()->join();
        pool.pop_front();
    }

    return 0;
}
