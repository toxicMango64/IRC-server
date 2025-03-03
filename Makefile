# Name of the output executable
NAME = test

# C++ Compiler
CXX = g++

# Compiler flags
CFLAGS = -Wall -Wextra -Werror -std=c++98

# Source files
SRCS = test.cpp  # Add your source files here

# Default rule
all: $(NAME)

# Rule to build the target
$(NAME): $(SRCS)
	@$(CXX) $(CFLAGS) $(SRCS) -o $(NAME)

# Rule to run the program
testing: $(NAME)
	@echo "Running the program..."
	@./$(NAME)

# Clean rule to remove the executable
clean:
	rm -f $(NAME)

# Fclean rule to remove all generated files
fclean: clean

# Rebuild rule
re: fclean all

.PHONY: all clean fclean re test

# NAME = test
# CXX = g++

# # Compiler flags
# CFLAGS = -Wall -Wextra -Werror -std=c++98

# # Source files
# SRCS = test.cpp

# all: $(NAME)

# # Rule to build the target
# $(NAME): $(SRCS)
# 	$(CXX) $(CFLAGS) $(SRCS) -o $(NAME)

# # Rule to run the program
# test: $(NAME)
# 	./$(NAME)  # Run the compiled program

# clean:
# 	rm -f $(NAME)

# fclean: clean

# re: fclean all

# .PHONY: all clean fclean re test