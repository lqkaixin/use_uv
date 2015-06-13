#include <uv.h>

#include "xl/hub_queryer.h"
#include "xl/shub_query.h"

int test_hub()
{
	printf("1: hub, 2: shub:\n");

	int i = 0;
	scanf("%d", &i);
	
	if (i == 1) hub_queryer hub;
	if (i == 2) shub_queryer shub;

	uv_run(uv_default_loop(), UV_RUN_DEFAULT);
	return 0;
}