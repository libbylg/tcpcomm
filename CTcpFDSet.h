#ifndef __CTcpFDSet_H_
#define __CTcpFDSet_H_


struct CTcpFDSet
{
private:
    fd_set  m_fsArray[3];

public:
    enum    ESELECT_EVENT
    {
        EVENT_READ,
        EVENT_WRITE,
        EVENT_EXCEPTION,
    };
    CTcpFDSet()
    {
    }
    void    zero()
    {
        FD_ZERO(m_fsArray[0]);
        FD_ZERO(m_fsArray[1]);
        FD_ZERO(m_fsArray[2]);
    }
    int     set(SOCKET sk)
    {
        FD_SET(sk, m_fsArray[0]);
        FD_SET(sk, m_fsArray[1]);
        FD_SET(sk, m_fsArray[2]);

        return (sk);
    }
    int     select(int iInterval)
    {
        timeb tIntv = {iInterval / 1000000, iInterval % 1000000};
        return (::select(0, &m_fsArray[0], &m_fsArray[1], &m_fsArray[2], &tIntv));
    }
    bool    isset(int iSelectIndex, ESELECT_EVENT eEvent)
    {
        switch (eEvent)
        {
        case EVENT_READ:        return (0 != FD_ISSET(iSelectIndex, m_fsArray[0]));
        case EVENT_WRITE:       return (0 != FD_ISSET(iSelectIndex, m_fsArray[1]));
        case EVENT_EXCEPTION:   return (0 != FD_ISSET(iSelectIndex, m_fsArray[2]));
        default:                return (false);
        }
    }
    size_t max_size()const
    {
        return (sizeof(m_fsArray[0].fd_array)) / sizeof(m_fsArray[0].fd_array[0]));
    }
    size_t size()const
    {
        return (m_fsArray[0].fd_count);
    }
};



#endif//__CTcpFDSet_H_


