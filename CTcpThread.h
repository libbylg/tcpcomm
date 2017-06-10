#ifndef __CTcpThread_H_
#define __CTcpThread_H_


class   CTcpThread
{
private:
    HANDLE      m_hThreadHandle;
    HANDLE      m_hExitEvent;

    static DWORD gc_ThreadEntry(LPVOID lpThreadParameter)
    {
       CTcpThread* pThread = (CTcpThread*)(lpThreadParameter);
        pThread->Run();
        return (0);
    }

    void    StopImp()
    {
        if (INVALID_HANDLE_VALUE != m_hExitEvent)
        {
            (void)SetEvent(m_hExitEvent);
        }

        if (INVALID_HANDLE_VALUE != m_hThreadHandle)
        {
            if (WAIT_OBJECT_0 != WaitForSingleObject(this->m_hThreadHandle, 1000))
            {
                TerminateThread(m_hThreadHandle);
            }

            (void)CloseHandle(m_hThreadHandle);
            m_hThreadHandle =   INVALID_HANDLE_VALUE;
        }

        if (INVALID_HANDLE_VALUE != m_hExitEvent)
        {
            (void)CloseHandle(m_hThreadHandle);
            m_hThreadHandle =   INVALID_HANDLE_VALUE;
        }
    }
public:
    CTcpThread()
    {
        m_hThreadHandle =   INVALID_HANDLE_VALUE;
        m_hExitEvent    =   INVALID_HANDLE_VALUE;
    }
    virtual ~CTcpThread()
    {
        StopImp();
    }
    int     Start(size_t siStackSize = 1 * 1024 * 1024)
    {
        m_hExitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
        if (INVALID_HANDLE_VALUE == this->m_hThreadHandle)
        {
            return (-2);
        }

        this->m_hThreadHandle = CreateThread(NULL, siStackSize, CTcpThread<HOSTCLASS>::gc_ThreadEntry, NULL, CREATE_SUSPENDED, NULL);
        if (INVALID_HANDLE_VALUE == this->m_hThreadHandle)
        {
            CloseHandle(m_hExitEvent);
            m_hExitEvent = INVALID_HANDLE_VALUE;
            return (-3);
        }

        (void)ResumeThread(this->m_hThreadHandle);
        return (0);
    }
    void    Stop()
    {
        this->StopImp();
    }
    BOOL    WaitExitEvent(int iWaitInterval)
    {
        return (WAIT_OBJECT_0 == WaitForSingleObject(iWaitInterval));
    }
    
    virtual void    Run()
    {
        (void)this->WaitExitEvent(INFINITE);
    }
};


#endif//__CTcpThread_H_


