#include "entry_thrift_svr.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TNonblockingServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/concurrency/PosixThreadFactory.h>

using namespace std;

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;
using namespace entry;

class EntryThriftHandler : virtual public EntryThriftIf 
{
public:
    EntryThriftHandler(ThrfitCmdHandler* h) 
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
    ThrfitCmdHandler* handler_;

};

void entry::EntryThriftManager::Start(int thread_cnt)
{
    boost::shared_ptr<EntryThriftHandler> handler(new EntryThriftHandler(cmd_handler_));
    boost::shared_ptr<TProcessor> processor(new EntryThriftProcessor(handler));
    boost::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());  
    boost::shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(thread_cnt);  
    boost::shared_ptr<PosixThreadFactory> threadFactory = boost::shared_ptr<PosixThreadFactory > (new PosixThreadFactory());  
    threadManager->threadFactory(threadFactory);  
    threadManager->start();  
    TNonblockingServer server(processor, protocolFactory, port_, threadManager);  
    server.serve();  
}

