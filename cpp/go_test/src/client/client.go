package main

import (
	"fmt"
	"gen-go/entry"
	"git.apache.org/thrift.git/lib/go/thrift"
	"net"
	"os"
)

func main() {
	//transportFactory := thrift.NewTBufferedTransportFactory(1024)
	transportFactory := thrift.NewTFramedTransportFactory(thrift.NewTTransportFactory())
	protocolFactory := thrift.NewTBinaryProtocolFactoryDefault()

	transport, err := thrift.NewTSocket(net.JoinHostPort("127.0.0.1", "9001"))
	if err != nil {
		fmt.Fprintln(os.Stderr, "error resolving address:", err)
		os.Exit(1)
	}

	useTransport := transportFactory.GetTransport(transport)
	client := entry.NewEntryThriftSvrClientFactory(useTransport, protocolFactory)
	func() {
		if err := transport.Open(); err != nil {
			fmt.Fprintln(os.Stderr, "Error opening socket to 127.0.0.1:9090", " ", err)
			os.Exit(1)
		}
	}()
	fmt.Println("aaaaaaaaaa")
	pkg := entry.NewThriftPkg()
	ret := int32(0)
	pkg.Ret = &ret
	ret = 1
	pkg.MainCmd = &ret
	ret = 2
	pkg.SubCmd = &ret
	r, e := client.Send(pkg)
	if e != nil {
		fmt.Println(e.Error())
	}
	fmt.Printf("r.main_cmd=%d|r.data.len=%d\n", *r.MainCmd, len(r.Data))
	fmt.Println("bbbbbbbbbb")

}
