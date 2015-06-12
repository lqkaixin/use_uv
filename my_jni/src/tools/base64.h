

char *base64_encode(const char *in, int in_len, char *out, int out_len);
char *base64_decode(const char *in, int in_len, char *out, int out_len);

int str2hex(const char *in, int in_len, char *out, int out_len);
int hex2str(const char *in, int in_len, char *out, int out_len);