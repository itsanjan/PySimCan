PySimCan
========

Python extension for CrossControl CCSimTech SimCan API. 

<strong>Unfortunately I don't have the clearance to post the source code here at the moment. But you can contact me if you are having trouble with it.</strong>

Requirement
========
* Python 2.7.x (32bit)
* CCSimTech

Usage
========
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

