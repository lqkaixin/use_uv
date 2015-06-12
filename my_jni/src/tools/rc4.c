#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rc4.h"
#include "log.h"

void rc4_init(rc4_ctx *ctx, char *key, int len)
{
	LOG_DEBUG("%s, %d\n", key, len);
	int i, j;
	for (i = 0; i <= 255; i++)
		ctx->S[i] = i;
	for (i = 0; i <= 255; i++)
	{
		j = (j + ctx->S[i] + (unsigned char)key[i%len])%256;
		int tmp = ctx->S[i];
		ctx->S[i] = ctx->S[j];
		ctx->S[j] = tmp;
	}
	ctx->i = ctx->j = 0;
}

void updatestate(rc4_ctx *ctx)
{
	ctx->i = (++ctx->i)%256;
	ctx->j = (ctx->j + ctx->S[ctx->i]) % 256;
	LOG_DEBUG("[updatestate] %d %d\n", ctx->i, ctx->j);
	int tmp = ctx->S[ctx->i];
	ctx->S[ctx->i] = ctx->S[ctx->j];
	ctx->S[ctx->j] = tmp;
	LOG_DEBUG("[updatestate] %d\n", ctx->i);
}

void rc4_crypt(rc4_ctx *ctx, char *src, char *des, int len)
{
	int k = 0;
	for (k=0;k<len;k++)
	{
		updatestate();
		des[k] = src[k]^ctx->S[(ctx->S[ctx->i]+ctx->S[ctx->j])%256];
		LOG_DEBUG("[decode] src: %d, des: %d\n", src[k], des[k]);
	}
}
