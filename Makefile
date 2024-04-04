TARGET_PLAYER		:= lemipc
TARGET_DISPLAY		:= lemipc_display

CC					:= gcc
CFLAGS_PLAYER		:=
CFLAGS_DISPLAY		:= $(shell sdl2-config --cflags)
CFLAGS_COMMON		:= -I. -I./libft/include -Wall -Wextra -Werror -MMD -MP
LDFLAGS_PLAYER		:=
LDFLAGS_DISPLAY		:= $(shell sdl2-config --libs) -lSDL2_ttf -lm
LDFLAGS_COMMON		:= -L./libft/build -lft

SRC_PLAYER_DIR		:= src_player
SRC_DISPLAY_DIR		:= src_display
SRC_COMMON_DIR		:= src_common

PLAYER_SRC_FILES	:= main.c shared_resources.c shared_data.c game.c turn.c
DISPLAY_SRC_FILES	:= main.c
COMMON_SRC_FILES	:= shared_resources.c shared_data.c

PLAYER_SOURCES		:= $(addprefix $(SRC_PLAYER_DIR)/, $(PLAYER_SRC_FILES))
DISPLAY_SOURCES		:= $(addprefix $(SRC_DISPLAY_DIR)/, $(DISPLAY_SRC_FILES))
COMMON_SOURCES		:= $(addprefix $(SRC_COMMON_DIR)/, $(COMMON_SRC_FILES))

OBJ_PLAYER			:= $(PLAYER_SOURCES:%.c=%.o)
OBJ_DISPLAY			:= $(DISPLAY_SOURCES:%.c=%.o)
OBJ_COMMON			:= $(COMMON_SOURCES:%.c=%.o)
DEPS				:= $(OBJ_PLAYER:.o=.d) $(OBJ_DISPLAY:.o=.d) $(OBJ_COMMON:.o=.d)

all:	$(TARGET_PLAYER) $(TARGET_DISPLAY)

$(TARGET_PLAYER):	$(OBJ_PLAYER) $(OBJ_COMMON)
	make -C libft
	$(CC) -o $@ $^ $(LDFLAGS_PLAYER) $(LDFLAGS_COMMON)

$(TARGET_DISPLAY):	$(OBJ_DISPLAY) $(OBJ_COMMON)
	make -C libft
	$(CC) -o $@ $^ $(LDFLAGS_DISPLAY) $(LDFLAGS_COMMON)

$(OBJ_PLAYER):	%.o:	%.c
	$(CC) -c -o $@ $< $(CFLAGS_PLAYER) $(CFLAGS_COMMON)

$(OBJ_DISPLAY):	%.o:	%.c
	$(CC) -c -o $@ $< $(CFLAGS_DISPLAY) $(CFLAGS_COMMON)

$(OBJ_COMMON):	%.o:	%.c
	$(CC) -c -o $@ $< $(CFLAGS_COMMON)

-include $(DEPS)

clean:
	rm -f $(OBJ_PLAYER) $(OBJ_DISPLAY) $(OBJ_COMMON) $(DEPS)

fclean:	clean
	rm -rf $(TARGET_PLAYER) $(TARGET_DISPLAY)

re:	fclean all

.PHONY: all clean fclean re
