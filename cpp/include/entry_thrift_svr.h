#ifndef ENTRY_THRIFT_SVR_H
#define ENTRY_THRIFT_SVR_H

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include "EntryThriftSvr.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

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
    void Start(int port, EntryThriftCmdHandler* h);
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
        printf("Send\n");
    
        int ret = handler_->Exec(req, _return);
        _return.__set_ret(ret);
        _return.__set_main_cmd(req.main_cmd);
        _return.__set_sub_cmd(req.sub_cmd);
    }
private:
    EntryThriftCmdHandler* handler_;

};

inline void EntryThriftSvrManager::Start(int port, EntryThriftCmdHandler* h)
{
    boost::shared_ptr<EntryThriftSvrHandler> handler(new EntryThriftSvrHandler(h));
    boost::shared_ptr<TProcessor> processor(new EntryThriftSvrProcessor(handler));
    boost::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    boost::shared_ptr<TTransportFactory> transportFactory(new TFramedTransportFactory());
    boost::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

    TSimpleServer* server = new TSimpleServer(processor, serverTransport, transportFactory, protocolFactory);
    server->serve();
}

}

#endif
