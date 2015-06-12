#include <uv.h>
#include "log.h"


class i_tcp_cb;

class tcp_conn
{
    DEFINE_LOG

    uv_tcp_t tcp_s;
    
    uv_getaddrinfo_t dns_req;
    uv_connect_t conn_req;
    uv_write_t wr_req;

    struct sockaddr_in dest;

    char buff[1024];
    uv_buf_t buf_wr;
    uv_buf_t buf_rd;

    i_tcp_cb *cb;

public:
    tcp_conn(i_tcp_cb *cb);

    int connect(const char *domain, int port);
    int send(const char *data, int len);
    int close();

public:
    static void getaddr_cb(uv_getaddrinfo_t* req, int status, struct addrinfo* res);
    static void connect_cb(uv_connect_t* req, int status);
    static void write_cb(uv_write_t* req, int status);
    static void read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
    static void alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);

};

class i_tcp_cb
{
public:
    virtual void conn_cb() = 0;
    virtual void send_cb() = 0;
    virtual void recv_cb(const char* buf, int len) = 0;
    virtual void close_cb() = 0;
    virtual void error_cb() = 0;
};