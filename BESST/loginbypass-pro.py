# written by elenhinan, 24.11.2016
# revision by CiDi, 11.02.2021
# revision for besst pro by przymaras, 27.07.2022

from http.server import BaseHTTPRequestHandler, HTTPServer
import socketserver
import simplejson as json
import socket

class BESST_server(BaseHTTPRequestHandler):
	def do_GET(self):
		#data_string = self.rfile.read(int(self.headers['Content-Length']))
		print("got get from BESST (%s)"%self.path)

		if '/client/1/user/login' in self.path:
			print("login accepted ;)")
			self.send_response(200)
			self.send_header('Content-type', 'application/json\n')
			self.end_headers()
			#user type: 1 brand, 2 dealer, 3 assembler, 4 component, 5 service center, 6 dealer for tool access
			userdata = json.dumps( {'code':0,'data':{'account_type':[1,2,3,6,7],'user':{'status':6,'name':'hacked','org': {'id': 1340, 'type': 6}},'token':'9e393b1ae594706896a9b037dc0dd9b9'}} )
			self.wfile.write(userdata.encode('utf-8'))


def run(handler_class=BESST_server, port=80):
        ip_lookup = socket.gethostbyname('test.api.besst.bafang-service.com')
        print(("test.api.besst.bafang-service.com forwards to %s"%ip_lookup))
        httpd = socketserver.TCPServer(("", port), handler_class)
        print("Starting httpd...")
        httpd.serve_forever()


if __name__ == "__main__":
	run()