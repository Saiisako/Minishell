/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:43:47 by skock             #+#    #+#             */
/*   Updated: 2025/03/12 11:56:30 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error_message(const char *msg)
{
	printf("%s\n", msg);
}

void	search_dollar(const char *input)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (input[i])
	{
		if (input[i] == '$')
			j++;
		i++;
	}
	printf("number of dollar : %d", j);
}

int	does_have_double_quotes(const char *input)
{
	int i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '\"')
		{
			i++;
			while (input[i] != '\"')
			{
				i++;
				if (input[i] == '\"')
				{
					printf("double quotes detected !\n");
					return (1);
				}
			}
		}
		i++;
	}
	return (0);
}

int	parsing_input(char *input, t_ms *ms)
{
	(void)ms;
	if (does_have_double_quotes(input))
		search_dollar(input);


	return (1);
}

void	cd(t_ms *minishell)
{
	(void)minishell;
}

char	*return_last_folder(const char *path)
{
	int		i;
	int		j;
	char	*current_path;

	current_path = NULL;
	i = 0;
	while (path[i])
		i++;
	j = i;
	while (path[i] != '/')
		i--;
	i++;
	current_path = malloc(sizeof(char) * (j - i + 1));
	if (!current_path)
		exit(1);
	j = 0;
	while (path[i])
		current_path[j++] = path[i++];
	current_path[j] = '\0';
	return (current_path);
}

char	*get_last_path(t_ms *minishell)
{
	char 	*current_path;
	t_env	*tmp;

	tmp = minishell->env_lst;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, "PWD"))
		{
			current_path = return_last_folder(tmp->value);
			return (current_path);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

void	get_input_prompt(t_ms *minishell)
{
	char	base[] = "\033[0;36mlazy\033[0mshell:";
	char	base_2[] = " ➜ $";
	minishell->prompt_msg = ft_strjoin(base, get_last_path(minishell));
	minishell->prompt_msg = ft_strjoin(minishell->prompt_msg, base_2);
	
}

int	main(int ac, char **av, char **envp)
{
	t_ms	*minishell;
	char	*input;

	(void)av;
	minishell = malloc(sizeof(t_ms));
	minishell->envp = envp;
	if (ac == 1)
	{
		fill_env_cpy(minishell, envp); // recupère le envp, malloc et le met dans une liste chainer.
		printf("\033[H\033[J"); // permet de faire un clear avant de pouvoir écrire.
		while (1)
		{
			get_input_prompt(minishell);
			input = readline(minishell->prompt_msg); // fonction qui permet de recuperer ce que l'on ecrit.
			if (input && *input)
				add_history(input); // permet avec la flèche du haut de récuperer le dernier input.
			if (!ft_strcmp(input, "env"))
				print_env(minishell); // builtin env (pas encore dans l'environnement mais fonctionnel).
			if (!ft_strcmp(input, "bozo"))
				break ;
			if (!ft_strcmp(input, "pwd"))
				print_pwd();
			if (!parsing_input(input, minishell))
				print_error_message("error");
			printf("\n%s\n", input);
			free(input);
		}
		return (0);
	}
	return (1);
}
