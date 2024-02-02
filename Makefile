TARGET_PLAYER		:= lemipc
TARGET_DISPLAY		:= lemipc_display

CC					:= clang
CFLAGS_PLAYER		:=
CFLAGS_DISPLAY		:= $(shell sdl2-config --cflags)
CFLAGS_COMMON		:= -I. -Wall -Wextra -Werror -MMD -MP
LDFLAGS_PLAYER		:=
LDFLAGS_DISPLAY		:= $(shell sdl2-config --libs)
LDFLAGS_COMMON		:=

SRC_PLAYER_DIR		:= src_player
SRC_DISPLAY_DIR		:= src_display
SRC_COMMON_DIR		:= src_common

PLAYER_SRC_FILES	:= main.c shared_resources.c shared_data.c game.c turn.c
DISPLAY_SRC_FILES	:= main.c
COMMON_SRC_FILES	:=

PLAYER_SOURCES		:= $(addprefix $(SRC_PLAYER_DIR)/, $(PLAYER_SRC_FILES))
DISPLAY_SOURCES		:= $(addprefix $(SRC_DISPLAY_DIR)/, $(DISPLAY_SRC_FILES))
COMMON_SOURCES		:= $(addprefix $(SRC_COMMON_DIR)/, $(COMMON_SRC_FILES))

OBJ_PLAYER			:= $(PLAYER_SOURCES:%.c=%.o)
OBJ_DISPLAY			:= $(DISPLAY_SOURCES:%.c=%.o)
OBJ_COMMON			:= $(COMMON_SOURCES:%.c=%.o)
DEPS				:= $(OBJ_PLAYER:.o=.d) $(OBJ_DISPLAY:.o=.d) $(OBJ_COMMON:.o=.d)

all:	$(TARGET_PLAYER) $(TARGET_DISPLAY)

$(TARGET_PLAYER):	$(OBJ_PLAYER) $(OBJ_COMMON)
	$(CC) $(LDFLAGS_PLAYER) $(LDFLAGS_COMMON) -o $@ $^

$(TARGET_DISPLAY):	$(OBJ_DISPLAY) $(OBJ_COMMON)
	$(CC) $(LDFLAGS_DISPLAY) $(LDFLAGS_COMMON) -o $@ $^

$(OBJ_PLAYER):	%.o:	%.c
	$(CC) $(CFLAGS_PLAYER) $(CFLAGS_COMMON) -c -o $@ $<

$(OBJ_DISPLAY):	%.o:	%.c
	$(CC) $(CFLAGS_DISPLAY) $(CFLAGS_COMMON) -c -o $@ $<

-include $(DEPS)

clean:
	rm -f $(OBJ_PLAYER) $(OBJ_DISPLAY) $(DEPS)

fclean:	clean
	rm -rf $(TARGET_PLAYER) $(TARGET_DISPLAY)

re:	fclean all

.PHONY: all clean fclean re
