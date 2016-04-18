#include "include/TinyRTSPSession.h"
#include "stdio.h"

int main(int argc,char** argv)
{
	if(argc < 2)
	{
		printf("param error\n");
		return -1;
	}

	char * pszUrl = argv[1];
	printf("%s\n",pszUrl);

	TinyRTSPSession session(pszUrl);

	session.InitSession();
}