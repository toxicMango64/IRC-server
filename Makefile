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

CXXFLAGS	+= -Wall -Wextra -Werror -I$(INC) -std=c++98
SANITIZE	:= -fsanitize=address
LDFLAGS		:=
CFLAGS		:=
DEBUGFLAGS	:=

# ------------------------------- Variables ---------------------------------- #

RM     := rm -fr
OBS    := $(NAME).dSYM .DS_Store .vscode output.log

SHIFT  = $(eval O=$(shell echo $$((($(O)%15)+1))))

# # ------------------------------ System Detection ---------------------------- #

UNAME := $(shell uname -s)
NUMPROC :=
CC :=

# CPU core detection
ifeq ($(UNAME), Darwin)
    CC := c++
    NUMPROC := $(shell sysctl -n hw.ncpu)
else ifeq ($(UNAME), Linux) # Detect best available compiler
	CC := $(shell \
		for bin in clang++-19 clang++-18 clang++-17 clang++ g++-13 g++-12 g++ ; do \
			if command -v $$bin >/dev/null 2>&1; then echo $$bin; break; fi; \
		done \
	)
	NUMPROC := $(shell grep -c ^processor /proc/cpuinfo)
else
	$(error Unsupported OS: $(UNAME))
endif

# Optional: Display detected compiler
COMPILER_VERSION := $(shell $(CC) --version | head -n 1)
# $(info [INFO] Compiler: $(CC))
# $(info [INFO] Version : $(COMPILER_VERSION))
# $(info [INFO] CPU Cores: $(NUMPROC))

# ------------------------------ Build Mode Logic ---------------------------- #

MODE ?=
SANITIZED_EXISTS := $(shell [ -f $(SANITIZED_FLAG) ] && echo 1)

PHONY	:= all
ifeq ($(MODE), debug)
    DEBUGFLAGS += -g3 $(SANITIZE)
    ifeq ($(SANITIZED_EXISTS), 1)
        all: build_mode_info $(NAME) info
    else
        all: createSANITIZED clean build_mode_info $(NAME) info
    endif
else
    ifeq ($(SANITIZED_EXISTS), 1)
        all: removeSANITIZED clean $(NAME) banner
    else
        all: $(NAME) banner
    endif
endif

PHONY	+= build
build:
	@$(MAKE) -j$(NUMPROC)

# --------------------------- Targets & Rules --------------------------------- #

CXXFLAGS += $(DEBUGFLAGS)
$(NAME): $(OBJ)
	@$(CC) $(CXXFLAGS) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $(NAME)

PHONY	+= prepare
prepare:
	@mkdir -p $(ODIR)

# Define a pattern rule that compiles every .cpp file into a .o file
PHONY	+= build_mode_info
build_mode_info: ## prints the build information
	@echo "$(L_CYAN)[compiler]: $(L_MAGENTA)$(CC) $(CXXFLAGS) $(CFLAGS)$(RESET)"

$(ODIR)%.o: $(SRCDIR)%.cpp | prepare build_mode_info
	@mkdir -p $(dir $@)
	@printf "$(L_BLUE)[compiling]: $(L_GREEN)%-30s -> $(L_BLUE)%s$(RESET)\n" "$<" "$@"
	@$(CC) -c $(CXXFLAGS) $(CFLAGS) $< -o $@

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
	@echo "$(L_GREEN)NAME        $(RESET): $(L_MAGENTA)$(NAME)$(RESET)"
	@echo "$(L_GREEN)UNAME       $(RESET): $(L_MAGENTA)$(UNAME)$(RESET)"
	@echo "$(L_GREEN)NUMPROC     $(RESET): $(L_MAGENTA)$(NUMPROC)$(RESET)"
	@echo "$(L_GREEN)CC          $(RESET): $(L_MAGENTA)$(COMPILER_VERSION)$(RESET)"
	@echo "$(L_GREEN)CXXFLAGS    $(RESET): $(L_MAGENTA)$(CXXFLAGS)$(RESET)"
	@echo "$(L_GREEN)LDFLAGS     $(RESET): $(L_MAGENTA)$(LDFLAGS)$(RESET)"
	@echo "$(L_GREEN)CFLAGS      $(RESET): $(L_MAGENTA)$(CFLAGS)$(RESET)"
	@echo "$(L_GREEN)DEBUGFLAGS  $(RESET): $(L_MAGENTA)$(DEBUGFLAGS)$(RESET)"
	@echo "$(L_GREEN)BUILD MODE  $(RESET): $(L_MAGENTA)$(MODE)$(RESET)"
	@echo "$(L_GREEN)SRC         $(RESET):"
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
	@echo >&2 "${L_RED}[Error]${RESET}: ${L_BLUE}\tUnknown target '${L_RED}$@${L_BLUE}'.${RESET}"
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
