# **************************************************************************** #
#  Makefile builtin approach
# **************************************************************************** #
UNAME	=	$(shell uname -s)

# **************************************************************************** #
#  Project based configuration
# **************************************************************************** #
NAME	=	ircserv
RM		=	rm -fr
INC		:=	./inc/
CFLAGS	+=	-Wall -Wextra -Werror -I${INC}
SANITIZE	:=	-fsanitize=address

OBS		+=	$(NAME).dSYM \
			.DS_Store \
			.vscode

# **************************************************************************** #
#  SYSTEM SPECIFIC SETTINGS
# **************************************************************************** #
ifeq ($(UNAME), Darwin) # mac
  CC	:= g++
  NUMPROC	:=  $(shell sysctl -n hw.ncpu)
  CPPFLAGS	+=  
else ifeq ($(UNAME), Linux) # linux
  CC	:=	clang-19
  NUMPROC	:=  $(shell grep -c ^processor /proc/cpuinfo)
  CPPFLAGS	+=  
else # or others
	@echo "unsupported OS"
	@exit (1);
endif

# --------------------------------------------------------------------------- #"

# Source files
SRCDIR		:=	./src/

# SRC	:=	$(SRCDIR)main.cpp
SRC	:=	$(SRCDIR)test.cpp

# Object files
ODIR	:=	obj
OBJ		:=	$(SRC:%.cpp=$(ODIR)/%.o)

SANITIZED_FLAG	=	.sanitized

PHONY	+= all clean info createSANITIZED removeSANITIZED
mode	?=

# Check if the SANITIZED_FLAG file exists
SANITIZED_EXISTS := $(shell [ -f $(SANITIZED_FLAG) ] && echo 1)

ifeq ($(mode), debug) ## checks for debug mode
  CFLAGS	+=	-g3 $(SANITIZE)
  ifeq ($(SANITIZED_EXISTS), 1)
  	all: build_info $(NAME) info
  else
  	all: createSANITIZED clean build_info $(NAME) info
  endif
else
  ifeq ($(SANITIZED_EXISTS), 1)
    all: removeSANITIZED clean $(NAME) ircserv
  else
    all: $(NAME) ircserv
  endif
endif

createSANITIZED:
	@touch $(SANITIZED_FLAG)

removeSANITIZED:
	@$(RM) $(SANITIZED_FLAG)

# non-phony targets
$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(OBJ) -o $(NAME)

# Define a pattern rule that compiles every .cpp file into a .o file
PHONY	+= build_info
$(ODIR)/%.o : %.cpp | build_info
	@mkdir -p $(dir $@)
	@printf "${L_BLUE}[prereq]: ${L_GREEN}%-30s ${L_BLUE}[target]: ${L_GREEN}%s${RESET}\n" "$<" "$@"
	@$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

build_info:
	@echo "${L_CYAN}[compiler info]: ${L_MAGENTA}$(CC) -c $(CFLAGS) $(CPPFLAGS)${RESET}"

PHONY	+= clean
clean: ## cleans all the obj files
	@@$(RM) $(OBJ)
#	@$(RM) $(ODIR)/*.o  # Only remove object files

PHONY	+= fclean
fclean: clean removeSANITIZED ## uses the rule clean and removes the obsolete files
	@$(RM) $(NAME) $(ODIR) $(DEBUGODIR) $(OBS)

PHONY	+= re
re: fclean all ## does fclean and all

test: $(NAME) ## Rule to run the program
	@echo "Running the program..."
	@./$(NAME)
	@${MAKE} re
	@${MAKE} fclean

PHONY	+=	ircserver
SHIFT	=	$(eval O=$(shell echo $$((($(O)%15)+1))))
ircserver:
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
	@echo "${L_CYAN}# -------------------------------------------------------------------------------- #$(RESET)"
	@{ [ -n "${NAME}" ] && echo "${L_GREEN}NAME${RESET}		:	${L_MAGENTA}${NAME}${RESET}"; } || true
	@{ [ -n "${UNAME}" ] && echo "${L_GREEN}UNAME${RESET}		:	${L_MAGENTA}${UNAME}${RESET}"; } || true
	@{ [ -n "${NUMPROC}" ] && echo "${L_GREEN}NUMPROC${RESET}		:	${L_MAGENTA}${NUMPROC}${RESET}"; } || true
	@{ [ -n "${CC}" ] && echo "${L_GREEN}CC${RESET}		:	${L_MAGENTA}${CC}${RESET}"; } || true
	@{ [ -n "${CFLAGS}" ] && echo "${L_GREEN}CFLAGS${RESET}		:	${L_MAGENTA}${CFLAGS}${RESET}"; } || true
	@{ [ -n "${LDFLAGS}" ] && echo "${L_GREEN}LDFLAGS${RESET}		:	${L_MAGENTA}${LDFLAGS}${RESET}"; } || true
	@{ [ -n "${CPPFLAGS}" ] && echo "${L_GREEN}CPPFLAGS${RESET}	:	${L_MAGENTA}${CPPFLAGS}${RESET}"; } || true
	@{ [ -n "${MAKEFLAGS}" ] && echo "${L_GREEN}MAKEFLAGS${RESET}	:	${L_MAGENTA}${MAKEFLAGS}${RESET}"; } || true
	@{ [ -n "${mode}" ] && echo "${L_GREEN}BUILD MODE${RESET}	:	${L_MAGENTA}${mode}${RESET}"; } || true
	@{ [ -n "${SRC}" ] && echo "${L_GREEN}SRC${RESET}		:\n	${L_BLUE}${SRC}${RESET}"; } || true
	@echo "${L_CYAN}# -------------------------------------------------------------------------------- #$(RESET)"

PHONY	+= help
help: ## prints a list of the possible commands
	@echo "${L_CYAN}# ---------------------------------------------------------------- #$(RESET)"
	@printf "${L_MAGENTA}%-15s ${RESET}${L_BLUE}make [option] [target] ...${RESET}\n\n" "Usage:"
	@printf "${L_MAGENTA}Option:${RESET}\n"
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; \
	{printf "${L_GREEN}%-15s ${L_BLUE}%s${RESET}\n", $$1, $$2}'
	@printf "\n${L_GREEN}NOTE:%-10s ${L_BLUE}run make to build the project or with args \\ \n \
	%-15s'make mode=debug' to run in debug mode${RESET}\n"
	@printf "\n%-35s ${L_BLUE}This MAKE has Super Cow Powers.${RESET}\n"
	@echo "${L_CYAN}# ---------------------------------------------------------------- #$(RESET)"

.DEFAULT:
	@echo "${L_CYAN}# ---------------------------------------------------------------- #$(RESET)"
	@echo "${L_RED}[Error]${RESET}: ${L_BLUE}\tUnknown target '${L_RED}$@${L_BLUE}'.${RESET}"
	@${MAKE} -s help

# Declare the contents of the PHONY variable as phony.  We keep that
# information in a variable so we can use it in if_changed and friends.
.PHONY: $(PHONY)

# **************************************************************************** #
# Color Defination
# **************************************************************************** #
GB			:=	\033[42m
RESET		:=	\033[0m
RED			:=	\033[0;31m
WHITE		:=	\033[0;97m
BLUE		:=	\033[0;34m
GRAY		:=	\033[0;90m
CYAN		:=	\033[0;36m
BLACK		:=	\033[0;30m
GREEN		:=	\033[0;32m
YELLOW		:=	\033[0;33m
MAGENTA		:=	\033[0;35m
BLUE		:=	\033[0;36m
L_RED		:=	\033[0;91m
L_GRAY		:=	\033[0;37m
L_BLUE		:=	\033[0;94m
L_CYAN		:=	\033[0;96m
L_GREEN		:=	\033[0;92m
L_YELLOW	:=	\033[0;93m
L_MAGENTA	:=	\033[0;95m
C			:=	\033[38;5;
O			:=	72
L			:=	m
