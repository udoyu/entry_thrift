// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include <entry_thrift_cli.h>
#include <iostream>
using namespace entry;
using namespace std;

EntryThriftCli cli("localhost", 9001);

int main()
{
    while(1)
    {
        ThriftPkg req;
        ThriftPkg resp;
        req.main_cmd = 100;
        req.buf_data = "hello";
        ThriftClientObj obj(cli.getClient(), cli.getCallback(), cli.getDestroyCallback());
        try
        {
            if (obj) 
            {
                obj->trySend(resp, req);
                cout<<resp.buf_data<<endl;
            }
            else
            {
                cout<<cli.errmsg()<<endl;
            }
        }catch(apache::thrift::transport::TTransportException& e)
        {
            cout<<"exception="<<e.what()<<endl;
            obj.destroy();
        }
        catch(...)
        {
        }
        sleep(10);
    }
    return 0;
}
