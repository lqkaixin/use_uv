#include "http_conn.h"

class hub_queryer: public i_http_cb
{
    http_conn dev;

public:
    hub_queryer();

public:
    virtual void conn_cb();
    virtual void header_cb();
    virtual void body_cb(const char *data, int len);
};