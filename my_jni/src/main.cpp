#include "test.h"

#include <uv.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

#include "log.h"

static long long counter = 0;
static long long counter1 = 0;

static void wait_for_a_while(uv_idle_t *handle)
{
     counter++;

    if (counter == 1)
    {
      // uv_cancel(&open_req);
    }
    sleep(1);
    if (counter%5==0)
      LOG_DEBUG("wait_for_a_while tid: %d, run time: %lld s", 123, counter);
}

static uv_timer_t timer;


static void wait_for_a_while1(uv_timer_t *handle)
{
     counter1++;

    if (counter1 == 1 && 0)
    {
      // uv_cancel(&open_req);
    }
    LOG_DEBUG("wait_for_a_while1 tid: %d, run time: %lld s", 123, counter1*5);
}



int main()
{
    test_rg();

    uv_idle_t idler;
  
    //uv_idle_init(uv_default_loop(), &idler);
    //uv_idle_start(&idler, wait_for_a_while);

    //uv_timer_init(uv_default_loop(), &timer);
    //uv_timer_start(&timer, wait_for_a_while1, 5000, 10000);

    // uv_getaddrinfo_t dns_req;
    // uv_getaddrinfo(uv_default_loop(), &dns_req, getaddr_cb, "p2s.cl.kankan.com", NULL, NULL);

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  
  return 0;
}