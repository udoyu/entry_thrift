#include "entry_thrift_cli.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
//#include <thrift/transport/TTransportUtils.h>
using namespace entry;
using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

EntryThriftClientObj::EntryThriftClientObj(const std::string& ip, uint16_t port)
{
    boost::shared_ptr<TTransport> socket(new TSocket(ip, port));
    //transport.reset(new TBufferedTransport(socket));
    transport.reset(new TFramedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    transport->open();
    client.reset(new EntryThriftClient(protocol));
}

EntryThriftClientObj::~EntryThriftClientObj()
{
    transport->close();
}

EntryThriftCli::EntryThriftCli(const std::string& ip, uint16_t port) :
    ip_(ip),
    port_(port),
    status_(0)
{
    callback_ = boost::bind(&EntryThriftCli::recover, this, _1);
    destroyCallback_ = boost::bind(&EntryThriftCli::destroy, this);
}

void EntryThriftCli::reset()
{
    boost::unique_lock<boost::mutex> lock(mutex_);
    clientPool_.clear();
    status_ = 0;
    errmsg_.clear();
}

void EntryThriftCli::recover(const EntryThriftClientObjPtr& c)
{
    if (isValid())
    {
        boost::unique_lock<boost::mutex> lock(mutex_);
        clientPool_.push_back(c);
    }
}

void EntryThriftCli::destroy()
{
    boost::unique_lock<boost::mutex> lock(mutex_);
    if (isValid())
    {
        status_ |= 2;
        clientPool_.clear();
    }
}

EntryThriftClientObjPtr EntryThriftCli::getClient()
{
    EntryThriftClientObjPtr c;
    if (isValid())
    {
        {
        boost::unique_lock<boost::mutex> lock(mutex_);
        if (!clientPool_.empty())
        {
            c = clientPool_.front();
            clientPool_.pop_front();
            return c;
        }
        }
    }
    try
    {
        errmsg_.clear();
        c.reset(newClient());
        status_ = 0;
    }
    catch (TException& tx)
    {
        errmsg_ = tx.what();
    }
    catch (...)
    {
        errmsg_ = "connection failed!, other exception";
    }
    if (!c)
    {
        destroy();
    }
    return c;
}


