#include "http_conn.h"

#include <string.h>

static const char url[1024] = "GET /getCdnresource_flv?gcid=0E3CE7A2ADC93B85C8E9943E03E97D71B03BA15C HTTP/1.1\r\n"
            "Host: p2s.cl.kankan.com\r\n"
            "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:38.0) Gecko/20100101 Firefox/38.0\r\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
            "Accept-Language: zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3\r\n"
            "Accept-Encoding: gzip, deflate\r\n"
            "Connection: close\r\n"
            "\r\n";


class hub_queryer: public i_http_cb
{
    http_conn dev;

public:
    hub_queryer():dev(this)
    {
        dev.connect("p2s.cl.kankan.com", 80);
    }

public:
    virtual void conn_cb(){dev.send(url, strlen(url));}
    virtual void header_cb(){}
    virtual void body_cb(const char *data, int len){}
};

