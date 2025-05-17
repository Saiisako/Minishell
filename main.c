/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ChloeMontaigut <ChloeMontaigut@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:43:47 by skock             #+#    #+#             */
/*   Updated: 2025/05/16 23:40:15 by ChloeMontai      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error_message(const char *msg, t_ms *minishell)
{
	char	*special;

	if (minishell->unexpected)
	{
		printf("bash: syntax error near unexpected token 'newline'\n");
		minishell->unexpected = false;
		return ;
	}
	if (minishell->first_special != 69 && minishell->second_special != 69)
	{
		if (minishell->second_special == HEREDOC)
			special = ft_strdup("<<");
		if (minishell->second_special == APPEND)
			special = ft_strdup(">>");
		if (minishell->second_special == REDIR_IN)
			special = ft_strdup("<");
		if (minishell->second_special == REDIR_OUT)
			special = ft_strdup(">");
		if (minishell->second_special == PIPE)
			special = ft_strdup("|");
		printf("bash: syntax error near unexpected token '%s'\n", special);
	}
	else
		printf("%s\n", msg);
}

int	g_sig = 0;

void	handle_signal_prompt(int sig)
{
	if (sig == SIGINT)
	{
		g_sig = 1;
		rl_on_new_line();
		// rl_replace_line("", 0);
		write(1, "\n", 1);
		rl_redisplay();
	}
}

void	prompt(t_ms *minishell)
{
	char	*input;
	char	*full_prompt;

	while (1)
	{
		g_sig = 0;
		signal(SIGINT, handle_signal_prompt);
		signal(SIGQUIT, SIG_IGN);
		
		full_prompt = ft_strjoin(minishell->current_prompt, " > ");
		input = readline(full_prompt);
		free(full_prompt);
		if (!input && g_sig == 1) //
		{
			write(1, "\n", 1);
			continue;
		}
		else if (!input)
		{
			write(1, "exit\n", 5);
			free_env(minishell);
			free(minishell);
			exit(0);
		}
		if (!parsing_input(input, minishell))
 		{
 			print_error_message("", minishell);
 			continue;
 		}
		if (input && *input)
			add_history(input);
		if (setup_heredocs(minishell->cmd_list, minishell) != 0)
		{
			print_error_message("heredoc failed", minishell);
			free(input);
			free_env(minishell);
			free(minishell);
			exit(1);
		}
		if (minishell->cmd_list)
		{
			execute_pipeline(minishell, minishell->cmd_list);
			g_sig = 0;
			free_cmd_list(minishell->cmd_list);
			minishell->cmd_list = NULL;
		}
		free(input);
	}
}

int	create_token_chain(t_token *first_token, char **args)
{
	t_token	*current;
	t_token	*new_token;
	int		i;

	current = first_token;
	i = 1;
	while (args[i])
	{
		new_token = malloc(sizeof(t_token));
		if (!new_token)
			return (1);
		new_token->value = args[i];
		new_token->is_next_space = true;
		new_token->type = WORD;
		new_token->index = i;
		new_token->next = NULL;
		current->next = new_token;
		current = new_token;
		i++;
	}
	return (0);
}

int	run_builtin_command(t_ms *minishell, t_cmd *cmd, char **args)
{
	int	result = 0;

	if (!ft_strcmp(args[0], "echo"))
		print_echo(cmd);
	else if (!ft_strcmp(args[0], "cd"))
		result = cd(cmd, minishell);
	else if (!ft_strcmp(args[0], "pwd"))
		print_pwd(minishell);
	else if (!ft_strcmp(args[0], "export"))
		result = ft_export(minishell, cmd);
	else if (!ft_strcmp(args[0], "unset"))
		ft_unset(minishell, cmd);
	else if (!ft_strcmp(args[0], "env"))
		print_env(minishell);
	else if (!ft_strcmp(args[0], "exit"))
		result = ft_exit(cmd, minishell);
	else
		return (minishell->status = 1, 1);
	minishell->status = result;
	return (result);
}

int	execute_builtin(t_ms *minishell, char **args)
{
	t_cmd	cmd;
	t_token	first_token;
	t_token	*current;
	int		result;

	if (!args || !args[0])
		return (1);
	cmd.path = NULL;
	cmd.infile_fd = -2;
	cmd.outfile_fd = -2;
	cmd.heredoc_fd = -1;
	cmd.is_pipe = false;
	cmd.is_redir = false;
	cmd.pid = -1;
	cmd.next = NULL;

	first_token.value = args[0];
	first_token.is_next_space = true;
	first_token.type = WORD;
	first_token.index = 0;
	first_token.next = NULL;
	
	if (create_token_chain(&first_token, args))
		return (1);
		
	cmd.token = &first_token;
	result = run_builtin_command(minishell, &cmd, args);
	
	current = first_token.next;
	while (current)
	{
		t_token *to_free = current;
		current = current->next;
		free(to_free);
	}
	return (result);
}

char	*get_last_dir(char *path)
{
	char	*last_slash;
	if (!path)
		return ("?");
	last_slash = ft_strrchr(path, '/');
	if (!last_slash)
		return (path);
	return (last_slash + 1);
}

void	setup_minishell(t_ms **minishell, char **envp)
{
		*minishell = malloc(sizeof(t_ms));
		if (!minishell)
			exit(1);
		(*minishell)->status = 0;
		(*minishell)->envp = envp;
		(*minishell)->unexpected = false;
		(*minishell)->is_next_space = false;
		(*minishell)->first_special = 69;
		(*minishell)->second_special = 69;
		(*minishell)->go_cmd = true;

	char *cwd = getcwd(NULL, 0);
	if (cwd)
	{
		char *last = get_last_dir(cwd);
		(*minishell)->current_prompt = ft_strdup(last);
		(*minishell)->pwd = ft_strdup(cwd);
		free(cwd);
	}
	else
	{
		(*minishell)->current_prompt = ft_strdup("?");
		(*minishell)->pwd = ft_strdup("?");
	}

}

int	main(int ac, char **av, char **envp)
{
	t_ms	*minishell;

	minishell = NULL;
	(void)av;
	if (ac == 1)
	{
		setup_minishell(&minishell, envp);
		fill_env_cpy(minishell, envp);
		prompt(minishell);
		free_minishell(minishell);
		return (0);
	}
	return (1);
}
