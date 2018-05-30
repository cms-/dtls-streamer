#!/usr/bin/env python
from os import path
import select
import ssl
import sys
#from logging import basicConfig, DEBUG
#basicConfig(level=DEBUG)  # set now for dtls import code

# Protobuf message:
#	required bytes payload=4;
#	required sfixed32 len=3;
#	required sfixed64 uuid=1;
#	required sfixed32 seq=2;

# Protobuf generated header for packet definition
import packet_pb2

#from socket import socket, AF_INET, SOCK_DGRAM, SHUT_RDWR
from socket import *
from dtls import do_patch
from dtls.err import SSLError, SSL_ERROR_WANT_READ, SSL_ERROR_ZERO_RETURN, ERR_READ_TIMEOUT
do_patch()

cert_path = path.join(path.abspath(path.dirname(__file__)), "/mnt/sync/certs")

############################################################################################
# Pulls files/frames from a C-based DTLS server in < 512 byte UDP packets, reassembled using
# basic per-packet metadata encoded by the server.

def read_frames(pull_next_packet, buf_size=512):
	packets = {}
	pkt = packet_pb2.Packet()
	count = 0
	while True:

		try:
			readable = select.select( [pull_next_packet], [], [], 5 )
			if readable:
				
				packet = pull_next_packet.recv()
				if not packet:
					print "not packet"
					return

				pkt.ParseFromString(packet)

				if not pkt.uuid in packets:
					packets[pkt.uuid] = { 'len': pkt.len, 'data': {pkt.seq: pkt.payload} }

				else:
					packets[pkt.uuid]['data'].update( {pkt.seq: pkt.payload} )

				# # print "totlen: " + str(len(packets[pkt.uuid]['data']))
				# # print "uuid: " + str(pkt.uuid)
				# # print "len: " + str(pkt.len)
				# # print "seq: " + str(pkt.seq)

				if len(packets[pkt.uuid]['data']) == packets[pkt.uuid]['len']:
					yield ''.join( sorted( packets[pkt.uuid]['data'].values() ) )
					del packets[pkt.uuid]

			else:
				print "Quitting time"
				# raise StopIteration
				return
		except Exception as err:
			sys.stderr.write('Iterator ERROR: %s\n' % str(err))
			return

sock = ssl.wrap_socket(socket(AF_INET, SOCK_DGRAM),
	ssl_version="TLSv1.1",
	keyfile=path.join(cert_path, "blab.local.key"),
	certfile=path.join(cert_path, "blab.local.crt"),
	cert_reqs=ssl.CERT_REQUIRED, 
	ca_certs=path.join(cert_path, "ca_auth.crt")
	)
print(gethostname())

sock.connect(("facade.local", 4433))

cert = sock.getpeercert()

print cert

sock.send('GET /?action=stream')

sock.settimeout(5)

frames = []

for jpg_frame in read_frames(sock):
	try:
		frames.append(jpg_frame)
		
	except Exception, err:
		sys.stderr.write('Frame read ERROR: %s\n' % str(err))
		break
	
try:
	sock.unwrap()
except Exception, err:
	sys.stderr.write('Unwrap ERROR: %s\n' % str(err))
# finally:
# 	sock.shutdown(SHUT_RDWR)

print "Number frames: " + str( len(frames) )
print "All done!\n"




