/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:44:02 by skock             #+#    #+#             */
/*   Updated: 2025/03/28 19:32:56 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termcap.h>
# include <unistd.h>
# include <stdbool.h>
# include "library/libft/libft.h"

typedef enum e_type
{
	WORD = 1,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC,
	S_QUOTE,
	D_QUOTE,
	SPECIAL,
}	t_type;

typedef struct s_exec
{
	char			*value;
	t_type			type;
	struct s_exec	*next;
}					t_exec;

typedef struct s_token
{
	char			*value;
	t_type			type;
	bool			is_next_space;
	struct s_token	*next;
}					t_token;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				index;
	struct s_env	*next;
}					t_env;

typedef struct s_ms
{
	const char	*prompt_msg;
	char		**envp;
	bool		is_next_space;
	t_env		*env_lst;
	t_token		*token;
	t_exec		*exec;
}				t_ms;

///////////////// PARSING /////////////////

// ENV(LIST && UTILS)

t_env	*lst_new(t_ms *ms);
void	ft_lstadd_back(t_env **lst, t_env *new);
char	*cpy_left(char	*env_var);
char	*cpy_right(char *env_var);
void	fill_env_cpy(t_ms *ms, char **envp);

// PROMPT (see later)

char	*return_last_folder(const char *path);
char	*get_last_path(t_ms *minishell);
void	get_input_prompt(t_ms *minishell);

// TOKENIZER (+LIST))

t_token	*new_token(char *str, t_ms *minishell, t_type type);
int		double_quote(char *input, int *i, t_ms *minishell);
int		single_quote(char *input, int *i, t_ms *minishell);
int		parsing_input(char *input, t_ms *minishell);
int		parsing_input(char *input, t_ms *minishell);
void	fill_token_list(t_ms *minishell, char *str, t_type type);
void	process_token(char *input, int *i, t_ms *minishell);
void	word_token(char *input, int *i, t_ms *minishell);
void	token_add_back(t_token **lst, t_token *new);
void	print_tokens(t_token *tokens);
void	merge_token(t_ms *minishell);

// EXPANDER

// CLEAR QUOTE

char	*quote_rmv(const char *str);
void	clear_quote(t_ms *minishell);

///////////////// EXECUTION /////////////////

void	execution(t_ms *minishell);

///////////////// BUILTIN /////////////////

// CD

void	cd(t_ms *minishell, char *input);
void	cd(t_ms *minishell, char *input);
void	update_pwd(t_ms *minishell);
void	go_tilde(t_ms *minishell);
void	go_back(t_ms *minishell);
void	go_root(t_ms *minishell);
void	go_old(t_ms *minishell);
char	*get_oldpwd(t_ms *minishell);
char	*get_last_folder(char *path);
char	*get_user(t_ms *minishell);

// ENV

void	print_env(t_ms *minishell);

// PWD

void	print_pwd(void);

///////////////// PRINTF /////////////////

void	print_error_message(const char *msg);

#endif