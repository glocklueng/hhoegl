#!/usr/bin/python

import os, sys, telnetlib

HOST = "127.0.0.1"
PORT = 4444
TIMEOUT = 5

def usage():
    print "usage: %s pgm.bin" % sys.argv[0]
    sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) == 1:
        usage()
    else:
        fn = os.path.abspath(sys.argv[1])
        tn = telnetlib.Telnet(HOST, PORT, TIMEOUT)
        tn.write('reset halt\n');
        tn.write('flash probe 0\n');
        tn.write('flash write_image erase %s 0x08000000\n' % fn);
        tn.write('reset\n');
        tn.write('exit\n');
        tn.write('\n');
        print tn.read_all()

