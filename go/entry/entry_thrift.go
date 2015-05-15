package entry

import (
	"code.google.com/p/goprotobuf/proto"
	. "github.com/udoyu/entry_thrift/go/entry/gen-go/entry"
	"github.com/udoyu/utils/thrift/thriftst"
)

type EntryClient struct {
	st     *thriftst.ThriftSt
	client *EntryThriftClient
}

func (this *EntryClient) Copy() *EntryClient {
	c := &EntryClient{}
	c.Init(this.st)
	return c
}

func (this *EntryClient) Reset() error {
	this.Close()
	this.Open()
	this.Init(this.st)
	return nil
}

func (this *EntryClient) Open() error {
	return this.st.Open()
}

func (this *EntryClient) Close() {
	this.st.Close()
}

func (this *EntryClient) Init(st *thriftst.ThriftSt) {
	if st != nil {
		client := NewEntryThriftClientFactory(st.TTransport(),
			st.TProtocolFactory())
		this.st = st
		this.client = client
	}
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
		e1 := this.Reset()
		if e1 != nil {
			println("err=", e1.Error())
		}
		if i++; i > 3 {
			return e
		}
		goto L
		return e
	}
	err = proto.Unmarshal(r.BufData, resp)
	return err
}
