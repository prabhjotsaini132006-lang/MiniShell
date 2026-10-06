CC = gcc

CFLAGS = -Wall -Wextra -Iinclude

TARGET = minishell

SRC = \
	src/main.c \
	src/parser.c \
	src/redirection.c \
	src/pipeline.c \
	src/background.c \
	src/signals.c \
	src/history.c \
	src/foreground.c \
	src/ls.c \
	src/pwd.c \
	src/cd.c \
	src/cat.c \
	src/touch.c \
	src/mkdir.c \
	src/rm.c \
	src/echo.c \
	src/clear.c \
	src/head.c \
	src/tail.c \
	src/wc.c \
	src/cp.c \
	src/mv.c \
	src/exit.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: clean run