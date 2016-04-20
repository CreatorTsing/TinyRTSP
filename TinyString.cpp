#include "include/TinyString.h"

const char* TinyString::m_EmptyString = "";

TinyString::TinyString()
: m_Data((char*)m_EmptyString),
    m_Size(0),
    mAllocSize(1)
{
   m_Data = m_EmptyString; 
}

TinyString::TinyString(char *from)
: m_Data((char*)m_EmptyString),
    m_Size(0),
    m_AllocSize(1)
{
    SetTo(from);
}

TinyString::TinyString(char *from,int size)
: m_Data(),
	m_Size(0),
	m_AllocSize(1)
{
    SetTo(from,size);
}

~TinyString::TinyString()
{
    clear();
}

void TinyString::MakeMutable()
{
    if(m_Data == m_EmptyString)
	{
		m_Data = strdup(m_EmptyString);
	}
}

void TinyString::Append(char *s)
{
	Append(s,strlen(s));
}

void TinyString::Append(char* s,int size)
{
	MakeMutable();

	if(m_Size + size +1 > mAllocSize)
	{
		m_AllocSize = (m_AllocSize + size + 31)&(-31);
		m_Data = (char*)realloc(m_Data,m_AllocSize);
		if(!m_Data)
		{
			printf("[%s][%d]realloc failed\n");
			return;
		}
	}

	memcpy(&m_Data[m_Size],s,size);
	m_Size += size;
	m_Data[m_Size] = '\0';
}

void TinyString::SetTo(const char* s)
{
	SetTo(s,strlen(s));
}

void TinyString::SetTo(const char*s,int size)
{
	clear();
	append(s,size);
}

void TinyString::Clear()
{
	if(m_Data&&m_Data != m_EmptyString)
	{
		free(m_Data);
		m_Data = NULL;
	}
	m_Data = m_EmptyString;
	m_Size = 0;
	m_AllocSize = 1;
}
