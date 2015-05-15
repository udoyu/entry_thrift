#ifndef ENTRY_THRIFT_CLI_H
#define ENTRY_THRIFT_CLI_H

#include "EntryThrift.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <atomic>
#include <thrift/transport/TBufferTransports.h>

namespace entry {

typedef boost::shared_ptr<EntryThriftClient> EntryThriftClientPtr;
class EntryThriftClientObj
{
public:
    EntryThriftClientObj(const std::string& ip, uint16_t port);    
    ~EntryThriftClientObj();

    void Send(ThriftPkg& _return, const ThriftPkg& req);
    void send_Send(const ThriftPkg& req);
    void recv_Send(ThriftPkg& _return);

    void trySend(ThriftPkg& _return, const ThriftPkg& req);
    void trySend_Send(const ThriftPkg& req);
    void tryRecv_Send(ThriftPkg& _return);
private:
    void reset();
    void init();
    std::string ip_;
    uint16_t port_;
    boost::shared_ptr<::apache::thrift::transport::TTransport> transport_;
    EntryThriftClientPtr client_;
};
typedef boost::shared_ptr<EntryThriftClientObj> EntryThriftClientObjPtr;

class ThriftClientObj
{
public:
    typedef boost::function<void(const EntryThriftClientObjPtr&)> Callback;
    typedef boost::function<void()> DestroyCallback;
    ThriftClientObj(const EntryThriftClientObjPtr& o, 
                    const Callback& b=Callback(),
                    const DestroyCallback& c = DestroyCallback()):
        obj_(o),
        callback_(b),
        destroyCallback_(c)
    {
    }
    ~ThriftClientObj()
    {
        if (callback_) callback_(obj_);
    }
    void destroy()
    {
        callback_ = Callback();
        if (destroyCallback_) destroyCallback_();
    }
    const EntryThriftClientObjPtr& operator-> () const
    {
        return obj_;
    }
    operator const bool () const
    {
        return obj_.get()!=NULL;
    }
private:
    ThriftClientObj( const ThriftClientObj& ) {}
    const ThriftClientObj& operator=( const ThriftClientObj&) { return *this; }
private:
    EntryThriftClientObjPtr obj_;
    Callback callback_;
    DestroyCallback destroyCallback_;
};


class EntryThriftCli
{
public:
    EntryThriftCli(const std::string& ip, uint16_t port);
    EntryThriftClientObjPtr getClient();
    const std::string& errmsg() const
    {
        return errmsg_;
    }
    const ThriftClientObj::Callback& getCallback() const
    {
        return callback_;
    }
    const ThriftClientObj::DestroyCallback& getDestroyCallback() const
    {
        return destroyCallback_;
    }
    void destroy();
    bool isValid() const
    {
        return status_ == 0;
    }
    void reset();
private:
    void recover(const EntryThriftClientObjPtr& c);
    EntryThriftClientObj* newClient()
    {
        return new EntryThriftClientObj(ip_, port_);
    }
private:
    std::string ip_;
    uint16_t port_;
    std::atomic_int status_;
    std::list<EntryThriftClientObjPtr> clientPool_;
    ThriftClientObj::Callback callback_;
    ThriftClientObj::DestroyCallback destroyCallback_;
    std::string errmsg_;
    boost::mutex mutex_;
};

}
#endif
