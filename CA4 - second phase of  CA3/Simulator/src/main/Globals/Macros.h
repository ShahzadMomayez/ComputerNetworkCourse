#pragma once

#include <QDebug>
#include <QElapsedTimer>

#define UT_TerminalColorRed     "\033[31m"
#define UT_TerminalColorGreen   "\033[32m"
#define UT_TerminalColorYellow  "\033[33m"
#define UT_TerminalColorBlue    "\033[34m"
#define UT_TerminalColorMagenta "\033[35m"
#define UT_TerminalColorCyan    "\033[36m"
#define UT_TerminalColorReset   "\033[0m"

#define UT_DEBUG                                                                                   \
    qDebug() << UT_TerminalColorYellow << "DEBUG" << UT_TerminalColorReset << "@"                  \
             << UT_TerminalColorCyan << Q_FUNC_INFO << UT_TerminalColorReset <<

#define UT_INFO                                                                                    \
    qDebug() << UT_TerminalColorBlue << "INFO" << UT_TerminalColorReset << "@"                     \
             << UT_TerminalColorCyan << Q_FUNC_INFO << UT_TerminalColorReset <<

#define UT_WARNING                                                                                 \
    qDebug() << UT_TerminalColorYellow << "WARNING" << UT_TerminalColorReset << "@"                \
             << UT_TerminalColorCyan << Q_FUNC_INFO << UT_TerminalColorReset <<

#define UT_ERROR                                                                                   \
    qDebug() << UT_TerminalColorRed << "ERROR" << UT_TerminalColorReset << "@"                     \
             << UT_TerminalColorCyan << Q_FUNC_INFO << UT_TerminalColorReset <<

#define UT_SUCCESS                                                                                 \
    qDebug() << UT_TerminalColorGreen << "SUCCESS" << UT_TerminalColorReset << "@"                 \
             << UT_TerminalColorCyan << Q_FUNC_INFO << UT_TerminalColorReset <<

#define PRINT_IF(CONDITION)                                                                        \
    if(CONDITION) UT_DEBUG

class ScopeTimer
{
public:
    ScopeTimer(QString where) :
        m_where(where)
    {
        m_timer.start();
    }

    ~ScopeTimer()
    {
        if(m_started)
            qDebug() << UT_TerminalColorGreen << "ScopeTimer" << UT_TerminalColorReset << "@"
                     << UT_TerminalColorCyan << m_where << UT_TerminalColorReset << "took"
                     << UT_TerminalColorMagenta << m_timer.nsecsElapsed() / 1'000 << "micro-second"
                     << UT_TerminalColorReset;
    }

    void
    start()
    {
        m_started = true;
    }

private:
    bool          m_started = false;
    QElapsedTimer m_timer;
    QString       m_where;
};

#define SCOPE_CHORNOMETER ScopeTimer ScopeTimer___Timer___AT(Q_FUNC_INFO);

#define SCOPE_CHORNOMETER_AT(WHERE, CLK)                                                           \
    ScopeTimer ScopeTimer___Timer___AT(Q_FUNC_INFO + QString("::") + WHERE + " @CLK " +            \
                                       QString::number(CLK));                                      \
    ScopeTimer___Timer___AT.start();

#define SCOPE_CHORNOMETER_AT_IF(CONDITION, WHERE, CLK)                                             \
    ScopeTimer ScopeTimer___Timer___AT(Q_FUNC_INFO + QString("::") + WHERE + " @CLK " +            \
                                       QString::number(CLK));                                      \
    if(CONDITION) ScopeTimer___Timer___AT.start();

#define BENCHMARK(FUNCTION_CALL)                                                                   \
    {                                                                                              \
        SCOPE_CHORNOMETER;                                                                         \
        FUNCTION_CALL;                                                                             \
    }

#define BENCHMARK_AT(WHERE, CLK, FUNCTION_CALL)                                                    \
    {                                                                                              \
        SCOPE_CHORNOMETER_AT(QString(#FUNCTION_CALL) + " @ " + WHERE, CLK);                        \
        FUNCTION_CALL;                                                                             \
    }

#define BENCHMARK_AT_IF(CONDITION, WHERE, CLK, FUNCTION_CALL)                                      \
    {                                                                                              \
        SCOPE_CHORNOMETER_AT_IF(CONDITION, QString(#FUNCTION_CALL) + " @ " + WHERE, CLK);          \
        FUNCTION_CALL;                                                                             \
    }
