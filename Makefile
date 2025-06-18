# **************************************************************************** #
#                            Makefile for ircserv                              #
# **************************************************************************** #

# ------------------------------- Configuration ------------------------------ #

NAME        := ircserv
ODIR        := obj
SANITIZED_FLAG := .sanitized

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

CXXFLAGS	+= -Wall -Wextra -Werror -I$(INC)
STDFLAG		:= -std=c++98
SANITIZE	:= # -fsanitize=address
LDFLAGS		:=
CFLAGS		:= # -stdlib=libstdc++ # -fno-rtti
DEBUGFLAGS	:=

# ------------------------------- Variables ---------------------------------- #

RM     := rm -fr
OBS    := $(NAME).dSYM .DS_Store .vscode output.log

SHIFT  = $(eval O=$(shell echo $$((($(O)%15)+1))))

# # ------------------------------ System Detection ---------------------------- #

UNAME := $(shell uname -s)
NUMPROC :=
CXX := $(firstword $(foreach v,$(shell seq 21 -1 10),$(if $(shell command -v clang++-$(v)),clang++-$(v))))

# CPU core detection
ifeq ($(UNAME), Darwin)
	CXX := c++
	NUMPROC := $(shell sysctl -n hw.ncpu)
else ifeq ($(UNAME), Linux) # Detect best available compiler
# CXX := $(shell \
# 	for bin in clang++-20 clang++-19 clang++-18 clang++-17 clang++ g++-13 g++-12 g++ ; do \
# 		if command -v $$bin >/dev/null 2>&1; then echo $$bin; break; fi; \
# 	done \
# )
	NUMPROC := $(shell grep -c ^processor /proc/cpuinfo)
else
	$(error Unsupported OS: $(UNAME))
endif

# Optional: Display detected compiler
COMPILER_VERSION := $(shell $(CXX) --version | head -n 1)
# $(info [INFO] Compiler: $(CXX))
# $(info [INFO] Version : $(COMPILER_VERSION))
# $(info [INFO] CPU Cores: $(NUMPROC))

# ------------------------------ Build Mode Logic ---------------------------- #

MODE ?=
SANITIZED_EXISTS := $(shell [ -f $(SANITIZED_FLAG) ] && echo 1)

PHONY	:= all
ifeq ($(MODE), debug)
    DEBUGFLAGS += -g3 $(SANITIZE)
    ifeq ($(SANITIZED_EXISTS), 1)
        all: buildinfo $(NAME) info
    else
        all: createSANITIZED clean buildinfo $(NAME) info
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
	@echo "$(L_BLUE)  [info]:  $(L_GREEN)Starting parallel build using '$(NUMPROC)'' threads...$(RESET)"
	@$(MAKE) -s -j$(NUMPROC)

# --------------------------- Targets & Rules --------------------------------- #

CXXFLAGS += $(DEBUGFLAGS) $(STDFLAG)
$(NAME): $(OBJ)
	@$(CXX) $(CXXFLAGS) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $(NAME)

# Automatically create necessary object directories before compilation
OBJDIRS := $(sort $(dir $(OBJ)))
PHONY	+= prepare
prepare:
	@mkdir -p $(sort $(dir $(OBJ)))

# $(ODIR)%.o: $(SRCDIR)%.cpp | prepare buildinfo
$(ODIR)/%.o: %.cpp | prepare buildinfo
	@mkdir -p $(dir $@)
	@printf "$(L_BLUE)  [info]:  $(L_GREEN)%-30s -> $(L_BLUE)%s$(RESET)\n" "$<" "$@"
	@$(CXX) -c $(CFLAGS) $(CXXFLAGS) $< -o $@

# Define a pattern rule that compiles every .cpp file into a .o file
PHONY	+= buildinfo
buildinfo:
	@echo "$(L_BLUE)  [info]:  $(L_MAGENTA)$(CXX) $(CFLAGS) $(CXXFLAGS)$(RESET)"

PHONY	+= createSANITIZED
createSANITIZED:
	@touch $(SANITIZED_FLAG)

PHONY	+= removeSANITIZED
removeSANITIZED:
	@$(RM) $(SANITIZED_FLAG)

PHONY	+= clean
clean: ## cleans all the obj files
	@$(RM) $(ODIR)*.o

PHONY	+= fclean
fclean: clean removeSANITIZED ## uses the rule clean and removes the obsolete files
	@$(RM) $(NAME) $(ODIR) $(OBS)

PHONY	+= re
re: fclean all ## does fclean and all

TIDY_FLAGS =	'clang-analyzer-*,\
				bugprone-*,\
				performance-*,\
				misc-const-correctness,\
				misc-misplaced-const,\
				cppcoreguidelines-avoid-const-or-ref-data-members,\
				readability-avoid-const-params-in-decls,\
				readability-const-return-type,\
				readability-make-member-function-const,\
				llvm-include-order,\
				cppcoreguidelines-init-variables,\
				llvmlibc-inline-function-decl,\
				hicpp-braces-around-statements,\
				readability-braces-around-statements,\
				google-runtime-int,\
				readability-implicit-bool-conversion,\
				readability-isolate-declaration,\
				readability-redundant-string-init,\
				cppcoreguidelines-special-member-functions,\
				hicpp-special-member-functions,\
				readability-convert-member-functions-to-static,\
				google-explicit-constructor,\
				hicpp-explicit-conversions,\
				hicpp-signed-bitwise,\
				hicpp-deprecated-headers,\
				modernize-deprecated-headers,\
				misc-use-anonymous-namespace,\
				misc-definitions-in-headers'

TIDY_EXTRA_ARGS =	--extra-arg=-std=c++98 \
					--extra-arg=-Iincludes \
					--extra-arg=-Wno-c++98-compat

CLANG_TIDY = clang-tidy-20

PHONY	+= tidy
tidy: MODE := tidy $(CLANG_TIDY)
tidy: info ## run clang-tidy on project 
	@$(CLANG_TIDY) $(SRC) \
	--checks=$(TIDY_FLAGS) \
	$(TIDY_EXTRA_ARGS) \
	--header-filter='includes/.*' \
	--system-headers=false \
	--quiet \
	-- $(CXXFLAGS) $(INCLUDES)
	@echo "✅ check complete"

# DETECTED_SCAN_BUILD := $(firstword $(foreach v,$(shell seq 21 -1 15),$(if $(shell command -v scan-build-$(v) >/dev/null 2>&1 && echo scan-build-$(v)),scan-build-$(v))))
# Try to find the newest scan-build from version 21 down to 15
# If none are found in that range, default to scan-build-20 (original value)
PHONY += scan

SCAN_BUILD := $(firstword $(foreach v,$(shell seq 21 -1 15),$(if $(shell command -v scan-build-$(v)),scan-build-$(v))))
ifeq ($(SCAN_BUILD),)
SCAN_BUILD := scan-build-20
endif

scan: MODE := scan $(SCAN_BUILD)
scan: fclean ## Scan-build static analysis
	@echo "🔍 Running scan-build..."
	$(info Using scan-build: $(SCAN_BUILD))
	@CC=$(CC) CXX=$(CXX) $(SCAN_BUILD) \
		-enable-checker alpha \
		-enable-checker security -enable-checker unix -enable-checker core \
		-enable-checker cplusplus -enable-checker deadcode -enable-checker nullability \
		-analyzer-config aggressive-binary-operation-simplification=true \
		-v make
	@echo "✅ Scan-build analysis complete"

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

PHONY	+= info
info: ## prints project based info
	@echo "$(L_CYAN)# ------------------------- Build Info -------------------------- #$(RESET)"
	@echo "$(L_GREEN)NAME        $(RESET): $(L_MAGENTA)$(NAME)$(RESET)"
	@echo "$(L_GREEN)UNAME       $(RESET): $(L_MAGENTA)$(UNAME)$(RESET)"
	@echo "$(L_GREEN)NUMPROC     $(RESET): $(L_MAGENTA)$(NUMPROC)$(RESET)"
	@echo "$(L_GREEN)CC          $(RESET): $(L_MAGENTA)$(COMPILER_VERSION)$(RESET)"
	@echo "$(L_GREEN)STANDARD    $(RESET): $(L_MAGENTA)$(STANDARD)$(RESET)"
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
