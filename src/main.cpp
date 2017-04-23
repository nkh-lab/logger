#include "logger.hpp"

void test1(void);


int main(void)
{
    LOG_FUNC_ENTRY;

    LOG_INFO << "Test Info " << 1;
    LOG_WARNING << "Test Warning " << 2;
    LOG_ERROR << "Test Error " << 3;
    LOG_DEBUG << "Test Debug " << 4;

    for(int i = 0; i < 10; i++)
    {
        test1();
    }


    LOG_FUNC_EXIT;
}


void test1(void)
{
   LOG_FUNC_ENTRY;

   LOG_FUNC_EXIT;
}
