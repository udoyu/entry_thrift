package entry_thrift

import (
	"code.google.com/p/goprotobuf/proto"
	. "github.com/xiying/entry_thrift/gen-go/entry"
	"github.com/xiying/xytool/beego/controller"
	"github.com/xiying/xytool/thrift/thriftst"
	"strconv"
)

func Atol(s string) int64 {
	i, _ := strconv.ParseInt(s, 10, 64)
	return i
}

func Atoi(s string) int {
	i, _ := strconv.Atoi(s)
	return i
}

type EntryClient struct {
	client   *EntryThriftSvrClient
	handlers map[string]func(*controller.Controller)
	//handlers map[string]EntryInterfaceHandler
	//	handlers map[string]func(*controller.Controller)
}

func (this *EntryClient) Init(st *thriftst.ThriftSt) {
	client := NewEntryThriftSvrClientFactory(st.TTransport(),
		st.TProtocolFactory())
	this.client = client
	//        c.handlers = make(map[string]EntryInterfaceHandler)
	this.handlers = make(map[string]func(*controller.Controller))
}

func (this *EntryClient) Handler(ctl *controller.Controller) bool {
	f, ok := this.handlers[ctl.Path()]
	if ok {
		f(ctl)
	}
        this.DefaultHandler(ctl)
	return ok
}

func (this *EntryClient) AddHandler(path string, h func(*controller.Controller)) {
	this.handlers[path] = h
}

func (this *EntryClient) DelHandler(path string) {
	delete(this.handlers, path)
}

func (this *EntryClient) DefaultHandler(ctl *controller.Controller) {
}

func (this *EntryClient) Send(main_cmd, sub_cmd int32,
	req, resp proto.Message) error {
	var err error
	pkg := NewThriftPkg()
	ret := int32(0)
	pkg.Ret = &ret
	pkg.MainCmd = main_cmd
	pkg.SubCmd = sub_cmd

	pkg.Data, err = proto.Marshal(req)
	if err != nil {
		return err
	}
	r, e := this.client.Send(pkg)
	if e != nil {
		return e
	}
	err = proto.Unmarshal(r.Data, resp)
	if err != nil {
		return err
	}
	return nil
}
