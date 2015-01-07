namespace cpp entry
namespace go entry

struct ThriftPkg{
 1: optional i32 ret,
 2: required i32 main_cmd,
 3: required i32 sub_cmd,
 4: optional binary data
}

service EntryThriftSvr {
  ThriftPkg Send(1: ThriftPkg req);
}

