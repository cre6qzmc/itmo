.PHONY: all clean

APP=lab2edsN3147
CFLAGS=-Wall -Wextra -Werror -g

all: $(APP)

$(APP): $(APP).c
	gcc -o $(APP) $(CFLAGS) $(APP).c

clean:
	rm $(APP)

