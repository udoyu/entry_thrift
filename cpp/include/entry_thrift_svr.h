#ifndef ENTRY_THRIFT_SVR_H
#define ENTRY_THRIFT_SVR_H

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include <concurrency/ThreadManager.h>
#include <concurrency/PosixThreadFactory.h>

#include <server/TThreadPoolServer.h>
#include <server/TThreadedServer.h>

#include "EntryThriftSvr.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using namespace ::apache::thrift::concurrency;

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

class EntryThriftSvrHandler : virtual public EntryThriftSvrIf 
{
public:
    EntryThriftSvrHandler(EntryThriftCmdHandler* h) 
    {
        handler_ = h;
    }

    void Send(ThriftPkg& _return, const ThriftPkg& req) 
    {
        // Your implementation goes here
        int ret = handler_->Exec(req, _return);
        _return.__set_ret(ret);
        _return.__set_main_cmd(req.main_cmd);
        _return.__set_sub_cmd(req.sub_cmd);
    }
private:
    EntryThriftCmdHandler* handler_;

};

inline void EntryThriftSvrManager::Start(int thread_cnt)
{
    boost::shared_ptr<EntryThriftSvrHandler> handler(new EntryThriftSvrHandler(cmd_handler_));
    boost::shared_ptr<TProcessor> processor(new EntryThriftSvrProcessor(handler));
    boost::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port_));
    boost::shared_ptr<TTransportFactory> transportFactory(new TFramedTransportFactory());
    boost::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

    boost::shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(8);
    boost::shared_ptr<PosixThreadFactory> threadFactory 
      = shared_ptr<PosixThreadFactory>(new PosixThreadFactory());

    threadManager->threadFactory(threadFactory);
    threadManager->start();
  
    TThreadPoolServer server(processor,
                           serverTransport,
                           transportFactory,
                           protocolFactory,
                           threadManager);

//    TSimpleServer* server = new TSimpleServer(processor, serverTransport, transportFactory, protocolFactory);
    server->serve();
}

}

#endif
