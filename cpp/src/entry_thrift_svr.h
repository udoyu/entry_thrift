#ifndef THRIFT_INTERFACE_SVR_H
#define THRIFT_INTERFACE_SVR_H

#include "EntryThrift.h"

namespace entry {

class EntryThriftCmdHandler
{
public:
    virtual int Exec(const entry::ThriftPkg& req,
                     entry::ThriftPkg& resp) = 0;
};

class EntryThriftManager
{
public:
    EntryThriftManager(int port, EntryThriftCmdHandler* h)
    : port_(port), cmd_handler_(h){}
    void Start(int thread_cnt = 8);
private:
    int port_;
    EntryThriftCmdHandler* cmd_handler_;
};

}

#endif
