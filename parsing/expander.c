/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 12:02:46 by skock             #+#    #+#             */
/*   Updated: 2025/04/02 15:17:48 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	do_expand(t_token *token, t_ms *minishell)
{
	int		i;
	int		start;
	char	*exp;
	char	*before;
	char	*result;
	char	*temp;
	t_env	*tmp;
	bool	found;

	i = 0;
	start = 0;
	result = NULL;
	if (token->type == WORD || token->type == D_QUOTE)
	{
		while (token->value[i])
		{
			if (token->value[i] == '$' && token->value[i + 1] &&
				(ft_isalnum(token->value[i + 1]) || token->value[i + 1] == '_'))
			{
				before = ft_substr(token->value, start, i - start);
				temp = ft_strjoin(result, before);
				free(result);
				free(before);
				result = temp;
				start = i + 1;
				i++;
				while (token->value[i] && (ft_isalnum(token->value[i])
						|| token->value[i] == '_'))
					i++;
				exp = ft_substr(token->value, start, i - start);
				tmp = minishell->env_lst;
				found = false;
				while (tmp)
				{
					if (!ft_strcmp(tmp->key, exp))
					{
						found = true;
						break;
					}
					tmp = tmp->next;
				}
				if (found)
				{
					temp = ft_strjoin(result, tmp->value);
					free(result);
					free(tmp->value)
					result = temp;
				}
				free(exp);
				start = i;
				continue;
			}
			i++;
		}
		if (token->value[start])
		{
			before = ft_substr(token->value, start, i - start);
			temp = ft_strjoin(result, before);
			free(result);
			free(before);
			result = temp;
		}
		free(token->value);
		token->value = result;
	}
}

void	expand_token(t_token *token, t_ms *minishell)
{
	t_token	*tmp;

	tmp = token;
	while (tmp)
	{
		if (tmp->type == S_QUOTE)
			tmp = tmp->next;
		else if (tmp->type == WORD)
		{
			do_expand(tmp, minishell);
			tmp = tmp->next;
		}
		else if (tmp->type == D_QUOTE)
		{
			do_expand(tmp, minishell);
			tmp = tmp->next;
		}
		else
			tmp = tmp->next;
	}
}
