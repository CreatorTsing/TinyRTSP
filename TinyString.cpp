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
    m_Data = m_EmptyString;
    TinyString(from,strlen(from));
}

TinyString::TinyString(char *from,int size)
{
    if(m_Data == m_EmptyString)
    {
        m_Data = (char *)malloc(size);
    }
}

~TinyString::TinyString()
{
    if(m_Data != m_EmptyString)
    {
        free(m_Data);
        m_Data = m_EmptyString;
    }
}

void TinyString::MakeMutable()
{
    if
}
