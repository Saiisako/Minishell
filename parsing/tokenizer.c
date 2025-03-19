/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:18:40 by skock             #+#    #+#             */
/*   Updated: 2025/03/19 15:42:41 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// ligne 59 pourrait poser problème si input[i] est nul selon Claude.

int	double_quote(char *input, int *i, t_ms *minishell)
{
	int		start;
	char	*token;

	start = *i;
	(*i)++;
	while (input[*i] && input[*i] != 34)
		(*i)++;
	if (!input[*i])
		return (printf("unclosed quote : "), 0);
	token = ft_substr(input, start, (*i - start) + 1);
	fill_token_list(minishell, token);
	return (1);
}

int	single_quote(char *input, int *i, t_ms *minishell)
{
	int		start;
	char	*token;

	start = *i;
	(*i)++;
	while (input[*i] && input[*i] != 39)
		(*i)++;
	if (!input[*i])
		return (printf("unclosed quote : "), 0);
	token = ft_substr(input, start, (*i - start) + 1);
	fill_token_list(minishell, token);
	return (1);
}

void	word_token(char *input, int *i, t_ms *minishell)
{
	int		start;
	char	*token;

	start = *i;
	(*i)++;
	while (input[*i] && !ft_iswhitespace(input[*i])
		&& (input[*i] != 39 && input[*i] != 34))
		(*i)++;
	token = ft_substr(input, start, (*i - start));
	fill_token_list(minishell, token);
	(*i)--;
}

void	process_token(char *input, int *i, t_ms *minishell)
{
	if (input[*i] == 34)
	{
		if (!double_quote(input, i, minishell))
			return ;
	}
	else if (input[*i] == 39)
	{
		if (!single_quote(input, i, minishell))
			return ;
	}
	else if (ft_isascii(input[*i]))
		word_token(input, i, minishell);
}

int	parsing_input(char *input, t_ms *minishell)
{
	int	i;

	minishell->token = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] && ft_iswhitespace(input[i]))
			i++;
		if (!input[i])
			break ;
		process_token(input, &i, minishell);
		if (!input[i])
			break ;
		i++;
	}
	print_tokens(minishell->token);
	return (1);
}
