# **************************************************************************** #
#                            Makefile for ircserv                              #
# **************************************************************************** #

# ------------------------------- Configuration ------------------------------ #

NAME        := ircserv
ODIR        := obj

SRCDIRS := src cmd
SRCDIR	:= src/
CMDDIR	:= ${SRCDIR}cmd/
INC	:=	./inc
SRC :=	$(SRCDIR)main.cpp \
		$(SRCDIR)Client.cpp \
		$(SRCDIR)CommandHandler.cpp \
		$(SRCDIR)Server.cpp \
		$(SRCDIR)Utils.cpp \
		$(SRCDIR)Channel.cpp \
		$(CMDDIR)invite.cpp \
		$(CMDDIR)join.cpp \
		$(CMDDIR)kick.cpp \
		$(CMDDIR)mode.cpp \
		$(CMDDIR)part.cpp \
		$(CMDDIR)privmsg.cpp \
		$(CMDDIR)quit.cpp \
		$(CMDDIR)topic.cpp

OBJ := $(patsubst %.cpp, $(ODIR)/%.o, $(SRC))

# ------------------------------ Compilation Flags --------------------------- #

STDFLAG		:= -std=c++98
CXXFLAGS	:= $(STDFLAG)
CXXFLAGS	+= -Wall -Wextra -Werror -I$(INC)

# ------------------------------- Variables ---------------------------------- #

RM     := rm -fr

# # ------------------------------ System Detection ---------------------------- #

CXX := c++

PHONY	:= all
all: $(NAME) banner

# --------------------------- Targets & Rules --------------------------------- #

$(NAME): $(OBJ)
	@$(CXX) $(CXXFLAGS) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $(NAME)

# Automatically create necessary object directories before compilation
OBJDIRS := $(sort $(dir $(OBJ)))
PHONY	+= prepare
prepare:
	@mkdir -p $(sort $(dir $(OBJ)))

$(ODIR)/%.o: %.cpp | prepare
	@mkdir -p $(dir $@)
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $< -o $@

PHONY	+= clean
clean: ## cleans all the obj files
	@$(RM) $(OBJ)

PHONY	+= fclean
fclean: clean ## uses the rule clean and removes the obsolete files
	@$(RM) $(NAME) $(ODIR) $(OBJ)

PHONY	+= re
re: fclean all ## does fclean and all

PHONY	+=	banner
SHIFT	=	$(eval O=$(shell echo $$((($(O)%15)+1))))
banner: ## prints the ircserv banner for the makefile
	@echo " $(C)$(O)$(L)+----------------------------------------+$(RESET)"
	@echo " $(C)$(O)$(L)|  _                                     |";
	$(SHIFT)
	@echo " $(C)$(O)$(L)| (_) _ __  ___  ___   ___  _ __ __   __ |";
	@echo " $(C)$(O)$(L)| | || '__|/ __|/ __| / _ \| '__|\ \ / / |";
	$(SHIFT)
	@echo " $(C)$(O)$(L)| | || |  | (__ \__ \|  __/| |    \ V /  |";
	@echo " $(C)$(O)$(L)| |_||_|   \___||___/ \___||_|     \_/   |";
	@echo " $(C)$(O)$(L)+----------------------------------------+$(RESET)"

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
