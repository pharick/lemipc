TARGET		= lemipc

SRC_DIR		= ./src
BUILD_DIR	= ./build

SRCS		= main.c shared_resources.c shared_data.c game.c turn.c
SRCS_		= $(addprefix $(SRC_DIR)/, $(SRCS))
OBJS		= $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS		= $(OBJS:.o=.d)

CFLAGS		= -Wall -Wextra -Werror

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I. -MMD -MP -c $< -o $@

.PHONY: clean fclean re

clean:
	rm -f $(OBJS) $(DEPS)

fclean:
	rm -r $(BUILD_DIR)

re: clean all

-include $(DEPS)
