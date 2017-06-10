#ifndef __os_H_
#define __os_H_

#define     INVALID_IP      0xFFFFFFFF
#define     INVALID_PORT    0xFFFF

class CCRITICALSECTION
{
private:
    CRITICAL_SECTION    m_cs;
public:
    CCRITICALSECTION()
    {
        InitializeCriticalSection(&m_cs);
    }
    ~CCRITICALSECTION()
    {
        DeleteCriticalSection(&m_cs);
    }
    void Lock()
    {
        EnterCriticalSection(&m_cs);
    }
    void Unlock()
    {
        LeaveCriticalSection(&m_cs);
    }

    class AUTO_SCOPE
    {
    private:
        CCRITICALSECTION&   m_cs;
    public:
        AUTO_SCOPE(CCRITICALSECTION& cs) : m_cs(cs)
        {
            m_cs.Lock();
        }
        ~AUTO_SCOPE()
        {
            m_cs.Unlock();
        }
    };
};

#endif//__os_H_

