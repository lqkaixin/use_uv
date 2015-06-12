#include "http_parser.h"
#include "tcp_conn.h"

#include "log.h"

class i_http_cb;

class http_conn: public i_tcp_cb
{
    DEFINE_LOG
private:
    tcp_conn dev;
    i_http_cb *cb;

    char buff_wr[1024];

    http_parser parser;
    http_parser_settings parser_settings;


    long long parsed_body_len;
    long long content_length;

public:
    http_conn(i_http_cb *);

    int connect(const char *host, int port);
    int send(const char *data, int len);

public:
    // callback called by tcp_conn
    virtual void conn_cb();
    virtual void send_cb(){}
    virtual void recv_cb(const char* buf, int len);
    virtual void close_cb(){}
    virtual void error_cb(){}

public:

    // 当解析到URL时的回调 callback called by http_parser
    static int on_message_begin(http_parser *parser);
    static int url_cb(http_parser *parser, const char *buf, size_t len);
    static int status_cb(http_parser *parser, const char *buf, size_t len);
    static int header_field_cb(http_parser *parser, const char *buf, size_t len);
    static int header_value_cb(http_parser *parser, const char *buf, size_t len);
    static int on_headers_complete(http_parser *parser);
    static int body_cb(http_parser *parser, const char *buf, size_t len);
    static int on_message_complete(http_parser *parser);
};

class i_http_cb
{
public:
    virtual void conn_cb() = 0;
    virtual void header_cb() = 0;
    virtual void body_cb(const char *data, int len) = 0;
};