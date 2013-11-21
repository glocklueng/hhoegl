#!/bin/sh

# find the right USBDEV by listing /sys/bus/usb/drivers/usb-storage/
USBDEV="1-2.2:1.0"
echo -n $USBDEV > /sys/bus/usb/drivers/usb-storage/unbind

