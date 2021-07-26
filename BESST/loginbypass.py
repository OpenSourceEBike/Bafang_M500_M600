# written by elenhinan, 24.11.2016
# revision by CiDi, 11.02.2021

from http.server import BaseHTTPRequestHandler, HTTPServer
import socketserver
import simplejson as json
import socket

class BESST_server(BaseHTTPRequestHandler):
	def do_POST(self):
		data_string = self.rfile.read(int(self.headers['Content-Length']))
		print("got post from BESST (%s)"%data_string)

		if b"password" in data_string:
			print("login accepted ;)")
			self.send_response(200)
			self.send_header('Content-type', 'application/json;charset:UTF-8\r\n')
			self.end_headers()
			#user type: 1 brand, 2 dealer, 3 assembler, 4 component, 5 service center, 6 dealer for tool access
			userdata = json.dumps( {'code':0,'data':{'account_type':[1,2,3,4,5,6],'user':{'status':6,'org': {'id': 1212, 'type': 6}},'token':123}} )
			self.wfile.write(userdata.encode('utf-8'))
		else:
			self.send_response(200)
			self.send_header('Content-type', 'application/json;charset:UTF-8\r\n')
			self.end_headers()
			userdata = json.dumps( {'code':0} )
			self.wfile.write(userdata.encode('utf-8'))


def run(handler_class=BESST_server, port=80):
        ip_lookup = socket.gethostbyname('api.besst.bafang-service.com')
        print(("test.api.besst.bafang-service.com forwards to %s"%ip_lookup))
        httpd = socketserver.TCPServer(("", port), handler_class)
        print("Starting httpd...")
        httpd.serve_forever()


if __name__ == "__main__":
	run()
