#include "http_conn.h"

#include <string.h>

DECLEAR_LOG(http_conn);

#undef LOG_DEBUG
#undef LOG_ERROR

#define LOG_DEBUG CLOG_DEBUG
#define LOG_ERROR CLOG_ERROR


http_conn::http_conn(i_http_cb *cb):dev(this)
{

    this->cb = cb;
    // memcpy(buff_wr, url, 1024);
    
    http_parser_init(&parser, HTTP_RESPONSE);

    parser.data = this;
    
    parser_settings.on_message_begin = on_message_begin;
    parser_settings.on_url = url_cb;
    parser_settings.on_status = status_cb;
    parser_settings.on_header_field = header_field_cb;
    parser_settings.on_header_value = header_value_cb;
    parser_settings.on_headers_complete = on_headers_complete;
    parser_settings.on_body = body_cb;
    parser_settings.on_message_complete = on_message_complete;
}


void http_conn::conn_cb()
{
    cb->conn_cb();
    // dev.send(buff_wr, strlen(buff_wr));
}
void http_conn::recv_cb(const char* buf, int len)
{
    int nparsed = http_parser_execute(&parser, &parser_settings, buf, len);

    LOG_DEBUG("%d parsed: %d", len, nparsed);
}



// 当解析到URL时的回调
int http_conn::url_cb(http_parser *parser, const char *at, size_t len) {
    char tmp[1024] = {0};
    memcpy(tmp, at, len);
    LOG_DEBUG("on url_cb: %s", tmp);
    return 0;
}

// 当解析到URL时的回调
int http_conn::status_cb(http_parser *parser, const char *at, size_t len) {
    char tmp[1024] = {0};
    memcpy(tmp, at, len);
    LOG_DEBUG("on status_cb: %d(%s)", parser->status_code, tmp);
    return 0;
}


// 当解析到头部字段名称的回调
int http_conn::header_field_cb(http_parser *parser, const char *at, size_t len) {
    char tmp[1024] = {0};
    memcpy(tmp, at, len);
    LOG_DEBUG("on header_field_cb: %s", tmp);

    return 0;
}


// 当解析到头部字段值的回调
int http_conn::header_value_cb(http_parser *parser, const char *at, size_t len) {
    char tmp[1024] = {0};
    memcpy(tmp, at, len);
    LOG_DEBUG("on header_value_cb: %s", tmp);

    http_conn *conn = (http_conn*)parser->data;
    LOG_DEBUG("parsed_len: %lld, %lu, %s", conn->parsed_body_len, parser->content_length, tmp);
    return 0;
}

// 当解析到HTTP消息主体的回调
int http_conn::body_cb(http_parser *parser, const char *at, size_t len) {
    char tmp[1024] = {0};
    memcpy(tmp, at, len);

    http_conn *conn = (http_conn*)parser->data;

    conn->cb->body_cb(tmp, len);

    LOG_DEBUG("parsed_len: %lld, %lu, %s", conn->parsed_body_len, parser->content_length, tmp);
    return 0;
}


// 通知回调：说明开始解析HTTP消息
int http_conn::on_message_begin(http_parser *parser) {
  LOG_DEBUG("on_message_begin%s","");

  http_conn *conn = (http_conn*)parser->data;
  conn->parsed_body_len = 0;
  return 0;
}


// 通知回调：说明消息解析完毕
int http_conn::on_message_complete(http_parser *parser) {
    LOG_DEBUG("on_message_complete%s","");
    http_conn *conn = (http_conn*)parser->data;
    conn->dev.close();
    return 0;
}


// 通知回调：说明HTTP报文头部解析完毕
int http_conn::on_headers_complete(http_parser *parser) {
  LOG_DEBUG("on_headers_complete%s","");
  return 0;
}


int http_conn::connect(const char *host, int port)
{
    dev.connect(host, port);
}
int http_conn::send(const char *data, int len)
{
    dev.send(data, len);
}