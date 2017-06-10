#ifndef __tcpcomm_default_object_H_
#define __tcpcomm_default_object_H_

#include    "tcpcomm.h"

class   CDefaultEventHandle :   public  IEventHandle
{
private:
    UCHAR   m_szBuffer[1];
    TMSGBUF m_tSendBuffer;

public:
    CDefaultEventHandle()
    {
        m_szBuffer[0]           = '\0';
        m_tSendBuffer.pBgn      =   m_szBuffer;
        m_tSendBuffer.pEnd      =   m_szBuffer;
        m_tSendBuffer.pReader   =   m_szBuffer;
    }
    virtual void        OnOpen(ISplitter& oSplitter, ULONG ulLocalIP, USHORT usLocalPort, ULONG ulRemote, USHORT usRemotePort)
    {
    }
    virtual void        OnClose()
    {
    }
    virtual void        OnException()
    {
    }
    virtual TMSGBUF&    OnSend()
    {
    }
    virtual void        OnRecv()
    {
    }
    virtual void        OnSplit(const IContainer* pContainer)
    {
    }
    virtual void        OnJoin(const IContainer* pContainer)
    {
    }
};


class CDefaultAcceptor
{
public:
    IEventHandle*   OnAccept(ULONG ulLocalIP, USHORT usLocalPort, ULONG ulRemote, USHORT usRemotePort)
    {
        return (NULL);
    }
};


class CDefaultThreadHost
{
public:
    void Run()
    {
    }
};

#endif//__tcpcomm_default_object_H_

