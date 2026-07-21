NAME = libsteamlocal.a
SRCS = src/init.c
OBJDIR = build
OBJS = $(patsubst src/%.c,$(OBJDIR)/%.o,$(SRCS))

CC = cc
CFLAGS = -Wall -Wextra -Werror -lvdfc
INCLUDE = -I./include
AR = ar rcs
RM = rm -rf
HEADER =

TEST_SRC =
TEST_BIN = $(OBJDIR)/test_runner
CRITERION_FLAGS = $(shell pkg-config --cflags --libs criterion)

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)

$(OBJDIR)/%.o: src/%.c $(HEADER) | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	$(RM) $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

test: $(NAME) | $(OBJDIR)
	@$(CC) -fsanitize=address $(CRITERION_FLAGS) $(CFLAGS) $(INCLUDE) -I./test $(TEST_SRC) $(SRCS) -o $(TEST_BIN)
	@ASAN_OPTIONS=abort_on_error=1 ./$(TEST_BIN)

install: $(NAME)
	source scripts/install.sh && _install

uninstall:
	source scripts/install.sh && _pop_install



.PHONY: all clean fclean re test
