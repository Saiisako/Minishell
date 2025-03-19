/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:43:47 by skock             #+#    #+#             */
/*   Updated: 2025/03/19 15:08:30 by skock            ###   ########.fr       */
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

t_token	*new_token(char *str)
{
	t_token		*token;

	token = malloc(sizeof(t_token));
	token->value = str;
	token->next = NULL;
	return (token);
}

void	token_add_back(t_token **lst, t_token *new)
{
	t_token	*last;

	if (!*lst)
		*lst = new;
	else
	{
		last = *lst;
		while (last->next != NULL)
			last = last->next;
		last->next = new;
	}
}

void	fill_token_list(t_ms *minishell, char *str)
{
	t_token	*token;

	token = new_token(str);
	if (!token)
		return ;
	token_add_back(&minishell->token, token);
}

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("node = {%s}\n", tokens->value);
		tokens = tokens->next;
	}
}

int parsing_input(char *input, t_ms *minishell)
{
	int		i;
	int		start;
	char	*token;

	minishell->token = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] && ft_iswhitespace(input[i]))
			i++;
		if (!input[i])
			break;
		if (input[i] == 34) // 34 = double quote
		{
			start = i;
			i++;
			while (input[i] && input[i] != 34)
				i++;
			if (!input[i])
				return (printf("unclosed quote : "), 0);
			token = ft_substr(input, start, (i - start) + 1);
			fill_token_list(minishell, token);
		}
		else if (input[i] == 39) // 39 = single quote
		{
			start = i;
			i++;
			while (input[i] && input[i] != 39)
				i++;
			if (!input[i])
				return (printf("unclosed quote : "), 0);
			token = ft_substr(input, start, (i - start) + 1);
			fill_token_list(minishell, token);
		}
		else if (ft_isascii(input[i]))
		{
			start = i;
			i++;
			while (input[i] && !ft_iswhitespace(input[i]) && (input[i] != 39 && input[i] != 34))
				i++;
			token = ft_substr(input, start, (i - start));
			fill_token_list(minishell, token);
			i--; // This might cause issues if input[i] is null (pas moi qui le dit).
		}
		if (!input[i])
			break;
		i++;
	}
	print_tokens(minishell->token);
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
			if (!ft_strncmp(input, "cd", 2))
				cd(minishell, input);
			if (!ft_strcmp(input, "env"))
				print_env(minishell); // builtin env (pas encore dans l'environnement mais fonctionnel).
			if (!ft_strcmp(input, "bozo"))
				break ;
			if (!ft_strcmp(input, "pwd"))
				print_pwd();
			if (!parsing_input(input, minishell))
				print_error_message("error");
			free(input);
		}
		return (0);
	}
	return (1);
}

// si un pipe est le dernier token, "unexpected end of line".