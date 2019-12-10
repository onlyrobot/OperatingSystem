# include<iostream>
# include<windows.h>
# include<cstring>
using namespace std;


class CCountUpDown
{
    int m_nValue = 0;
    HANDLE m_hMutexValue;
    HANDLE m_hThreadInc;
    HANDLE m_hThreadDec;
    int m_nAccess;

public:
    CCountUpDown(int m_nAccess)
    {
        this->m_nAccess = m_nAccess;
        m_hMutexValue = CreateMutex(NULL, TRUE, NULL);
        m_hThreadInc = CreateThread(NULL, 0, IncThreadProc, this, 0, NULL);
        m_hThreadDec = CreateThread(NULL, 0, DecThreadProc, this, 0, NULL);
        ReleaseMutex(m_hMutexValue);
    }

    static DWORD WINAPI IncThreadProc(LPVOID pThis)
    {
        ((CCountUpDown*)pThis)->DoCount(+1);
        return 0;
    }

    static DWORD WINAPI DecThreadProc(LPVOID pThis)
    {
        ((CCountUpDown*)pThis)->DoCount(-1);
        return 0;
    }

    void DoCount(int nStep)
    {
        while (m_nAccess > 0)
        {
            cout << "current access: " << m_nAccess << " current value: " \
            << m_nValue << endl;
            WaitForSingleObject(m_hMutexValue, INFINITE);
            m_nValue += nStep;
            --m_nAccess;
            Sleep(500);
            ReleaseMutex(m_hMutexValue);
        }
    }

    ~CCountUpDown()
    {
        CloseHandle(m_hThreadInc);
        CloseHandle(m_hThreadDec);
        CloseHandle(m_hMutexValue);
    }

    virtual void WaitForCompletion()
    {
        WaitForSingleObject(m_hThreadInc, INFINITE);
        WaitForSingleObject(m_hThreadDec, INFINITE);
    }
};


int main()
{
    CCountUpDown ud(10);
    ud.WaitForCompletion();
    return 0;
}