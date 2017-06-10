#ifndef __CTcpConnector_H_
#define __CTcpConnector_H_

class CTcpConnector : private CTcpThread
{
private:
    CStreamList         m_lsConnectStreamList;

    CCRITICALSECTION    m_csReadyStreamList;
    CStreamList         m_lsReadyStreamList;

    struct TSelectTable
    {
        int         iSize;
        CTcpFDSet   tFDSet;
        int         iSelectIndex[FD_SETSIZE];
        CTcpStream* pStream;
        POSITION    pPos;
    };

private:
    virtual void Run();

public:
    CTcpConnector();
    ~CTcpConnector();

    int     Create();
    void    Destroy();

    int     OpenConnection(ULONG ulLocalIP, ULONG ulRemoteIP, USHORT usRemotePort, IEventHandle* pEventHandle);
};

#endif//__CTcpConnector_H_