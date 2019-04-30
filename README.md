# Smart-Lighting-Solution

To build the project on CCS, do the following steps:
1) Go to Project properties
2) Resources -> Linked Resources
3) Under Path Variables, change the value of SW_ROOT symbol
   to your TivaWare folder path. Also, change the value of
   ORIGINAL_PROJECT_ROOT to CCS project of blinky example
   for board ek-tm4c1294xl under TivaWare folder
   
To build the project on BBG, change the IP of BBG in Makefile
and run the following command:
   make PLATFORM=BBG 
   
   
   References: 
   http://embedded-basics.blogspot.com/2014/11/executing-program-on-bootup.html
   http://tinkernow.com/2015/01/beaglebone-black-rs232-uart-setup/
   https://class.ece.uw.edu/474/peckol/code/BeagleBone/ForestExamples/Example_6_UART/uart.c

