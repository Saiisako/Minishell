/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:43:47 by skock             #+#    #+#             */
/*   Updated: 2025/03/13 10:36:21 by skock            ###   ########.fr       */
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


int	is_builtin(char *input)
{
	if (!ft_strcmp(input, "cd"))
	return (1);
	if (!ft_strcmp(input, "echo"))
	return (1);
	if (!ft_strcmp(input, "export"))
	return (1);
	if (!ft_strcmp(input, "pwd"))
	return (1);
	if (!ft_strcmp(input, "env"))
	return (1);
	if (!ft_strcmp(input, "unset"))
	return (1);
	if (!ft_strcmp(input, "exit"))
	return (1);
	return (0);
}

int	parsing_input(char *input, t_ms *minishell)
{
	(void)minishell;
	if (is_builtin(input))
	{
		printf("is builtin\n");
		return (0);
	}
	if (does_have_double_quotes(input))
		search_dollar(input);
	return (1);
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
			if (!parsing_input(input, minishell))
				print_error_message("error");
			if (!ft_strcmp(input, "env"))
				print_env(minishell); // builtin env (pas encore dans l'environnement mais fonctionnel).
			if (!ft_strcmp(input, "bozo"))
				break ;
			if (!ft_strcmp(input, "pwd"))
				print_pwd();
			if (!ft_strncmp(input, "cd", 2))
				cd(minishell, input);
			printf("\n%s\n", input);
			free(input);
		}
		return (0);
	}
	return (1);
}
