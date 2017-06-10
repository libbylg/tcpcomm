#ifndef __tcpcomm_H_
#define __tcpcomm_H_


#define TCPCOMM_OK      0


struct  TMSGBUF
{
    UCHAR*  pBgn;
    UCHAR*  pEnd;
    UCHAR*  pReader;
};


struct  IError
{
    virtual int         GetReturn()const = 0;
    virtual int         GetOSError()const = 0;
    virtual int         GetInnerError()const = 0;
    virtual const CHAR* GetDesc()const = 0;
    virtual bool        IsSuccess()const = 0;
};


struct  ISelector
{
    virtual void Split() = 0;
};


struct  ITcpStream
{
    virtual BOOL    GetLoadlAddress(ULONG& ulLocalIP, ULONG& ulLocalIP)     = 0;
    virtual BOOL    GetLoadlAddress(ULONG& ulRemoteIP, ULONG& ulRemotePort) = 0;
    virtual int     Split() = 0;
};


struct  IEventHandle
{
    virtual void        OnOpen(ITcpStream& oStream, IError& oError) = 0;
    virtual void        OnClose() = 0;

    virtual void        OnException(IError& oError) = 0;
    virtual TMSGBUF&    OnSend() = 0;
    virtual void        OnRecv() = 0;

    virtual void        OnSplit() = 0;
    virtual void        OnJoin(const IContainer* pContainer) = 0;
};

// 
// struct  IConnector
// {
//     virtual int             OpenConnection(ULONG ulLocalIP, USHORT usLocalPort, ULONG ulRemote, USHORT usRemotePort, IEventHandle* pEventHandle) = 0;
// };


#endif//__tcpcomm_H_

