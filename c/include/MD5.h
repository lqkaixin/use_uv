
class MD5
{
public:

	MD5();
	~MD5();
	void reset();

	void update(unsigned char *str, int len);
	void final();

	void get_result(char out[]);
	void get_hex_result(char out[]);
	void out_put_result();

	void md5_string(char *str, int len);
	bool md5_file(char * file_path);

private:
	void calc();

private:

	static unsigned F(unsigned b, unsigned c, unsigned d);
	static unsigned G(unsigned b, unsigned c, unsigned d);
	static unsigned H(unsigned b, unsigned c, unsigned d);
	static unsigned I(unsigned b, unsigned c, unsigned d);

private:

	//unsigned data[16] = {0x8061, 0,0,0,0,0,0,0,0,0,0,0,0,0,8,0};//"a"
	unsigned data[16];
	long long data_len;
	unsigned result[4];
	
private:

	static unsigned (* f[4])(unsigned b, unsigned c, unsigned d);
	
	static const unsigned PRE[4];
	
	static const unsigned data_idx[4][2];
	static const unsigned s[4][4];
	
	static const unsigned t[64];
};