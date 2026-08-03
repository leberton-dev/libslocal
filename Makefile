NAME = libsteamlocal.a
SRCS = src/init.c src/apps.c src/internal.c
OBJDIR = build
OBJS = $(patsubst src/%.c,$(OBJDIR)/%.o,$(SRCS))

CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lvdfc
INCLUDE = -I./include
AR = ar rcs
RM = rm -rf
HEADER = include/slocal/slocal.h include/slocal/slocal_data.h include/slocal/slocal_errors.h \
		 include/slocal/slocal_internal.h include/slocal/slocal_apps.h

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
	@$(CC) -fsanitize=address $(CFLAGS) $(INCLUDE) -I./test $(TEST_SRC) $(SRCS) -o $(TEST_BIN) $(CRITERION_FLAGS) $(LDFLAGS)
	@ASAN_OPTIONS=abort_on_error=1 ./$(TEST_BIN)

simple_test: $(NAME)
	$(CC) $(CFLAGS) $(INCLUDE) test/fixtures/main_for_testing.c $(NAME) -o $(OBJDIR)/t $(LDFLAGS)
	$(OBJDIR)/t

install: $(NAME)
	source scripts/install.sh && _install

uninstall:
	source scripts/install.sh && _pop_install



.PHONY: all clean fclean re test simple_test
