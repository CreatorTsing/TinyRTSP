#include "include/TinyRTSPSession.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

TinyRTSPSession::TinyRTSPSession(char * url)
{
	m_Url = NULL;
    if(url)
        m_Url = strdup(url);
    m_Port = 0;

    m_sockNum = -1;
}

TinyRTSPSession::~TinyRTSPSession()
{
	if(m_Url)
	{
		free(m_Url);
	}
	m_Url = NULL;

	if(m_sockNum >= 0)
	{
		close(m_sockNum);
		m_sockNum = -1;
	}
}

int TinyRTSPSession::ParseUrl(char *url)
{
    if(strncasecmp(url,"rtsp://",7) != 0)
    {
        printf("url doesn't contain rtsp \n");
        return -1;
    }
    
    char * pHostStart = url+7;
    char * pSlash = strstr(pHostStart,"/");
    if(!pSlash)
    {
        printf("url doesn't contain /\n");
        return -1;
    }
    int hostNameLen = pSlash - pHostStart;
    char * pTmpHostName = (char *)malloc(hostNameLen +1);
    if(!pTmpHostName )
    {
        printf("malloc failed\n");
        return -1;
    }

    memset(pTmpHostName,0,hostNameLen+1);
    strncpy(pTmpHostName,pHostStart,hostNameLen);
   	
   	printf("[%s][%d]%s\n",__FUNCTION__,__LINE__,pTmpHostName);
    char * pPortStart; 
    if((pPortStart = strstr(pTmpHostName,":")) != NULL)
    {
        //pPortStart += 1;
        //strncpy(m_Port,pPortStart,strlen(pTmpHostName)-(pPortStart - pTmpHostName));
        
        sscanf(pPortStart,":%d/",&m_Port);
        strncpy(m_Host,pTmpHostName,(int)(pPortStart - pTmpHostName));
    }else
    {
        strcpy(m_Host,pTmpHostName);
    }

    free(pTmpHostName);
    pTmpHostName = NULL;
    
    return 0;
}

int TinyRTSPSession::MakeSocketBlocking(int s,bool blocking)
{
	int flags = fcntl(s,F_GETFL,0);

	if(flags == -1)
	{
		return -1;
	}
	if(blocking)
	{
		flags &= ~O_NONBLOCK;
	}else
	{
		flags |= O_NONBLOCK;
	}
	
	flags = fcntl(s,F_SETFL,flags);
	if(flags == -1)
	{
		printf("[%s][%d] fcntl error\n",__FUNCTION__,__LINE__);
		return -1;
	}
	return 0;
}


int TinyRTSPSession::InitSession()
{
	int res = ParseUrl(m_Url);
	if(res < 0)
	{
		printf("parse url failed\n");
		return -1;
	}

	printf("[%s][%d][host %s][port %d]\n",__FUNCTION__,
						__LINE__,
						m_Host,
						m_Port);

    m_sockNum = socket(AF_INET,SOCK_STREAM,0);
    if(m_sockNum < 0)
    {
        printf("[%s][%d] create socket error\n",__FUNCTION__,__LINE__);
        return -1;
    }
    
    //connect
	/*
    struct addrinfo *res,hint;
    bzero(&hint);

    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;

    if(getaddrinfo(m_Host,NULL,&hint,&res) != 0)
    {
        printf("[%s][%d] resolve host fail\n",__FUNCTION__,__LINE__,);
        return -1;
    }

    if(res == NULL)
    {
        printf("[%s][%d] resolve host fail\n",__FUNCTION__,__LINE__,);
        return -1;
    }
    */

	struct hostent * ht = gethostbyname(m_Host);
	if(ht == NULL)
	{
		printf("[%s][%d] get host failed\n",__FUNCTION__,__LINE__);
		return -1;
	}    
    
    MakeSocketBlocking(m_sockNum,false);

	struct sockaddr_in server;
	memset(server.sin_zero,0,sizeof(server.sin_zero));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = *(in_addr_t *)ht->h_addr;
	server.sin_port = htons(m_Port);
	
	int err = connect(m_sockNum,(const struct sockaddr *)&server,sizeof(server));	
    if(err < 0)
	{
		if(errno == EINPROGRESS)
		{
			struct timeval tv;
			tv.tv_sec = 3;
			tv.tv_usec = 500*1000;
			fd_set ws;
			FD_ZERO(&ws);
			FD_SET(m_sockNum,&ws);
			
			int res = select(m_sockNum + 1,NULL,&ws,NULL,&tv);
			if(res == 0)
			{
				printf("[%s][%d]connect timeout\n",__FUNCTION__,__LINE__);
				return -1;
			}

			int socketErr;
			socklen_t optionLen = sizeof(socketErr);
			getsockopt(m_sockNum,SOL_SOCKET,SO_ERROR,&socketErr,&optionLen);

			if(socketErr != 0)
			{
				printf("[%s][%d][%s]\n",__FUNCTION__,__LINE__,strerror(socketErr));
				return -1;
			}

		}
	}
	
	return 0;
}













