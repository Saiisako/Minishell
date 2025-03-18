/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:43:47 by skock             #+#    #+#             */
/*   Updated: 2025/03/18 18:16:12 by skock            ###   ########.fr       */
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
void	create_token(t_ms *minishell, char *read)
{
	(void)minishell;
	if (read)
		printf("%s\n", read);
}

int	is_special_char(char cur, char next)
{
	if (next == cur && (cur == '>' || cur == '<'))
	{
		if (cur == '>')
			return (APPEND);
		if (cur == '<')
			return (HEREDOC);
	}
		return (2);
	if (cur == '|')
		return (PIPE);
	if (cur == '>')
		return (REDIR_OUT);
	if (cur == '<')
		return (REDIR_IN);
	return (0);
}

// void	fill_token_lst(t_ms *minishell, char *token)
// {
	
// }

int parsing_input(char *input, t_ms *minishell)
{	
	int		i;
	int		start;
	char	*token;
	(void)minishell;
	i = 0;

	while (input[i] && ft_iswhitespace(input[i]))
		i++;
	while (input[i])
	{
		if (input[i] == 34) // 34 = double quote
		{
			start = i;
			i++;
			while (input[i] && input[i] != 34)
				i++;
			token = ft_substr(input, start, (i - start) + 1);
			// printf("node [%s]\n", ft_substr(input, start, (i - start) + 1));
		}
		else if (input[i] == 39)
		{
			start = i;
			i++;
			while (input[i] && input[i] != 39) // 39 = single quote
				i++;
			token = ft_substr(input, start, (i - start) + 1);
			// printf("node [%s]\n", ft_substr(input, start, (i - start) + 1));
		}
		else if (ft_isascii(input[i]))
		{
			start = i;
			i++;
			while (!ft_iswhitespace(input[i]) && (input[i] != 39 && input[i] != 34))
			{
				if (!input[i])
					break ;
				i++;
			}
			token = ft_substr(input, start, (i - start) + 1);
			// printf("node [%s]\n", ft_substr(input, start, (i - start) + 1));
		}
		printf("%s\n", token);
		// fill_token_list(minishell, token);
		// free(token);
		if (!input[i])
			break ;
		i++;
	}
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
			input = readline("minishell >"); // fonction qui permet de recuperer ce que l'on ecrit.
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
			// if (!ft_strncmp(input, "cd", 2))
			// 	cd(minishell, input);
			// printf("\n%s\n", input);
			free(input);
		}
		return (0);
	}
	return (1);
}

// si un pipe est le dernier token, "unexpected end of line".