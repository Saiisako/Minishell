/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:43:47 by skock             #+#    #+#             */
/*   Updated: 2025/04/11 17:50:09 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error_message(const char *msg)
{
	printf("%s\n", msg);
}

void	free_env(t_ms *minishell)
{
	t_env	*tmp;
	t_env	*next_node;

	tmp = minishell->env_lst;
	while (tmp)
	{
		next_node = tmp->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
		tmp = next_node;
	}
}

void	prompt(t_ms *minishell)
{
	char	*input;

	while (1)
	{
		char	*cwd;
		char	*full_prompt;
		char	*last;
		
		cwd = getcwd(NULL, 0);
		last = get_last_dir(cwd);
		full_prompt = ft_strjoin(last, " > ");
		free(cwd);

		input = readline(full_prompt);
		free(full_prompt);
		
		if (!input)
		{
			printf("CTRL + D\n");
			free_env(minishell);
			free(minishell);
			exit(0);
		}
		if (!parsing_input(input, minishell))
			print_error_message("error");
		if (input && *input)
			add_history(input);			
		if (minishell->cmd_list)
		execute_builtin(minishell->cmd_list, minishell);
		free(input);
	}
}

//add-on chloe

void	execute_builtin(t_cmd *cmd, t_ms *minishell)
{
	char *cmd_name;

	if (!cmd || !cmd->token)
		return;
	cmd_name = cmd->token->value;
	if (!ft_strcmp(cmd_name, "echo"))
		print_echo(cmd);
	else if (!ft_strcmp(cmd_name, "pwd"))
		print_pwd();
	else if (!ft_strcmp(cmd_name, "env"))
		print_env(minishell);
	else if (!ft_strcmp(cmd_name, "cd"))
		cd(cmd, minishell);
	
}

char	*get_last_dir(char *path)
{
	char	*last_slash = ft_strrchr(path, '/');
	if (!last_slash)
		return (path);
	return (last_slash + 1);
}

int	main(int ac, char **av, char **envp)
{
	t_ms	*minishell;

	(void)av;
	if (ac == 1)
	{
		minishell = malloc(sizeof(t_ms));
		minishell->envp = envp;
		minishell->is_next_space = false;
		fill_env_cpy(minishell, envp);
		prompt(minishell);
		return (0);
	}
	return (1);
}
