#ifndef ENTRY_THRIFT_SVR_H
#define ENTRY_THRIFT_SVR_H

#include "EntryThriftSvr.h"

namespace entry {

class EntryThriftCmdHandler
{
public:
    virtual int Exec(const entry::ThriftPkg& req,
                     entry::ThriftPkg& resp) = 0;
};

class EntryThriftSvrManager
{
public:
    EntryThriftSvrManager(int port, EntryThriftCmdHandler* h)
    : port_(port), cmd_handler_(h){}
    void Start(int thread_cnt = 8);
private:
    int port_;
    EntryThriftCmdHandler* cmd_handler_;
};


}

#endif
