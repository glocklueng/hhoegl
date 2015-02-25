#! /usr/bin/env python3

'''
ST Link Manager

The "ST Link" debug adaptor on the STM32VLDISCOVERY Board mounts by 
default as a usb-storage device. This prevents OpenOCD to work with this
adaptor. To make things work, first unbind ST Link from usb-storage. This
is done automatically when this script is invoked with the -u option. 

H. Hoegl, 2015-02-25
'''

import getopt, os, re, sys

USBSTOR = '/sys/bus/usb/drivers/usb-storage'
SYSDEV = '/sys/bus/usb/devices'

r = re.compile('([\d.-]+):([\d.-]+)')


def usage():
    print("""ST Link Manager
%s [opts]
-h | --help     print options
-s | --search   Search for ST Link adaptor (same as if no option given)
-u | --unbind   Unbind ST Link adaptor
-b | --bind     Bind ST Link adaptor
""" % (sys.argv[0]))


def find_stlink_adaptor():
    for f in os.listdir(SYSDEV):
       if r.match(f):
           p = os.path.join(SYSDEV, f, 'interface')
           if os.path.isfile(p):
               with open(os.path.join(SYSDEV, f, 'interface')) as fi:
                   if fi.read().find('ST Link') == 0:
                       return f
           else:
               pass
    return None


def search_adaptor():
    a = find_stlink_adaptor()
    if a:
        print("Found ST Link adaptor %s" % a)
        if check_usbstorage_bound(a):
            print("device bound to usb-storage")
        else:
            print("device not bound to usb-storage")
    else:
        print("No ST Link adaptor found")


def check_usbstorage_bound(usbid):
    if os.path.isdir(os.path.join(USBSTOR, usbid)):
        return True
    else:
        return False


def unbind():
    usbid = find_stlink_adaptor()
    if not usbid:
        return None
    if check_usbstorage_bound(usbid):
        with open(os.path.join(USBSTOR, 'unbind'), 'w') as fo:
            fo.write(usbid)
        if not os.path.isdir(os.path.join(USBSTOR, usbid)):
            print("unbind successful")
            return usbid
    else:
        print("ST Link is not bound")
        return None


def bind():
    usbid = find_stlink_adaptor()
    if not usbid:
        return None
    if not check_usbstorage_bound(usbid):
        with open(os.path.join(USBSTOR, 'bind'), 'w') as fo:
            fo.write(usbid)
        if os.path.isdir(os.path.join(USBSTOR, usbid)):
            print("bind successful")
            return usbid
    else:
        print("ST Link is already bound")


def main(argv):
   try:
      opts, args = getopt.getopt(argv, "hbus", ["help", "bind", "unbind",
                                                "search"])
   except getopt.GetoptError:
      usage()
      sys.exit(2)
   if not opts:
       search_adaptor()
       print("\n")
       usage()
   for opt, arg in opts:
      if opt in ('-h', '--help'):
         usage()
         sys.exit()
      if opt in ('-b', '--bind'):
         bind()
      if opt in ('-u', '--unbind'):
         unbind()
      if opt in ('-s', '--search'):
         search_adaptor()
         print("\n")
         usage()


if __name__ == "__main__":
    main(sys.argv[1:])


# vim: et sw=4 ts=4
