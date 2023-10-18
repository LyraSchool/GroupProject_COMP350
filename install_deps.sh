#!/bin/bash

SUDO=''
if [ $EUID != 0 ]; then
	    SUDO='sudo'
fi

$SUDO apt-get install -y bcc nasm






