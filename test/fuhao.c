char *get_android_system_info()
{
	return "ubuntu";
}

const char * get_android_imei(void)
{
	return "abcd";
}

unsigned int gettid()
{
    return pthread_self();
}
