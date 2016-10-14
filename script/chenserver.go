package main

import (
	"fmt"
	"log"
	"net"
)

func startServer() {
	listener, err := net.Listen("tcp4", "127.0.0.1:25531")
	checkError(err)
	fmt.Println("建立成功!")
	for {
		conn, err := listener.Accept()
		checkError(err)
		go doServerStuff(conn)
	}
}

func doServerStuff(conn net.Conn) {
	nameInfo := make([]byte, 512)
	_, err := conn.Read(nameInfo)
	checkError(err)

	for {
		buf := make([]byte, 512)
		_, err := conn.Read(buf)
		flag := checkError(err)
		if flag == 0 {
			break
		}
		fmt.Println(string(buf))
	}
}


func checkError(err error) int {
	if err != nil {
		if err.Error() == "EOF" {
			return 0
		}
		log.Fatal("an error!", err.Error())
		return -1
	}
	return 1
}
func main() {
	startServer()
}
