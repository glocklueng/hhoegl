# sam7.py

# http://pexpect.sourceforge.net
# child.expect(<str>, timeout=<secs>)

import pexpect, re, string
import sys

SLOW_CLOCK = 32000 # plus/minus 10 kHz

MAIN_CLOCK = 18432000 

# Clock Generator (contained in Power Management Controller (PMC))
CKGR_MOR = 0xFFFFFC20 # rw - Main Oscillator Register
CKGR_MCFR = 0xFFFFFC24 # r - Main Clock Frequency Register


# USBDIV[29:28]
# MUL[26:16]
# OUT[15:14]
# PLLCOUNT[13:8]
# DIV[7:0]
CKGR_PLLR = 0xFFFFFC2C # rw - Main Register

class CKGR_PLLR:
    def __init__(self, val):
        self.val = val
        self.usbdiv = 0
        self.mul = 0
        self.out = 0
        self.pllcount = 0
        self.div = 0
        self.get_fields()
    def get_fields(self):
        self.div = get_bit_region(self.val, 7, 0)
        self.pllcount = get_bit_region(self.val, 13, 8)
        self.out = get_bit_region(self.val, 15, 14)
        # pll output freq = pll input freq * (mul + 1)
        self.mul = get_bit_region(self.val, 26, 16)
        self.usbdiv = get_bit_region(self.val, 29, 28)
    def __str__(self):
        return "CKGR_PLLR: div=0x%x pllcount=0x%x out=0x%x mul=0x%x usbdiv=0x%x" % (self.div, self.pllcount, self.out, self.mul, self.usbdiv)


class CKGR_MOR:
    def __init__(self, val):
        self.val = val
        self.oscount = 0
        self.oscbypass = 0
        self.moscen = 0
        self.get_fields()
    def get_fields(self):
        self.oscount = get_bit_region(self.val, 15, 8)
        self.oscbypass = get_bit_region(self.val, 1, 1)
        self.moscen = get_bit_region(self.val, 0, 0)
    def __str__(self):
        return "CKGR_MOR: oscount=0x%x oscbypass=0x%x moscen=0x%x" % (self.oscount, self.oscbypass, self.moscen)


class PMC_MCKR:
    def __init__(self, val):
        self.val = val
        self.mdiv = 0
        self.pres = 0
        self.css = 0
        self.get_fields()
    def get_fields(self):
        self.mdiv = get_bit_region(self.val, 9, 8)
        self.pres = get_bit_region(self.val, 4, 2)
        self.css = get_bit_region(self.val, 1, 0)
    def __str__(self):
        return "PMC_MCKR: mdiv=0x%x pres=0x%x css=0x%x" % (self.mdiv, self.pres, self.css)


class PMC_SCSR:
    def __init__(self, val):
        self.val = val
        self.pck = 0
        self.progck = 0
        self.get_fields()
    def get_fields(self):
        self.pck = get_bit_region(self.val, 0, 0)
        self.progck = get_bit_region(self.val, 10, 8)
    def __str__(self):
        return "PMC_SCSR: pck=0x%x progck=0x%x" % (self.pck, self.progck)


def get_bit_region(val, left, right):
    i = right
    bit = 1 << right
    mask = 0
    while i <= left:
        mask = mask | bit
        bit = bit << 1
        i = i + 1
    return (val & mask) >> right
    

def mdw(addr):
    child.sendline("mdw 0x%x" % addr)
    child.expect("> ")
    n = child.before.find(':')
    return string.atoi(child.before[n+1:], 16)


def mww(addr, data):
    child.sendline("mww 0x%x 0x%x" % (addr, data))
    child.expect("> ")
    if string.find(child.before, "Failure") >= 0 or \
       string.find(child.before, "abort") >= 0:
        return 0
    else:
        return 1


def to_base(number, base):
	'converts base 10 integer to another base'

	number = int(number)
	base = int(base)	
	if base < 2 or base > 36:
		raise ValueError, "Base must be between 2 and 36"	
	if not number:
		return 0
		
	symbols = string.digits + string.lowercase[:26]	
	answer = []
	while number:
		number, remainder = divmod(number, base)
		answer.append(symbols[remainder])	
        s = ''.join(reversed(answer))
        f = '0' * (32 - len(s))
	return f + s


# Return a region between 31:0 
def binregion(binstring, left, right):
    n = 31 - left
    m = 31 - right
    return binstring[n:m+1]


def main1():
    child = pexpect.spawn ('telnet localhost 4444')
    child.expect ('> ')
    # print child.before
    #rv = mww(0x200000, 0x12345678)
    #print "mww returned", rv
    d = mdw(CKGR_MOR)
    e = to_base(d, 2)
    r = binregion(e, 15, 8)
    print r
    n = string.atoi(r, 2)
    print "Startup delay is %d slow clock cycles." % (n * 8)
    #print "CKGR_PLLR =", hex(mdw(CKGR_PLLR))
    #print "CKGR_MCFR =", hex(mdw(CKGR_MCFR))

    
if __name__ == "__main__":
    c = CKGR_MOR(0x601)
    print c    

    c = CKGR_PLLR(0x00191c05)
    print c    

    if c.div == 0:
        print "Divider 0 is currently not handled"
        sys.exit(0)
    elif c.div >= 1:
        pll_in = MAIN_CLOCK / c.div
        pll_out = (c.mul + 1) * pll_in
        print "Main clock = %d" % MAIN_CLOCK
        print "pll_in = %d" % pll_in
        print "pll_out = %d" % pll_out
              
    c = PMC_MCKR(0x7)
    print c    
    if c.css == 0:
        print "MCKR: selected slow clock" 
    elif c.css == 1:
        print "MCKR: selected main clock (%d)" % MAIN_CLOCK
    elif c.css == 2:
        print "MCKR: reserved"
    elif c.css == 3:
        print "MCKR: selected PLL clock"

    n = pow(2, c.pres)
    print "Divide pll_out by %d" % n
    print "=> Processor clock = %d" % (pll_out / n)
