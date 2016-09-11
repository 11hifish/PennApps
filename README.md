# PennApps Project: Who'sThere

Purpose:

This project implements a modern intelligent door bell that automatically reminds hosts to open the door and give instant feedback to guests who rang the bell. When some one rings the bell, it will immediately send a notification to the uer to let him or her choose whether to open the door or not. If the user does not give a clear response in 30 seconds, it will notify the guests as the door will not open. 

Methods:

The project mainly consists of three parts:
Part I: An Arduino circuit with three LEDs that simulates the behavior of the bell and use LEDs as feedback to guesets.
Part II: A server developed in Python that connects both the bell and the host. It forwards the "bell-rang" message to the host when guests come and then sends the user's to response to the bell and displays it on the LED to let the guests know.
Part III: A front end iOS application developed in Swift that receives "bell-rang" message and lets the users leave their feedback. It also records the door bell activities when the host is not at home.

