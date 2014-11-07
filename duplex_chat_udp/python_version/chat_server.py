#server
from socket import *
import os
import os.path
import sys
import select 

LOCAL_ADDR = ("127.0.0.1",4050)
DEST_ADDR = ("127.0.0.1",4060)
MSG_LEN = 200

fd = socket(AF_INET,SOCK_DGRAM,0)

fd.bind(LOCAL_ADDR)

master = [sys.stdin,fd]
while(1):
	rlist,wlist,elist = select.select(master,[],[])

	for s in rlist:
		if s == fd:
			inbox = fd.recvfrom(MSG_LEN)
			print inbox[0]
			print "msg recived from client"
		elif s == sys.stdin:
			msg = raw_input()
			fd.sendto(msg,DEST_ADDR)
			print "msg send to client"


