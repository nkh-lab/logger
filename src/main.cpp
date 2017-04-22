#include "logger.hpp"

int main(void)
{
    LOG_INFO << "Test Info " << 1;
    LOG_WANING << "Test Warning " << 1;
    LOG_ERROR << "Test Error " << 1;
    LOG_DEBUG << "Test Debug " << 1;
}
