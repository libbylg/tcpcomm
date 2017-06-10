#ifndef __CTcpError_H_
#define __CTcpError_H_

struct  CTcpError : public IError
{
private:
    int     m_iReturn;
    int     m_iOSError;
    int     m_iInnerError;
    CHAR    m_szDesc[200];

public:
    virtual int         GetReturn()const
    {
        return (m_iReturn);
    }
    virtual int         GetOSError()const
    {
        return (m_iOSError);
    }
    virtual int         GetInnerError()const
    {
        return (m_iInnerError);
    }
    virtual const CHAR* GetDesc()const 
    {
        return (m_szDesc);
    }
    virtual bool        IsSuccess()const
    {
        return ((0 == m_iReturn) && () && (0 == m_iOSError) && (0 == m_iInnerError));
    }
    CTcpError(int iRet = 0, int iOSErr = 0, int iInnerErr = 0, const CHAR* pFormat, ...)
    {
        m_iReturn       =   iRet;
        m_iOSError      =   iOSErr;
        m_iInnerError   =   iInnerErr;

        if ((NULL != pFormat) && ('\0' == pFormat[0]))
        {
            va_list valist;
            va_start(valist, pFormat);
            (void)_vsnprintf(m_szDesc, sizeof(m_szDesc), pFormat, valist);
            va_end(valist);
        }
        m_szDesc[sizeof(m_szDesc) - 1] = '\0';
    }
};


#endif//__CTcpError_H_


