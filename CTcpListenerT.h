#ifndef __CTcpListenerT_H_
#define __CTcpListenerT_H_

#include "CTcpThreadT.h"
#include "tcpcomm_default_object.h"

template<class ACCEPTCLASS = CDefaultAcceptor>
class CTcpListenerT : public CTcpThread
{
private:
    ULONG           m_ulListenIP;
    ULONG           m_ulListenPort;
    SOCKET          m_skSocket;
    ACCEPTCLASS*    m_pAcceptor;
    static CDefaultAcceptor gc_oDefaultAcceptor;


private:
    virtual void    Run();  //  实现 CTcpThread 所依赖的接口

public:
    CTcpListenerT();
    ~CTcpListenerT();
    int     Create(ULONG ulLocalIP, USHORT usLocalPort, ACCEPTCLASS& oAcceptor);
};


template<class ACCEPTCLASS>
CDefaultAcceptor CTcpListenerT<ACCEPTCLASS>::gc_oDefaultAcceptor;


template<class ACCEPTCLASS>
CTcpListenerT<ACCEPTCLASS>::CTcpListenerT()
{
    m_ulListenIP    =   INVALID_IP;
    m_ulListenPort  =   INVALID_PORT;
    m_skSocket      =   INVALID_SOCKET;
    m_pAcceptor     =   &CTcpListenerT<ACCEPTCLASS>::gc_oDefaultAcceptor;
}

template<class ACCEPTCLASS>
CTcpListenerT<ACCEPTCLASS>::~CTcpListenerT()
{
}

template<class ACCEPTCLASS>
int     CTcpListenerT<ACCEPTCLASS>::Create(ULONG ulLocalIP, USHORT usLocalPort, ACCEPTCLASS& oAcceptor)
{
    //  参数校验
    if (INVALID_IP == ulLocalIP)
    {
        return (-1);
    }

    if (INVALID_PORT == usLocalPort)
    {
        return (-2);
    }

    //  保存最重要的两个参数
    m_ulListenIP    =   ulLocalIP；
    m_ulListenPort  =   usLocalPort;
    m_pAcceptor     =   &oAcceptor;

    //  启动线程
    int iRet = this->Start();
    if (0 != iRet)
    {
        return (-3);
    }

    return (TCPCOMM_OK);
}


template<class ACCEPTCLASS>
void    CTcpListenerT<ACCEPTCLASS>::Run()
{
    //  等待一下退出事件
    while (FALSE == this->WaitExitEvent(0))
    {
        if (INVALID_SOCKET == this->m_skSocket)
        {
            //  创建socket
            SOCKET sk = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (INVALID_SOCKET == sk)
            {
                continue;
            }

            //  绑定监听端口
            struct sockaddr_in tLocalAddr;
            memset(&tLocalAddr, 0, sizeof(tLocalAddr));
            tLocalAddr.sin_addr.s_addr  =   ulLocalIP;
            tLocalAddr.sin_port         =   htons(usLocalPort);
            tLocalAddr.sin_family       =   AF_INET;
            int iRet = bind(sk, (sockaddr*)&tLocalAddr, sizeof(tLocalAddr));
            if (0 != iRet)
            {
                closesocket(sk);
                continue;
            }

            //  监听
            int iRet = listen(m_skSocket, 0);
            if (SOCKET_ERROR == iRet)
            {
                closesocket(m_skSocket);
                continue;
            }
        }

        //  试图接受一个新连接
        sockaddr_in tRemoteAddr;
        int         iRemoteAddrLen = sizeof(tRemoteAddr);
        memset(&tRemoteAddr, 0, iRemoteAddrLen);
        SOCKET sk = accept(m_skSocket, &tRemoteAddr, iRemoteAddrLen);
        if (INVALID_SOCKET == sk)
        {
            continue;
        }

        //  取socket的本地地址
        sockaddr_in tLocalAddr;
        int         iLocalAddrLen = sizeof(tLocalAddr);
        memset(&tLocalAddr, 0, iLocalAddrLen);
        int iRet = getsockname(sk, (sockaddr*)(&tLocalAddr), &iLocalAddrLen);

        //  通知Acceptor接受这个事件句柄
        IEventHandle* pEventHandle = m_pAcceptor->OnAccept(tLocalAddr.sin_addr.s_addr, ntohs(tLocalAddr.sin_port), tRemoteAddr.sin_addr.s_addr, ntohs(tRemoteAddr.sin_port));
        if (NULL == pEventHandle)
        {
            closesocket(sk);
            continue;
        }

        //  创建流对象，并打开
        CTcpStream* pStream = new CTcpStream;
        pStream->Open(pEventHandle, sk);
    }
}

// 
// template<class ACCEPTCLASS>
// IEventHandle*   OnAccept(ULONG ulLocalIP, USHORT usLocalPort, ULONG ulRemote, USHORT usRemotePort)
// {
// 
// }


#endif//__CTcpListenerT_H_

