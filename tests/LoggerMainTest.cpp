#include "nkh-lab/logger.hpp"

#include <thread>

void test1(void);
void test2(void);
void main_th(void);

// if use logger like headeronly library
#ifdef BUILD_HEADER_LIB
std::mutex nlab::logger::gCoutMutex;
#endif

int main(void)
{
    LOG_FNC;

    std::thread t1(main_th);
    std::thread t2(main_th);

    t1.join();
    t2.join();

    LOG_INF << "Test Info " << 1;
    LOG_WRN << "Test Warning " << 2;
    LOG_ERR << "Test Error " << 3;
    LOG_DBG << "Test Debug " << 4;

    nlab::logger::logMsg<nlab::logger::MsgType::Info>(
        __FILE__, __FUNCTION__, __LINE__, "Test %d %s %c", 888, "str", 'c');

    void* ptr = nullptr;
    if (CHECK(ptr, "ptr = nullptr"))
    {
    }

    for (int i = 0; i < 3; i++)
    {
        test1();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // t1.detach();
    // t2.detach();
}

void test1(void)
{
    LOG_FNC;
}

void test2(void)
{
    LOG_FNC;
}

void main_th(void)
{
    LOG_FNC;
    for (int i = 0; i < 3; i++)
    {
        test2();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
