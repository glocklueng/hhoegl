# stm32lab-debug.py
# H. Hoegl, 2015-02-24 

import pexpect, string, sys, time, os

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

def flash(filename):
    '''
    filename -- A .bin filename, given as absolute path
    '''
    delay = 0.1
    child.sendline("reset halt")
    child.expect("> ")
    print "--- 1"
    print child.before
    time.sleep(delay)
    child.sendline("poll")
    child.expect("> ")
    print "--- 2"
    print child.before
    time.sleep(delay)
    child.sendline("flash erase_sector 0 0 0")
    child.expect("> ")
    print "--- 3"
    print child.before
    time.sleep(delay)
    child.sendline("flash write_bank 0 %s 0" % filename)
    child.expect("> ")
    print "--- 4"
    print child.before
    time.sleep(delay)
    child.sendline("reset init")
    child.expect("> ")
    print "--- 5"
    print child.before
    time.sleep(delay)
    child.sendline("resume")
    child.expect("> ")
    print "--- 6"
    print child.before


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
    
if __name__ == "__main__":
    child = pexpect.spawn ('telnet localhost 4444')
    child.expect ('> ')
    if 0:
        # d = mdw(CKGR_MOR)
        # e = to_base(d, 2)
        # r = binregion(e, 15, 8)
        # n = string.atoi(r, 2)
        # print "Startup delay is %d slow clock cycles." % (n * 8)
        pass
    if len(sys.argv) == 2:
        basename = os.path.basename(sys.argv[1])
        absfn = os.path.join(os.path.abspath('.'), basename)
        print "Programmiere", absfn
        flash(absfn)
    else:
        print "Fehler"


# vim: et sw=4 ts=4
