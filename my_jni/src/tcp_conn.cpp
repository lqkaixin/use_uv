#include <uv.h>
#include <stdio.h>

#include <string.h>

#include "http_parser.h"
#include "http_conn.h"


DECLEAR_LOG(tcp_conn);

#undef LOG_DEBUG
#undef LOG_ERROR

#define LOG_DEBUG CLOG_DEBUG
#define LOG_ERROR CLOG_ERROR


void tcp_conn::getaddr_cb(uv_getaddrinfo_t* req, int status, struct addrinfo* res)
{
  LOG_DEBUG("getaddr: %s, status: %d", "", status);

  tcp_conn * dev = (tcp_conn *)req->data;


  struct addrinfo* res1 = res;
  while (res1 && 0)
  {
    LOG_DEBUG("getaddr flags: %x  %s %x", res1->ai_flags, res1->ai_canonname, ((struct sockaddr_in*)(res1->ai_addr))->sin_addr.s_addr);
    res1 = res1->ai_next;
  }

  char p[100] = {0};
  uv_ip4_name(((struct sockaddr_in*)(res->ai_addr)), p, 100);
  
  LOG_DEBUG("ip: %s, port: %d, family: %d", p, ((struct sockaddr_in*)(res->ai_addr))->sin_port,
    ((struct sockaddr_in*)(res->ai_addr))->sin_family);

  uv_ip4_addr(p, 80, &dev->dest);
  uv_tcp_init(uv_default_loop(), &dev->tcp_s);
  uv_tcp_connect(&dev->conn_req, &dev->tcp_s, (struct sockaddr *)&dev->dest, connect_cb);


  uv_freeaddrinfo(res);
}


void tcp_conn::connect_cb(uv_connect_t* req, int status)
{
    LOG_DEBUG("connect status: %d", status);

    tcp_conn * dev = (tcp_conn *)req->data;

    dev->cb->conn_cb();
    uv_read_start((uv_stream_t *)&dev->tcp_s, alloc_cb, read_cb);

}

void tcp_conn::write_cb(uv_write_t* req, int status)
{
    LOG_DEBUG("write status: %d", status);
    tcp_conn * dev = (tcp_conn *)req->data;

    dev->cb->send_cb();
}

void tcp_conn::read_cb(uv_stream_t* stream,
                           ssize_t nread,
                           const uv_buf_t* buf)
{
    LOG_DEBUG("read nread: %d", (int)nread);
    tcp_conn * dev = (tcp_conn *)stream->data;

    if (nread <0)
    {
        LOG_DEBUG("%s", uv_err_name(nread));
        dev->close();
    }
    else 
    {
        // dev->cb->recv_cb(buf->base, 200);
        // dev->cb->recv_cb(buf->base+200, nread-200);
        dev->cb->recv_cb(buf->base, nread);
    }
}


void tcp_conn::alloc_cb(uv_handle_t* handle,
                            size_t suggested_size,
                            uv_buf_t* buf)
{
    tcp_conn * dev = (tcp_conn *)handle->data;
    dev->buf_rd = uv_buf_init(dev->buff, 1024);
    *buf = dev->buf_rd;
    LOG_DEBUG("alloc suggested: %d", (int)suggested_size);

}



tcp_conn::tcp_conn(i_tcp_cb *cb)
{
    this->cb = cb; dns_req.data = this; conn_req.data = this; wr_req.data = this; tcp_s.data = this;
}

int tcp_conn::connect(const char *domain, int port)
{
    uv_getaddrinfo(uv_default_loop(), &dns_req, getaddr_cb, /*"p2s.cl.kankan.com"*/domain, NULL, NULL);
}
int tcp_conn::send(const char *data, int len)
{
    buf_wr = uv_buf_init((char*)data, len);
    uv_write(&wr_req, (uv_stream_t *)&tcp_s, &buf_wr, 1, write_cb);
}
int tcp_conn::close()
{
    int ret = uv_read_stop((uv_stream_t *)&tcp_s);
    LOG_DEBUG("%d", ret);
    uv_close((uv_handle_t *)&tcp_s, NULL);

    cb->close_cb();
}