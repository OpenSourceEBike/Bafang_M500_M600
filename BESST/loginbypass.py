# written by elenhinan, 24.11.2016

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
			#user type: 1 brand, 2 dealer, 3 assembler, 4 manufacturer, 5 terminal. 4 for tool access, 3 for diagnostics
			userdata = json.dumps( {'code':0,'data':{'user':{'status':1, 'authList':['productMenu', 'productPart', 'productTraining', 'productEditCategory', 'productProductAdd', 'productProductEdit', 'productProductDelete', 'productProductBomView', 'productProductBomEdit', 'productProductBomDelete', 'productProductTrainingEdit', 'productProductTrainingDelete', 'productSecondaryMotor', 'productSecondaryHmi', 'productSecondaryBattery', 'productSecondarySensor', 'productSecondaryController', 'productSecondaryConnector', 'productSecondaryCharger', 'productSecondaryTool', 'productionMenu', 'productionAdd', 'productionEdit', 'productionDelete', 'productionProduce', 'productionReport', 'orderMenu', 'orderAdd', 'orderEdit', 'orderDelete', 'orderReplace', 'itemMenu', 'itemAdd', 'itemEdit', 'itemDelete', 'itemCopy', 'itemBomEdit', 'itemBomDelete', 'accountMenu', 'accountDelete', 'ticketMenu', 'ticketDelete', 'ticketSC', 'ticketSCAdd', 'ticketSCEdit', 'ticketComplaint', 'ticketTask', 'brandMenu', 'brandAdd', 'brandEdit', 'brandDelete', 'objectMenu', 'objectDelete', 'objectReplace', 'toolMenu', 'diagnoseMenu', 'errorShootingMenu', 'dealerMenu', 'dealerDelete', 'dealerInvitation', 'oemMenu', 'oemDelete', 'oemInvitation', 'fileFeedback', 'fileFeedbackDelete', 'fileFeedbackDetail', 'fileFeedbackAdd', 'fileFeedbackEdit', 'material'], 'authListInDB':['productMenu', 'productPart', 'productTraining', 'productEditCategory', 'productProductAdd', 'productProductEdit', 'productProductDelete', 'productProductBomView', 'productProductBomEdit', 'productProductBomDelete', 'productProductTrainingEdit', 'productProductTrainingDelete', 'productSecondaryMotor', 'productSecondaryHmi', 'productSecondaryBattery', 'productSecondarySensor', 'productSecondaryController', 'productSecondaryConnector', 'productSecondaryCharger', 'productSecondaryTool', 'productionMenu', 'productionAdd', 'productionEdit', 'productionDelete', 'productionProduce', 'productionReport', 'orderMenu', 'orderAdd', 'orderEdit', 'orderDelete', 'orderReplace', 'itemMenu', 'itemAdd', 'itemEdit', 'itemDelete', 'itemCopy', 'itemBomEdit', 'itemBomDelete', 'accountMenu', 'accountDelete', 'ticketMenu', 'ticketDelete', 'ticketSC', 'ticketSCAdd', 'ticketSCEdit', 'ticketComplaint', 'ticketTask', 'brandMenu', 'brandAdd', 'brandEdit', 'brandDelete', 'objectMenu', 'objectDelete', 'objectReplace', 'toolMenu', 'diagnoseMenu', 'errorShootingMenu', 'dealerMenu', 'dealerDelete', 'dealerInvitation', 'oemMenu', 'oemDelete', 'oemInvitation', 'fileFeedback', 'fileFeedbackDelete', 'fileFeedbackDetail', 'fileFeedbackAdd', 'fileFeedbackEdit', 'material'],'org': {'id': 25, 'type': 3}},'token':123}} )
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