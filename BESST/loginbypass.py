# written by elenhinan, 24.11.2016
# revision by CiDi, 11.02.2021
# revision by Ornias, early-mid 2022
# revision for besst pro by przymaras, 27.07.2022

from http.server import BaseHTTPRequestHandler, HTTPServer
import socketserver
import json
import socket

bafang_api_domains = [
    'api.besst.bafang-service.com',
	'test.api.besst.bafang-service.com',
	'eu-central-1.api.besst.bafang-service.com',
	'bafang-besst.oss-cn-shanghai.aliyuncs.com'
]

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


	def do_POST(self):
		data_string = self.rfile.read(int(self.headers['Content-Length']))
		print("got post from BESST (%s)"%data_string)

		if self.path == '/client/1/user/login':
			print("login accepted ;)")
			self.send_response(200)
			self.send_header('Content-type', 'application/json;charset:UTF-8\r\n')
			self.end_headers()
			#user type: 1 brand, 2 dealer, 3 assembler, 4 component, 5 service center, 6 dealer for tool access
			userdata = json.dumps( {'code':0,'data':{'account_type':[1,2,3,6,7],'user':{'status':6,'name':'hacked','org': {'id': 1340, 'type': 6}},'token':'9e393b1ae594706896a9b037dc0dd9b9'}} )
			self.wfile.write(userdata.encode('utf-8'))
		elif self.path == '/client/1/auth/detail':
			print("sending auth details")
			self.send_response(200)
			self.send_header('Content-type', 'application/json;charset:UTF-8\r\n')
			self.end_headers()
			userdata = json.dumps( {'code':0,'data':{'auth':['product.menu','product.part','product.training','product.edit_category','product.product.add','product.product.edit','product.product.delete','product.product.bom.view','product.product.bom.edit','product.product.bom.delete','product.product.training.edit','product.product.training.delete','product_secondary_motor','product_secondary_hmi','product_secondary_battery','product_secondary_sensor','product_secondary_controller','product_secondary_connector','product_secondary_charger','product_secondary_tool','production.menu','production.add','production.edit','production.delete','production.produce','production.report','order.menu','order.add','order.edit','order.delete','order.replace','item.menu','item.add','item.edit','item.delete','item.copy','item.bom.edit','item.bom.delete','account.menu','account.delete','ticket.menu','ticket.delete','ticket.service_center','ticket.service_center.add','ticket.service_center.edit','ticket.complaint','ticket.task','brand.menu','brand.add','brand.edit','brand.delete','object.menu','object.delete','object.replace','tool.menu','diagnose.menu','error_shooting.menu','dealer.menu','dealer.delete','dealer.invitation','oem.menu','oem.delete','oem.invitation','file.error','file.error.delete','file.error.detail','file.error.add','file.error.edit','auth_material',]}} )
			self.wfile.write(userdata.encode('utf-8'))
		else:
			self.send_response(200)
			self.send_header('Content-type', 'application/json;charset:UTF-8\r\n')
			self.end_headers()
			userdata = json.dumps( {'code':0} )
			self.wfile.write(userdata.encode('utf-8'))


def run(handler_class=BESST_server, port=80):
        for d in bafang_api_domains:
            print(f"{d} forwards to {socket.gethostbyname(d)}")

        httpd = socketserver.TCPServer(("", port), handler_class)
        print("Starting httpd...")
        httpd.serve_forever()

if __name__ == "__main__":
	run()
