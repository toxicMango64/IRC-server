NAME 	=	ircserv
CXX 	=	c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -I$(INC)
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

OBJS 	=	$(SRC:.cpp=.o)

REMOVE	=	rm -rf

all:		$(NAME)

%.o: %.c
			$(CXX) $(CXXFLAGS) -o $@ -c $<

$(NAME):	$(OBJS)
			$(CXX) $(CXXFLAGS) $^ -o $(NAME)

clean:
			${REMOVE} ${OBJS}

fclean:		clean
			${REMOVE} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re