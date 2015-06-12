#include <uv.h>
#include <string.h>

#include "define.h"

#include "log.h"


char read_buff[1024];
char write_buff[1024] = "hello world!";


typedef struct my_file_s
{
	int fid;

	int64 filesize;
	
	uv_fs_t wr_hd;
	uv_fs_t rd_hd;

}my_file_t;

int write_file(my_file_t *f, char *p, int len, long long offset);

int read_file(my_file_t *f, char *p, int len, long long offset);

static void on_open(uv_fs_t *req)
{
	LOG_DEBUG("result: %lu", req->result);
	my_file_t *f = (my_file_t*)req->data;
	f->fid = req->result;
	write_file(f, write_buff, strlen(write_buff), 0);
	read_file(f, read_buff, sizeof(read_buff), 0);
}

static void on_write(uv_fs_t *req)
{
	LOG_DEBUG("result: %lu", req->result);
}

static void on_read(uv_fs_t *req)
{
	LOG_DEBUG("result: %lu", req->result);
}






int write_file(my_file_t *f, char *p, int len, long long offset)
{
	uv_buf_t buf = uv_buf_init(p, len);
	uv_fs_write(uv_default_loop(),&f->wr_hd, f->fid,
                          &buf,
                          1,
                          offset,
                          on_write);

	return 0;
}

int read_file(my_file_t *f, char *p, int len, long long offset)
{
	uv_buf_t buf = uv_buf_init(p, len);
	uv_fs_read(uv_default_loop(),&f->rd_hd, f->fid,
                          &buf,
                          1,
                          offset,
                          on_read);

	return 0;
}

int test_file()
{
	my_file_t file_s;
	file_s.rd_hd.data = &file_s;

	uv_fs_open(uv_default_loop(), &file_s.rd_hd, "a.flv", O_RDWR, 0, on_open);

	uv_run(uv_default_loop(), UV_RUN_DEFAULT);
	return 0;
}