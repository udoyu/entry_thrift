namespace cpp entry
namespace go entry

struct ThriftPkg{
 1: i32 ret,
 2: i32 main_cmd,
 3: i32 sub_cmd,
 4: binary buf_data
 5: string str_data
}

service EntryThrift {
  ThriftPkg Send(1: ThriftPkg req);
}

