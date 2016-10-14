#-*- coding:utf-8 -*-

from SocketServer import ThreadingTCPServer, StreamRequestHandler
import traceback

class MyStreamRequestHandlerr(StreamRequestHandler):
    def handle(self):
        while True:
            try:
                data = self.rfile.readline().strip()
                print "receive from (%r):%r" % (self.client_address, data)
                self.wfile.write(data.upper())
            except:
                traceback.print_exc()
                break

if __name__ == "__main__":
    host = "127.0.0.1"       
    port = 25531     
    addr = (host, port)
    
    #ThreadingTCPServer从ThreadingMixIn和TCPServer继承
    #class ThreadingTCPServer(ThreadingMixIn, TCPServer): pass
    server = ThreadingTCPServer(addr, MyStreamRequestHandlerr)
    server.serve_forever()
