package entry

import (
        "fmt"
	"code.google.com/p/goprotobuf/proto"
	. "github.com/udoyu/entry_thrift/go/entry/gen-go/entry"
	"github.com/udoyu/utils/beego/controller"
	"github.com/udoyu/utils/thrift/thriftst"
)

type EntryClient struct {
        st *thriftst.ThriftSt
	client   *EntryThriftSvrClient
	handlers map[string]func(*controller.Controller)int
	//handlers map[string]EntryInterfaceHandler
	//	handlers map[string]func(*controller.Controller)
}

func (this *EntryClient) Open() error {
    return this.st.Open()
}

func (this *EntryClient) Close() {
    this.st.Close()
}

func (this *EntryClient) Init(st *thriftst.ThriftSt) {
	client := NewEntryThriftSvrClientFactory(st.TTransport(),
		st.TProtocolFactory())
        this.st = st
	this.client = client
	//        c.handlers = make(map[string]EntryInterfaceHandler)
	this.handlers = make(map[string]func(*controller.Controller)int)
}

func (this *EntryClient) Handler(ctl *controller.Controller) bool {
        fmt.Println("path=" + ctl.Path())
	f, ok := this.handlers[ctl.Path()]
	if ok {
		f(ctl)
	}
        this.DefaultHandler(ctl)
	return ok
}

func (this *EntryClient) AddHandler(path string, h func(*controller.Controller)int) {
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
	pkg.MainCmd = main_cmd
	pkg.SubCmd = sub_cmd

	pkg.BufData, err = proto.Marshal(req)
	if err != nil {
		return err
	}
        i := 0
L:
	r, e := this.client.Send(pkg)
	if e != nil {
                this.st.Close()
                this.st.Open()
                if i++;i>3 {
		    return e
                }
                goto L
		return e
	}
	err = proto.Unmarshal(r.BufData, resp)
	if err != nil {
		return err
	}
	return nil
}
