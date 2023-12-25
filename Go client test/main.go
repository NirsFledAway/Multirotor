package Go_client_test

import (
	"bufio"
	"fmt"
	"net"
)

func main() {
	ln, _ := net.Listen("tcp", "0.0.0.0:8081")
	conn, _ := ln.Accept()
	for {
		msg, _ := bufio.NewReader(conn).ReadString('\n')
		fmt.Println("Message: ", string(msg))
	}
}
