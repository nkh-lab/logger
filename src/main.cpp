#include <thread>

#include "logger.hpp"

void test1(void);
void test2(void);
void main_th(void);

int main(void)
{
    LOG_FUNC;

    std::thread t1(main_th);
    std::thread t2(main_th);

    t1.join();
    t2.join();

    LOG_INFO << "Test Info " << 1;
    LOG_WARNING << "Test Warning " << 2;
    LOG_ERROR << "Test Error " << 3;
    LOG_DEBUG << "Test Debug " << 4;

    for(int i = 0; i < 3; i++)
    {
        test1();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    //t1.detach();
    //t2.detach();
}

void test1(void)
{
   LOG_FUNC;
}

void test2(void)
{
   LOG_FUNC;
}

void main_th(void)
{
    LOG_FUNC;
    for(int i = 0; i < 3; i++)
    {
        test2();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
