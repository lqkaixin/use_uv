#include <libavformat/avformat.h>

//#include <libswscale/swscale.h>

static ShowUseage(const char* p)
{
	printf("you must specify an input file as: %s youfile.avi!!!\n", p);
}

int main(int argc, char* argv[])
{
	AVFormatContext *pFormatContext = NULL;
	if(argc<2)
	{
		ShowUseage(argv[0]);
		return -1;
	}
	
	char *filepath = argv[1];
	av_register_all();
	//pFormatContext = avformat_alloc_context();
	if(avformat_open_input(&pFormatContext,filepath,NULL,NULL)!=0) //Open the media file and read the header
	{
		printf("Can not open the media file you specified!\n");
		return -1;
	}
	printf("****************file information*****************\n");
	av_dump_format(pFormatContext,0,filepath,0); //dump input information to the stdio
	printf("*************************************************\n");
	avformat_close_input(&pFormatContext);
	return  0;
}
