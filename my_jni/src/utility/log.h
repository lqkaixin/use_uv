#include <stdio.h>

#define DEFINE_LOG static const char * const _cl;
#define DECLEAR_LOG(cl) const char * const cl::_cl = #cl;

#define CLOG_DEBUG(fmt, ...) printf("[%-20s: %-20s: %4d] " fmt "\n", _cl, __FUNCTION__, __LINE__, __VA_ARGS__)
#define CLOG_ERROR(fmt, ...) printf(fmt "\n", __VA_ARGS__)

#define LOG_DEBUG(fmt, ...) printf("[%-10s: %4d] " fmt "\n", __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(fmt, ...) printf(fmt "\n", __VA_ARGS__)