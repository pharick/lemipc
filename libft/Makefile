TARGET		= libft.a

BUILD_DIR	= ./build
SRC_DIR		= ./src

SRC_FILES	= ft_printf/ft_printf.c ft_printf/parse_specifier.c ft_printf/print_specifier.c \
				ft_atoi.c ft_isdigit.c ft_putchar.c ft_strchr.c \
				ft_strlen.c ft_nbrlen.c ft_pow.c ft_strcmp.c

SRCS		= $(addprefix $(SRC_DIR)/,$(SRC_FILES))
OBJS		= $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS		= $(OBJS:.o=.d)

CFLAGS		= -I./include -Wall -Wextra -Werror

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJS)
	ar rc $(BUILD_DIR)/$(TARGET) $(OBJS)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

.PHONY: test clean re

clean:
	rm -r $(BUILD_DIR)

re: clean all

-include $(DEPS)
