#include <string.h>
#include <sys/types.h>

class TinyString
{
public:
    TinyString();
    TinyString(char* from);
    TinyString(char* from,int size);
    ~TinyString();
    Append(char* from);
    Append(char* from,int size);

private:
    char *m_Data;
    static const char* m_EmptyString
    int m_Size;
    int m_AllocSize; 
}

