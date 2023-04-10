SRCS		= 	minishell.c \
				minishell_utils.c \

SRCS_SIGNAL =	./signal/signal.c \
				./signal/signal_sig_int.c

SRCS_BUILT	=	./built_in/env_utils.c \
				./built_in/built_in.c \
				./built_in/built_in_utils.c \
				./built_in/built_in_utility.c \
				./built_in/echo.c \
				./built_in/env.c \
				./built_in/export.c \
				./built_in/export_utils.c \
				./built_in/unset.c \
				./built_in/alias.c \
				./built_in/alias_utils.c \
				./built_in/alias_utility.c \
				./built_in/unalias.c \
				./built_in/pwd.c \
				./built_in/exit.c \
				./built_in/exit_utils.c \
				./built_in/cd.c \

SRCS_INIT	=	./init/init_cmds.c \
				./init/init_files.c \
				./init/init.c \
				./init/init_utils.c \
				./init/init_aliases_utils.c \
				./init/init_env.c \
				./init/init_env_utils.c \
				./init/init_parentheses.c \
				./init/init_path_cmd.c \
				./init/string_utils.c \
				./init/init_files_utils.c \
				./init/init_aliases.c \
				./init/init_struct.c

SRCS_ERROR	=	./error/error_handling.c \
				./error/bad_input_token.c \
				./error/bad_input_checker.c \
				./error/bad_input_token_utils.c \
				./error/bad_syntax_alias_files.c \
				./error/check_parentheses.c \

SRCS_CLEAN	=	./clean/free.c \
				./clean/clean_struct.c

SRCS_EXEC	=	./exec/fd.c \
				./exec/command_redirection.c \
				./exec/execute.c \
				./exec/subshell.c \
				./exec/command_utils.c \
				./exec/child.c \
				./exec/here_doc.c \
				./exec/here_doc_utils.c \
				./exec/pipes.c \
				./exec/parentheses.c \
				./exec/s_pipes.c

SRCS_PARSER	=	./parser/cleaner.c \
				./parser/clean_cmd.c \
				./parser/alias_cmd.c \
				./parser/clean_files.c \
				./parser/clean_utils.c \
				./parser/quote_utils.c \
				./parser/quote_utils_2.c \
				./parser/quote_utils_3.c \
				./parser/dollars_utils.c \
				./parser/cleaner_utility.c \
				./parser/handle_back_slash.c \
				./parser/handle_expand_files.c \
				./parser/handle_expand_files_utils.c \
				./parser/handle_expand_files_utility.c \
				./parser/handle_expand_files_checker.c \

INCLUDES	= 	-I ./includes

OBJS		= 	$(SRCS:.c=.o) \
				$(SRCS_INIT:.c=.o) \
				$(SRCS_ERROR:.c=.o) \
				$(SRCS_CLEAN:.c=.o) \
				$(SRCS_SIGNAL:.c=.o) \
				$(SRCS_BUILT:.c=.o) \
				$(SRCS_EXEC:.c=.o) \
				$(SRCS_PARSER:.c=.o)

DEPS		= 	$(SRCS:.c=.d) \
				$(SRCS_INIT:.c=.d) \
				$(SRCS_ERROR:.c=.d) \
				$(SRCS_CLEAN:.c=.d) \
				$(SRCS_SIGNAL:.c=.d) \
				$(SRCS_BUILT:.c=.d) \
				$(SRCS_EXEC:.c=.d) \
				$(SRCS_PARSER:.c=.d)

CC			=	cc

CFLAGS		=   -Wall -Wextra -Werror -MMD

MINISHELL	=	minishell

all:			$(MINISHELL)

.c.o :	
				$(CC) -g3 $(CFLAGS) -c $< -o $@

$(MINISHELL):	${OBJS}
				mkdir -p lib
				make -C ./libft/get_next_line
				cp ./libft/get_next_line/libgnl.a ./lib
				make -C ./libft/ft_printf
				cp ./libft/ft_printf/libprintf.a ./lib
				make -C ./libft
				cp ./libft/libft.a ./lib
				$(CC) $(CFLAGS) ${OBJS} $(INCLUDES) -g3 -L./lib -lft -lprintf -lgnl -lreadline -o $(MINISHELL)

leaks		:	run
				
run			:	$(MINISHELL)
				make && valgrind --suppressions=ignore.txt -s  --leak-check=full --show-leak-kinds=all --track-fds=yes ./minishell $(arg)

clean:
				rm -rf $(OBJS)
				rm -rf $(DEPS)
			
fclean:			clean
				make -C ./libft/ft_printf fclean
				make -C ./libft/get_next_line fclean
				make -C ./libft/ fclean
				rm -rf $(MINISHELL) ./lib/libprintf.a ./lib/libft.a ./lib/libgnl.a

re:				fclean all

-include : 		$(DEPS)

.PHONY:			all clean fclean re
