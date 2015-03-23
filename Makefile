SRC_DIR := src
INCPATH := -Iinc

CC = arm-linux-gcc
AR = arm-linux-ar
ARFLAG := -rsvu

FILES = $(foreach dir, $(SRC_DIR),$(wildcard $(dir)/*.c))
OBJ := $(patsubst %.c,%.o,$(FILES))

TARGET := ../libs/libuibase.a

.PHONY : all clean 
all:$(OBJ) $(TARGET) 

$(TARGET):$(OBJ)
		$(AR) $(ARFLAG) $(TARGET) $(OBJ)
$(OBJ):%.o:%.c
		$(CC) $(INCPATH) -c $^ -o $@
clean:
	rm -f $(OBJ) 
	rm -f $(TARGET)	