/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:44:02 by skock             #+#    #+#             */
/*   Updated: 2025/03/18 16:59:48 by skock            ###   ########.fr       */
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
	WORD = 1, // Commande, argument, fichier
	PIPE, // |
	REDIR_IN, // <
	REDIR_OUT, // >
	APPEND, // >>
	HEREDOC // <<
}		t_type;

typedef struct s_token
{
	char			*value;
	t_type			type;
	bool			is_next_space;
	struct s_token	*next;
}					t_token;

typedef struct s_env
{
	char			*key;	// Avant le =.
	char			*value;	// Apres le =.
	int				index;
	struct s_env	*next;
}					t_env;

typedef struct s_ms
{
	const char	*prompt_msg;
	char	**envp;
	t_env	*env_lst;
	t_token	*token;
}			t_ms;

///////////////// PARSING /////////////////

// ENV(LIST && UTILS)

t_env	*lst_new(t_ms *ms);
void	ft_lstadd_back(t_env **lst, t_env *new);
char	*cpy_left(char	*env_var);
char	*cpy_right(char *env_var);
void	fill_env_cpy(t_ms *ms, char **envp);

// PROMPT

char	*return_last_folder(const char *path);
char	*get_last_path(t_ms *minishell);
void	get_input_prompt(t_ms *minishell);

//

///////////////// BUILTIN /////////////////

// CD

void	update_pwd(t_ms *minishell);
void	cd(t_ms *minishell, char *input);
char	*get_last_folder(char *path);
char	*get_user(t_ms *minishell);
char	*get_oldpwd(t_ms *minishell);
void	go_back(t_ms *minishell);
void	go_tilde(t_ms *minishell);
void	go_root(t_ms *minishell);
void	go_old(t_ms *minishell);
void	cd(t_ms *minishell, char *input);

// ENV

void	print_env(t_ms *minishell);

// PWD

void	print_pwd(void);

#endif