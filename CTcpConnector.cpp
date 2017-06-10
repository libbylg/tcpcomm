#include "CTcpConnector.h"
CTcpConnector::CTcpConnector()
{

}

CTcpConnector::~CTcpConnector()
{

}

int     CTcpConnector::Create()
{

}

void    CTcpConnector::Destroy()
{

}

int     CTcpConnector::OpenConnection(ULONG ulLocalIP, ULONG ulRemoteIP, USHORT usRemotePort, IEventHandle* pEventHandle)
{
    if (INVALID_IP == ulLocalIP)
    {
        return (-1);
    }

    if (INVALID_IP == ulRemoteIP)
    {
        return (-2);
    }

    if (INVALID_PORT == usRemotePort)
    {
        return (-3);
    }

    if (NULL == pEventHandle)
    {
        return (-4);
    }

    CTcpStream* pStream = new CTcpStream;
    int iRet = pStream->Create(ulLocalIP, ulRemoteIP, usRemotePort, pEventHandle);
    if (TCPCOMM_OK != iRet)
    {
        return (iRet);
    }

    CCRITICALSECTION::AUTO_SCOPE auto_cs(m_csReadyStreamList);
    POSITION pos = m_lsReadyStreamList.AddTail(pStream);
    if (NULL == pos)
    {
        delete pStream;
        return (-5);
    }

    return (TCPCOMM_OK);
}

void CTcpConnector::MerageReadyStream()
{
    CCRITICALSECTION::AUTO_SCOPE auto_cs(m_csReadyStreamList);
    m_lsConnectStreamList.AddTail(m_lsReadyStreamList);
    m_lsReadyStreamList.RemoveAll();
}



void CTcpConnector::Run()
{
    while (FALSE == this->WaitExitEvent(0))
    {
        TSelectTable tSelectTable;
        tSelectTable.tFDSet.zero();

        tSelectTable.iSize = 0;
        
        POSITION pos = m_lsConnectStreamList.GetHeadPosition();
        while (pos)
        {
            POSITION posCur = pos;
            CTcpStream* pStream = m_lsConnectStreamList.GetNext(pos);
            if (NULL == pStream)
            {
                continue;
            }

            tSelectTable.pStream[tSelectTable.iSize]        =   pStream;
            tSelectTable.iSelectIndex[tSelectTable.iSize]   =   tSelectTable.tFDSet.set(pStream->GetSocket());
            tSelectTable.pPos[tSelectTable.iSize]           =   posCur;
            tSelectTable.iSize++;
        }

        if (0 == tSelectTable.iSize)
        {
            continue;
        }

        int iRet = tSelectTable.tFDSet.select();
        if (0 == iRet)
        {
            continue;
        }

        if (-1 == iRet)
        {
            continue;
        }

        for (int i = 0; i < tSelectTable.iSize; i++)
        {
            tSelectTable.HandleSelectEvent(tSelectTable.tFDSet);
        }

    }
}



