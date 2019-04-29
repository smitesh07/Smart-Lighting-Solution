#  File name: Makefile
#  Description: Makefile
#  Author: Smitesh Modak and Ashish Tak
#  Date: 04/27/2019

include sources.mk

CC = gcc
CPPFLAGS += $(addprefix -I, $(INCLUDE_H))
ifeq ($(PLATFORM),HOST)
	CC = gcc
endif

ifeq ($(PLATFORM),BBG)
	CC = arm-linux-gnueabihf-gcc
endif

TARGET = project2

OBJS = $(SRCS:.c=.o)

%.o : %.c 
	$(CC) $(CPPFLAGS) -c $^ -o  $@

.PHONY: all clean

all: $(TARGET)
ifeq ($(PLATFORM),BBG)
	scp -r project2 root@10.0.0.65:/Project2
endif

$(TARGET): $(OBJS)
	$(CC) $(CPPFLAGS) $(OBJS) -o $@ -lpthread -lrt -lm

clean:
	rm -f $(OBJS) $(TARGET)
