#ifndef __CTcpStream_H_
#define __CTcpStream_H_



class   CTcpStream 
{
private:
    IEventHandle*   m_pEventHandle;
    SOCKET          m_skSocket;
    BOOL            m_bConnected;

    ULONG           m_ulLocalIP;
    ULONG           m_ulRemoteIP;
    USHORT          m_usRemotePort;

private:
    static  CDefaultEventHandle gc_oDefaultEventHandle;

public:
    CTcpStream();
    void    Open(IEventHandle* pEventHandle, SOCKET skAccepted);
    void    Open(IEventHandle* pEventHandle, ULONG ulLocalIP, ULONG ulRemoteIP, USHORT usRemotePort);
    int     Connect();
    void    Close();
    SOCKET  GetSocket()const;
    int     HandleException();
    int     HandleRead();
    int     HandleWrite();
};

typedef CList<CTcpStream*, CTcpStream*> CStreamList;

#endif//__CTcpStream_H_


