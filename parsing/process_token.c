/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 13:16:51 by skock             #+#    #+#             */
/*   Updated: 2025/06/01 16:05:49 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	double_quote(char *input, int *i, t_ms *minishell)
{
	int		start;
	char	*token;

	start = *i;
	(*i)++;
	while (input[*i] && input[*i] != 34)
		(*i)++;
	if (!input[*i])
		return (printf("unclosed quote : \n"), 0);
	token = ft_substr(input, start, (*i - start) + 1);
	if (input[(*i) + 1] != ' ' && input[(*i) + 1] != '\0')
		minishell->is_next_space = true;
	fill_token_list(minishell, token, D_QUOTE);
	free(token);
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
		return (printf("unclosed quote : \n"), 0);
	token = ft_substr(input, start, (*i - start) + 1);
	if (input[(*i) + 1] != ' ' && input[(*i) + 1] != '\0')
		minishell->is_next_space = true;
	fill_token_list(minishell, token, S_QUOTE);
	free(token);
	return (1);
}

void	word_token(char *input, int *i, t_ms *minishell)
{
	int		start;
	char	*token;

	start = *i;
	(*i)++;
	while (input[*i] && !ft_iswhitespace(input[*i])
		&& (input[*i] != 39 && input[*i] != 34) && ((input[*i] != '|')
			&& (input[*i] != '>') && (input[*i] != '<')))
		(*i)++;
	token = ft_substr(input, start, (*i - start));
	if (input[(*i)] != ' ' && input[(*i)] != '\0')
		minishell->is_next_space = true;
	fill_token_list(minishell, token, WORD);
	free(token);
	(*i)--;
}

void	special_token(char *input, int *i, t_ms *minishell)
{
	const char	*specials[5];
	int			len;
	char		*strd;
	int			j;

	j = 0;
	specials[0] = "|";
	specials[1] = ">>";
	specials[2] = "<<";
	specials[3] = ">";
	specials[4] = "<";
	while (j < 5)
	{
		len = ft_strlen(specials[j]);
		if (ft_strncmp(&input[*i], specials[j], len) == 0)
		{
			strd = ft_strdup(specials[j]);
			fill_token_list(minishell, strd, WORD);
			free(strd);
			*i += len;
			break ;
		}
		j++;
	}
	(*i)--;
}

int	process_token(char *input, int *i, t_ms *minishell)
{
	if (input[*i] == 34)
	{
		if (!double_quote(input, i, minishell))
			return (0);
	}
	else if (input[*i] == 39)
	{
		if (!single_quote(input, i, minishell))
			return (0);
	}
	else if (input[*i] == '>' || input[*i] == '<' || input[*i] == '|')
		special_token(input, i, minishell);
	else if (ft_isascii(input[*i]))
		word_token(input, i, minishell);
	return (1);
}
