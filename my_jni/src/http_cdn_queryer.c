#include "http_conn.h"

#include <string.h>

#include "log.h"

#undef LOG_DEBUG
#undef LOG_ERROR

#define LOG_DEBUG CLOG_DEBUG
#define LOG_ERROR CLOG_ERROR

static const char url[1024] = "GET /getCdnresource_flv?gcid=0E3CE7A2ADC93B85C8E9943E03E97D71B03BA15C HTTP/1.1\r\n"
            "Host: p2s.cl.kankan.com\r\n"
            "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:38.0) Gecko/20100101 Firefox/38.0\r\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
            "Accept-Language: zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3\r\n"
            "Accept-Encoding: gzip, deflate\r\n"
            "Connection: close\r\n"
            "\r\n";


class http_pipe: public i_http_cb
{
    http_conn dev;

    char host[128];
    int port;

    char key1[64];
    char key2[64];

public:
    http_pipe(char *host, int port, char *key1, char *key2):dev(this)
    {
        dev.connect(host, 80);
    }


public:
    virtual void conn_cb(){dev.send(url, strlen(url));}
    virtual void header_cb(){}
    virtual void body_cb(const char *data, int len){}
};

http_pipe *pipe;


class http_cdn_queryer:public i_http_cb
{
    DEFINE_LOG
    http_conn dev;

public:
    http_cdn_queryer():dev(this)
    {
        dev.connect("p2s.cl.kankan.com", 80);
    }

public:
    virtual void conn_cb(){dev.send(url, strlen(url));}
    virtual void header_cb(){}
    virtual void body_cb(const char *buf, int len)
    {
        char *start = strstr((char*)buf, "ip:\"") + 4;
        char *end = strstr(start, "\"");
        *end = '\0';
        LOG_DEBUG("%s", start);

        char *host = start;

        start = strstr(end+1, "path:\"") + 6;
        end = strstr(start, "\"");
        *end = '\0';
        LOG_DEBUG("%s", start);

        int port = 80;

        start = strstr(end+1, "param1:") + 7;
        end = strstr(start, ",");
        *end = '\0';
        LOG_DEBUG("%s", start);

        char *key1 = start;

        start = strstr(end+1, "param2:") + 7;
        end = strstr(start, "}");
        *end = '\0';
        LOG_DEBUG("%s", start);

        char *key2 = start;

        pipe = new http_pipe(host, port, key1, key2);
    }
};

DECLEAR_LOG(http_cdn_queryer)


http_cdn_queryer ss;