# **************************************************************************** #
#                            Makefile for ircserv                              #
# **************************************************************************** #

# ------------------------------- Configuration ------------------------------ #

NAME        := ircserv
SRCDIR      := src/
ODIR        := obj/
INC         := ./inc
SANITIZED_FLAG := .sanitized

SRC := $(SRCDIR)main.cpp \
       $(SRCDIR)Client.cpp \
       $(SRCDIR)CommandHandler.cpp \
       $(SRCDIR)Server.cpp \
       $(SRCDIR)Utils.cpp \
       $(SRCDIR)Channel.cpp

OBJ := $(patsubst %.cpp, $(ODIR)%.o, $(notdir $(SRC)))

# ------------------------------ Compilation Flags --------------------------- #

CFLAGS      += -Wall -Wextra -Werror -I$(INC) -std=c++98
SANITIZE    := -fsanitize=address
LDFLAGS     :=
CPPFLAGS    :=

# # ------------------------------ System Detection ---------------------------- #

UNAME := $(shell uname -s)

ifeq ($(UNAME), Darwin)
    CC := g++
    NUMPROC := $(shell sysctl -n hw.ncpu)
else ifeq ($(UNAME), Linux)
    CC := clang++-19
    NUMPROC := $(shell grep -c ^processor /proc/cpuinfo)
else
    $(error Unsupported OS)
endif

# ------------------------------ Build Mode Logic ---------------------------- #

mode ?=
SANITIZED_EXISTS := $(shell [ -f $(SANITIZED_FLAG) ] && echo 1)

ifeq ($(mode), debug)
    CFLAGS += -g3 $(SANITIZE)
    ifeq ($(SANITIZED_EXISTS), 1)
        all: build_info $(NAME) info
    else
        all: createSANITIZED clean build_info $(NAME) info
    endif
else
    ifeq ($(SANITIZED_EXISTS), 1)
        all: removeSANITIZED clean $(NAME) banner
    else
        all: $(NAME) banner
    endif
endif

# ------------------------------ Targets ------------------------------------- #

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(OBJ) -o $(NAME)

$(ODIR)%.o: $(SRCDIR)%.cpp | build_info
	@mkdir -p $(dir $@)
	@printf "$(L_BLUE)[compiling]: $(L_GREEN)%-30s -> $(L_BLUE)%s$(RESET)\n" "$<" "$@"
	@$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

# Define a pattern rule that compiles every .cpp file into a .o file
PHONY	+= build_info
build_info: ## prints the build information
	@echo "$(L_CYAN)[compiler]: $(L_MAGENTA)$(CC) $(CFLAGS) $(CPPFLAGS)$(RESET)"

createSANITIZED:
	@touch $(SANITIZED_FLAG)

removeSANITIZED:
	@$(RM) $(SANITIZED_FLAG)

PHONY	+= clean
clean: ## cleans all the obj files
	@$(RM) $(ODIR)/*.o

PHONY	+= fclean
fclean: clean removeSANITIZED ## uses the rule clean and removes the obsolete files
	@$(RM) $(NAME) $(ODIR) $(OBS)

PHONY	+= re
re: fclean all ## does fclean and all

PHONY	+=	banner
SHIFT	=	$(eval O=$(shell echo $$((($(O)%15)+1))))
banner: ## prints the ircserv banner for the makefile
	@echo "$(C)$(O)$(L)+----------------------------------------+$(RESET)"
	@echo "$(C)$(O)$(L)|  _                                     |";
	$(SHIFT)
	@echo "$(C)$(O)$(L)| (_) _ __  ___  ___   ___  _ __ __   __ |";
	@echo "$(C)$(O)$(L)| | || '__|/ __|/ __| / _ \| '__|\ \ / / |";
	$(SHIFT)
	@echo "$(C)$(O)$(L)| | || |  | (__ \__ \|  __/| |    \ V /  |";
	@echo "$(C)$(O)$(L)| |_||_|   \___||___/ \___||_|     \_/   |";
	@echo "$(C)$(O)$(L)+----------------------------------------+$(RESET)"

PHONY	+= info
info: ## prints project based info
	@echo "$(L_CYAN)# ------------------------- Build Info -------------------------- #$(RESET)"
	@echo "$(L_GREEN)NAME       $(RESET): $(L_MAGENTA)$(NAME)$(RESET)"
	@echo "$(L_GREEN)UNAME      $(RESET): $(L_MAGENTA)$(UNAME)$(RESET)"
	@echo "$(L_GREEN)NUMPROC    $(RESET): $(L_MAGENTA)$(NUMPROC)$(RESET)"
	@echo "$(L_GREEN)CC         $(RESET): $(L_MAGENTA)$(CC)$(RESET)"
	@echo "$(L_GREEN)CFLAGS     $(RESET): $(L_MAGENTA)$(CFLAGS)$(RESET)"
	@echo "$(L_GREEN)LDFLAGS    $(RESET): $(L_MAGENTA)$(LDFLAGS)$(RESET)"
	@echo "$(L_GREEN)CPPFLAGS   $(RESET): $(L_MAGENTA)$(CPPFLAGS)$(RESET)"
	@echo "$(L_GREEN)BUILD MODE $(RESET): $(L_MAGENTA)$(mode)$(RESET)"
	@echo "$(L_GREEN)SRC        $(RESET):"
	@echo "$(L_BLUE)$(SRC)$(RESET)"
	@echo "$(L_CYAN)# --------------------------------------------------------------- #$(RESET)"

PHONY	+= help
help: ## prints a list of the possible commands
	@echo "$(L_CYAN)# ------------------------- Help Menu -------------------------- #$(RESET)"
	@printf "$(L_MAGENTA)%-15s$(RESET) $(L_BLUE)make [option] [target] ...$(RESET)\n\n" "Usage:"
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "$(L_GREEN)%-15s$(L_BLUE) %s$(RESET)\n", $$1, $$2}'
	@printf "\n$(L_GREEN)NOTE:$(L_BLUE) Use 'make mode=debug' for debug mode build.$(RESET)\n"
	@printf "\n%-35s ${L_BLUE}This MAKE has Super Cow Powers.${RESET}\n"
	@echo "$(L_CYAN)# --------------------------------------------------------------- #$(RESET)"

.DEFAULT:
	@echo "${L_RED}[Error]${RESET}: ${L_BLUE}\tUnknown target '${L_RED}$@${L_BLUE}'.${RESET}"
	@${MAKE} -s help

# ----------------------------- Phony Targets -------------------------------- #

# Declare the contents of the PHONY variable as phony.  We keep that
# information in a variable so we can use it if changed.
.PHONY: $(PHONY)

# ---------------------------- Color Definitions ----------------------------- #

GB         := \033[42m
RESET      := \033[0m
RED        := \033[0;31m
WHITE      := \033[0;97m
BLUE       := \033[0;36m
GRAY       := \033[0;90m
CYAN       := \033[0;36m
BLACK      := \033[0;30m
GREEN      := \033[0;32m
YELLOW     := \033[0;33m
MAGENTA    := \033[0;35m
L_RED      := \033[0;91m
L_GRAY     := \033[0;37m
L_BLUE     := \033[0;94m
L_CYAN     := \033[0;96m
L_GREEN    := \033[0;92m
L_YELLOW   := \033[0;93m
L_MAGENTA  := \033[0;95m
C          := \033[38;5;
O          := 72
L          := m

# /irc_server_project
# ├── Makefile
# ├── include/
# │   ├── Server.hpp
# │   ├── Client.hpp
# │   ├── Channel.hpp
# │   ├── CommandHandler.hpp
# │   └── Utils.hpp
# ├── src/
# │   ├── main.cpp
# │   ├── Server.cpp
# │   ├── Client.cpp
# │   ├── Channel.cpp
# │   ├── CommandHandler.cpp
# │   └── Utils.cpp
# └── README.md
