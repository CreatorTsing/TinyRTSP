
class TinyRTSPSession
{
public:
    TinyRTSPSession(char* url);
    ~TinyRTSPSession();
    int ParseUrl(char * url);
    int MakeSocketBlocking(int s,bool blocking);
    int InitSession();
    int SessionDescribe();
    int SessionAnnounce();
    int SessionGetParameter();
    int SessionOptions();
    int SessionPause();
    int SessionPlay();
    int SessionRedirect();
    int SessionSetup();
    int SessionTeardown();
    int SessionSetParameter();

private:
    char* m_Url;
    char m_Host[128];
    int m_Port;
    
    int m_sockNum;
};

