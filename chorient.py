#!/usr/bin/env python

import errno, getopt, os, sys
import logging
import serial
import subprocess

default_device = '/dev/ttyORIENT'
default_baud = 9600

monitor_outputs = {0:'xrandr --output DFP7 --rotate %s --auto --mode 1920x1200 --left-of DFP6',
                   1:'xrandr --output DFP6 --rotate %s --auto --mode 1920x1200',
                   2:'xrandr --output DFP5 --rotate %s --auto --mode 1920x1200 --right-of DFP6'}

def usage():
    print 'Usage:'
    print '\t%s  [OPTIONS]' % sys.argv[0]
    print '\nOptions:'
    print '  -b [--baud]   arg\t\tBaud rate to use [DEFAULT: %d]' % default_baud
    print '  -d [--device] arg\t\tDevice to use [DEFAULT: %s]' % default_device
    print '  -D [--debug]     \t\tPrint debug messages to the console'
    print '  -h [--help]      \t\tPrint this help'

def monitor_orientation(monitors, orientation_data):
    logging.debug("Monitors:%d - Orientation:%s" % \
                  (monitors, bin(orientation_data & 0xFFFF)))

    for i in range(monitors):
        orientation = orientation_data & (1 << i)

        if orientation:
            orientation_str = "normal"
        else:
            orientation_str = "left"

        logging.debug(monitor_outputs[i] % orientation_str)
        subprocess.call(monitor_outputs[i] % orientation_str, shell=True)

def main():
    device = default_device
    baud = default_baud
    try:
        opts, args = getopt.getopt(sys.argv[1:], 'b:d:Dh',
                                   ['baud=', 'device=', 'debug', 'help',])
    except:
        print 'Error: unrecognised option'
        usage()
        sys.exit(erno.EINVAL)

    for o, a in opts:
        if o in ('-b', '--baud'):
            try:
                baud = int(a)
            except:
                print 'Error: invalid baud rate'
                sys.exit(errno.EINVAL)
        if o in ('-d', '--device'):
            device = a
        if o in ('-D', '--debug'):
            logging.basicConfig(level = logging.DEBUG)
        if o in ('-h', '--help'):
            usage()
            sys.exit(0)

    if not os.path.exists(device):
        print 'Error: device "%s" does not exist' % device
        sys.exit(errno.ENODEV)

    try:
        ser = serial.Serial(device, baud)
    except:
        print 'Error: unable to connect to device %s' % device
        sys.exit(errno.EIO)

    while True:
        data = ser.readline().split()
        monitors = int(data[0])
        orientation = int(data[1])
        monitor_orientation(monitors, orientation)


if __name__ == '__main__':
    main()
