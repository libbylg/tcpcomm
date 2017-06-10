#include "CTcpStream.h"



CDefaultEventHandle CTcpStream::gc_oDefaultEventHandle;


void    CTcpStream::CTcpStream()
{
    m_pEventHandle  =   &gc_oDefaultEventHandle;
    m_skSocket      =   INVALID_SOCKET;
    m_bConnected    =   FALSE;

    m_ulLocalIP     =   INVALID_IP;
    m_ulRemoteIP    =   INVALID_IP;
    m_usRemotePort  =   INVALID_PORT;
}

void    CTcpStream::Open(IEventHandle* pEventHandle, SOCKET skAccepted)
{
    m_pEventHandle  =   pEventHandle;
    m_skSocket      =   skAccepted;
    m_bConnected    =   TRUE;
    return;
}

void    CTcpStream::Open(IEventHandle* pEventHandle, ULONG ulLocalIP, ULONG ulRemoteIP, USHORT usRemotePort)
{
    m_pEventHandle  =   pEventHandle;
    m_skSocket      =   INVALID_SOCKET;
    m_bConnected    =   FALSE;

    m_ulLocalIP     =   ulLocalIP;
    m_ulRemoteIP    =   ulRemoteIP;
    m_usRemotePort  =   usRemotePort;
    return;
}

void    CTcpStream::Accept(SOCKET sk)
{
    m_skSocket  =   sk;

    this->m_pEventHandle->OnOpen();
}

int     CTcpStream::Connect()
{
    if (INVALID_SOCKET != m_skSocket)
    {
        CTcpFDSet   tFDSet;
        tFDSet.clear();
        int iSelectIndex = tFDSet.set(m_skSocket);
        tFDSet.select(100);
        if (tFDSet.isset(iSelectIndex, CTcpFDSet::EVENT_READ) || tFDSet.isset(iSelectIndex, CTcpFDSet::EVENT_READ))
        {
            m_pEventHandle->OnOpen()
            return;
        }

        return;
    }

    //  创建socket
    WSASetLastError(0);
    SOCKET sk       =   socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int iOSError    =   WSAGetLastError();
    if (INVALID_SOCKET == sk)
    {
        return (-2);
    }


    //  设置地址重用标记
    BOOL bReuseAddress = TRUE;
    WSASetLastError(0);
    int iRet        =   setsockopt(sk, SOL_SOCKET, SO_REUSEADDR, &bReuseAddress, sizeof(bReuseAddress));
    int iOSError    =   WSAGetLastError();
    if (0 != iRet)
    {
        closesocket(sk);
        return (-3);
    }


    //  设置允许非阻塞IO
    ULONG ulBlockFlag = TRUE;
    WSASetLastError(0);
    iRet        =   ioctlsocket(sk, FIONBIO, &ulBlockFlag);
    iOSError    =   WSAGetLastError();
    if (0 != iRet)
    {
        closesocket(sk);
        return (-3);
    }


    //  绑定本地端口
    struct sockaddr_in tLocalAddr;
    memset(&tLocalAddr, 0, sizeof(tLocalAddr));
    tLocalAddr.sin_addr.s_addr  =   ulLocalIP;
    tLocalAddr.sin_family       =   AF_INET;
    WSASetLastError(0);
    iRet        =   bind(sk, (sockaddr*)&tLocalAddr, sizeof(tLocalAddr));
    iOSError    =   WSAGetLastError();
    if (0 != iRet)
    {
        closesocket(sk);
        return (-3);
    }


    //  绑定本地端口
    struct sockaddr_in tRemoteAddr;
    memset(&tRemoteAddr, 0, sizeof(tRemoteAddr));
    tRemoteAddr.sin_addr.s_addr =   ulLocalIP;
    tRemoteAddr.sin_port        =   htons(usRemotePort);
    tRemoteAddr.sin_family      =   AF_INET;


    //  链接对端服务器
    WSASetLastError(0);
    iRet        =   connect(sk, &tRemoteAddr, sizeof(tRemoteAddr));
    iOSError    =   WSAGetLastError();
    if (0 == iRet)
    {
        this->Accept(m_skSocket);
        return (-3);
    }

    if ((WSAEINPROGRESS == iOSError) || (WSAEWOULDBLOCK == iOSError))
    {
        return (-4);
    }

    this->m_pEventHandle->OnOpen();

}

void    CTcpStream::Close()
{

}

SOCKET  CTcpStream::GetSocket()const
{
    return (m_skSocket);
}


int     CTcpStream::HandleException()
{
    
}


int     CTcpStream::HandleRead()
{

}


int     CTcpStream::HandleWrite()
{
    TMSGBUF* pMsgBuf = NULL;
    int iRet = m_pEventHandle->OnSend(pMsgBuf);
    if (TCPCOMM_OK != iRet)
    {
        return (iRet);
    }

    if (NULL == pMsgBuf)
    {
        return (0);
    }

    if ((pMsgBuf->pBgn <= pMsgBuf->pReader) && (pMsgBuf->pReader < pMsgBuf->pEnd))
    {
        return (0);
    }

    os::SetLastError(0);
    int iRet        =   os::send(m_skSocket, pMsgBuf->pReader, (pMsgBuf->pEnd - pMsgBuf->pReader));
    int iOSError    =   os::GetLastError();
    if (0 == iRet)
    {
        return (-1);
    }

    if (-1 == iRet)
    {
        return (-1);
    }

    if ((WSAEINPROGRESS == iOSError) || (WSAEWOULDBLOCK == iOSError))
    {
        return (0);
    }

    pMsgBuf->pReader += iRet;
    return (0);
}

