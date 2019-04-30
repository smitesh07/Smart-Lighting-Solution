# Smart-Lighting-Solution

To build the project on CCS, do the following steps:
1) Go to Project properties
2) Resources -> Linked Resources
3) Under Path Variables, change the value of SW_ROOT symbol
   to your TivaWare folder path. Also, change the value of
   ORIGINAL_PROJECT_ROOT to CCS project of blinkly exammple
   for board ek-tm4c1294xl under TivaWare folder
   
To build the project on BBG, change the IP of BBG in Makefile
and run the following command:
   make PLATFORM=BBG 

