

typedef struct rc4_ctx_t
{
	unsigned char S[256];
	int i, j;
}rc4_ctx;

void rc4_init(rc4_ctx *ctx, char *key, int len);

void updatestate(rc4_ctx *ctx);

void rc4_crypt(rc4_ctx *ctx, char *src, char *des, int len);