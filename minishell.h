/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:44:02 by skock             #+#    #+#             */
/*   Updated: 2025/06/01 16:05:34 by cmontaig         ###   ########.fr       */
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
# include <errno.h>
# include <termios.h>

extern int	g_sig;

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
	EXPANDING,
}	t_type;

typedef struct s_exec
{
	int			pipe_fd[2];
	int			prev_pipe;
	int			status;
	pid_t		last_pid;
	char		**args;
	int			redir_ok;
}					t_exec;

typedef struct s_token
{
	char			*value;
	bool			is_next_space;
	t_type			type;
	int				index;
	struct s_token	*next;
	struct s_token	*prev;
}					t_token;

typedef struct s_cmd
{
	char			*path;
	int				infile_fd;
	int				outfile_fd;
	int				heredoc_fd;
	bool			is_pipe;
	bool			is_redir;
	pid_t			pid;
	struct s_token	*token;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				index;
	struct s_env	*next;
}					t_env;

typedef struct s_ms
{
	int			status;
	int			parse_error;
	char		**envp;
	bool		unexpected;
	const char	*prompt_msg;
	char		*pwd;
	char		*const_pwd;
	bool		is_next_space;
	char		*current_prompt;
	t_env		*env_lst;
	t_token		*token;
	t_token		*expand;
	t_exec		*exec;
	t_cmd		*cmd_list;
	t_type		first_special;
	t_type		second_special;
	bool		go_cmd;
	bool		here_doc_expand;
	bool		env_i;
}				t_ms;

///////////////// PARSING /////////////////

// ENV(LIST && UTILS)

t_env	*lst_new(t_ms *ms);
t_env	*lst_newpwd(t_ms *ms);
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
int		process_token(char *input, int *i, t_ms *minishell);
void	word_token(char *input, int *i, t_ms *minishell);
void	token_add_back(t_token **lst, t_token *new);
void	special_token(char *input, int *i, t_ms *minishell);
void	fill_token_list(t_ms *minishell, char *str, t_type type);
void	select_is_space(t_ms *minishell);
void	select_type(t_ms *minishell);

// MERGER

void	merge_inception(t_ms *minishell);

// EXPANDER

void	expand_token(t_token *token, t_ms *minishell);
void	do_expand(char *value, t_ms *minishell);
void	do_expand_dquote(char *value, t_ms *minishell);
void	modify_main_token_lst(t_ms *minishell, char *word, int index);
int		expand_size(t_ms *minishell);
void	dollar_expand(char *value, t_ms *minishell, int *i);
void	word_expand(char *value, t_ms *minishell, int *i);
void	fill_expand_lst(t_ms *minishell, char *str);
void	expand_add_back(t_token **lst, t_token *new);
t_token	*new_expand(char *str);
void	join_expand(t_ms *minishell, int index);
void	expand(t_ms *minishell);
void	expand_env(t_env *tmp_env, t_token **tmp2, int *found);

// HEREDOC EXPANDER

void	is_heredoc_token(t_ms *ms);
void	do_expand_heredoc(char **value, t_ms *minishell);
void	dollar_expand_heredoc(char *value, t_ms *minishell, int *i);
void	word_expand_heredoc(char *value, t_ms *minishell, int *i);
void	expand_heredoc(t_ms *minishell);
void	join_expand_heredoc(t_ms *minishell, char **heredoc, int index);

// CLEAR

char	*quote_rmv(char *str);
void	clear_quote(t_ms *minishell);

// CMD_LST

void	fill_cmd_lst(t_ms *minishell);
void	cut_weird(t_cmd *cmd);
t_cmd	*create_cmd_until_pipe(t_token *tmp);
t_cmd	*new_cmd(void);

// ERRORS

int		verif_several_pipes(t_ms *minishell);
int		verif_consecutives_tokens(t_ms *minishell);
int		verif_first_token(t_ms *minishell);
bool	what_type(t_type type);
int		verif_last_token(t_ms *minishell);

///////////////// EXECUTION /////////////////

int		execute_pipeline(t_ms *ms, t_cmd *cmd);
int		create_token_chain(t_token *first_token, char **args);
int		setup_pipes(t_cmd *cmd, int *pipe_fd, int *prev_pipe);
void	update_fds(t_cmd *cmd, int *pipe_fd, int *prev_pipe);
int		wait_all_children(t_ms *ms, int last_pid, int last_status);
void	basic_free(t_ms *ms, char **args);
void	cleanup_pipes(t_cmd *cmd, int pipe_fd[2], int *prev_pipe);
int		is_directory(const char *path);
int		execute_cmd(t_ms *minishell, t_cmd *cmd, char **args, t_exec *exec);
void	exec_redir(t_cmd *cmd, t_exec *exec, char **args, t_ms *ms);
int		handle_command(t_ms *ms, t_cmd *cmd, t_exec *exec);
void	init_exec_struct(t_exec *exec);
void	exec_redir(t_cmd *cmd, t_exec *exec, char **args, t_ms *ms);
int		run_cmd(t_ms *ms, t_cmd *cmd, t_exec *exec);
void	setup_minishell(t_ms **minishell, char **envp);
int		init_cmd_and_token(t_cmd *cmd, t_token *first_token, char **args);
int		handle_input_prompt(t_ms *minishell, char *input);

// ERROR HANDLING

int		redir_error(t_ms *ms, t_cmd *cmd, int pipe_fd[2], int *prev_pipe);
void	errors_prompt(int param, char **args, t_cmd *cmd);
int		handle_empty_cmd(t_cmd *cmd, int *prev_pipe, int pipe_fd[2], t_ms *ms);
void	handle_error_exec(t_ms *minishell, char **args, int errno_code);
void	print_cmd_not_found(char *cmd);
void	check_exec_errors(t_cmd *cmd, char **args, t_ms *ms);
void	handle_null_input(t_ms *ms);
void	handle_heredoc_error(t_ms *ms, char *input);

// SIGNALS

void	handle_signal_prompt(int sig);
void	handle_signal_exec(int sig);
int		child_signal(int status, int last_status);

// REDIRECTION

int		process_redirections(t_cmd *cmd, t_ms *ms);
void	handle_redirections(t_cmd *cmd, int prev_pipe, int *pipe_fd);

// PATH

char	*find_command_path(char *cmd, t_env *env);
char	*check_paths(char **paths, char *cmd);
char	**tokens_to_args(t_token *token);
int		count_tokens(t_token *t);
char	*get_env_value(t_env *env, char *key);

// HEREDOC //

int		setup_heredocs(t_cmd *cmd_list);
void	signal_heredoc(int sig);
int		here_doc_eof(void);
int		do_heredoc(t_cmd *cmd_list, char *limiter);

///////////////// BUILTIN /////////////////

bool	is_builtin(char *cmd);
int		execute_builtin(t_ms *minishell, char **args);
int		run_builtin_command(t_ms *minishell, t_cmd *temp_cmd, char **args);

// CD

int		cd(t_cmd *cmd, t_ms *ms);
void	update_pwd(t_ms *minishell);
void	go_back(t_ms *minishell);
char	*get_last_folder(char *path);
char	*get_user(t_ms *minishell);
char	*get_last_dir(char *path);
void	special_cd(t_token *arg, t_ms *ms);
void	update_oldpwd(t_env *env_lst, char *old_pwd_value);
void	print_error_cd(t_token *arg);
char	*search_home(t_ms *minishell);

// ENV

void	print_env(t_ms *minishell);

// PWD

void	print_pwd(t_ms *ms);

// ECHO

void	print_echo(t_cmd *cmd);
int		is_echo_n_flag(const char *str);

// EXIT

int		ft_exit(t_cmd *cmd, t_ms *minishell, char **args);
int		db_sign(char *str);
int		ft_exit_error(t_token *token, t_ms *ms, long long *numeric_value,
			char **args);
void	exit_clean(t_ms *ms, char **args);

// EXPORT

int		env_add_new(t_env **env_lst, char *key, char *value, t_ms *ms);
int		env_update_or_add(t_env **env_lst, char *key, char *value, t_ms *ms);
void	update_envp(t_ms *ms);
int		parse_export_var(char *arg, char **key, char **value);
void	export_set_var(t_ms *ms, char *arg);
void	fill_env_array(t_env *env_lst, t_env **array);
void	sort_env_array(t_env **array);
void	print_env_array(t_env **array);
void	export_print_sorted(t_env *env_lst);
char	*concat_env_var(char *key, char *value);
int		process_export_arg(t_ms *ms, t_token *arg);
int		ft_export(t_ms *ms, t_cmd *cmd);
char	**dup_envp(char **envp);
int		count_env_vars(t_ms *ms);

// UNSET

int		valid_name(const char *str);
void	ft_unset(t_ms *ms, t_cmd *cmd);
void	unset_var(t_ms *ms, char *key);

///////////////// PRINT /////////////////

void	print_cmd(t_cmd *cmd);
void	print_tokens(t_token *tokens);
void	print_expand(t_ms *minishell);

///////////////// FREE /////////////////

void	free_expand_list(t_ms *minishell);
void	free_env(t_ms *minishell);
void	free_token_list(t_token *token);
void	free_cmd_list(t_cmd *cmd);
void	free_minishell(t_ms *minishell);
void	free_array(char **array);

#endif