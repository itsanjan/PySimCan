#PySimCan

Python extension for CrossControl CCSimTech SimCan API. 

## Requirement

* Python 2.7.x (32bit)
* CrossControl SimTech 2.10.x
* To compile you have to have VC++ 2008 express.

## Usage

Put the file SimCan.pyd into the DLLs folder in your Python installation

You can try the following commands

<pre>
>>>from SimCan import *
>>>handle=can_open("CAN1")
>>>result=can_recevie(handle, [])
>>>can_close(handle)
</pre>

You can also set filters for can_receive(), for example:
<pre>
>>>result = can_receive(handle, [1, 0x7d0])
</pre>
and it will only capture the data sent to CAN ID 0x7d0.

## Compiling

To compile, you need the following files from CrossControl SimTech SDK.

In Headers folder:
* SimCan.h
* simcan_public.h
* simcanerrorcodes.h

In Libs folder:
* simcan.c

The easiest way to start is to put the source code in where your Python 2.x source code is, on the same level with Include and PCbuild. Then you don't have to change any path in the project settings.

## Copyright and license

Copyright 2013 Zhe Li under [the LGPL-3.0 license](LICENSE).
