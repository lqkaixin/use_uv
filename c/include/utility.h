
#define DEBUG(fmt, ...) printf("[%s: %d] " fmt "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)

int str2hex(char *buf, int len);

int hex2str(char *buf, int len);

int base642str(char *buf, int len);

int str2base64(char *buf, int len);

