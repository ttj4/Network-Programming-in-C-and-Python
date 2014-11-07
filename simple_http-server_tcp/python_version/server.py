from socket import *
import os
import os.path

def extension(name):
	ext = name.split(".")[1]
	if (ext == "jpg"):
		return "image/jpg"
	else:
		return "text/html"
	

print "\n=====simple http server in python======\n"

LOCAL_ADDR = ('127.0.0.1',8010)
MSG_LEN = 200

fd = socket(AF_INET,SOCK_STREAM,0)
fd.bind(LOCAL_ADDR)
fd.listen(10)

while True:
	os.chdir("/home/jinto")
	new_fd,FROM_ADDR = fd.accept()

	pid = os.fork()
	if (pid == 0):
		fd.close()
		r = new_fd.recv(MSG_LEN)
		print r
		http_start = r.find(" HTTP")
		if (http_start == 5):
			ext = extension("index.html")
			f = open("index.html")
			file_read = f.read()
		else:
			for i in r[5:http_start].split("/"):
				if (os.path.isdir(i)):
					os.chdir(i)
				else:
					ext = extension(i)
					f = open(i)
					file_read = f.read()

		s = "HTTP/1.1 200 OK\nContent-length: %d\nContent-Type: %s\n\n%s" % (len(file_read),ext,file_read)
#		print s
		new_fd.send(s)
		new_fd.close()
	else:
		os.wait()
		new_fd.close()

fd.close()


