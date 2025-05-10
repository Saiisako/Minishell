/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 12:02:46 by skock             #+#    #+#             */
/*   Updated: 2025/05/10 16:56:55 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	join_expand(t_ms *minishell, int index)
{
	int		size;
	char	*word_expand;
	t_token	*tmp;

	size = expand_size(minishell);
	tmp = minishell->expand;
	word_expand = tmp->value;
	while (tmp->next && tmp && size >= 0)
	{
		word_expand = ft_strjoin(word_expand, tmp->next->value);
		tmp = tmp->next;
		size--;
	}
	modify_main_token_lst(minishell, word_expand, index);
	free_expand_list(minishell);
}

void expand(t_ms *minishell)
{
	t_env *tmp_env;
	t_token *tmp2;

	tmp2 = minishell->expand;
	while (tmp2)
	{
		tmp_env = minishell->env_lst;
		int	found = 0;
		if (!ft_strcmp(tmp2->value, "$"))
		{
			tmp2 = tmp2->next;
			continue;
		}
		while (tmp_env)
		{
			if (tmp2->value[0] != '$')
				break ;
			if (!ft_strcmp(tmp_env->key, tmp2->value + 1))
			{
				free(tmp2->value);
				tmp2->value = ft_strdup(tmp_env->value);
				found = 1;
				break;
			}
			tmp_env = tmp_env->next;
		}
		if (!found && tmp2->value[0] == '$')
		{
			free(tmp2->value);
			tmp2->value = ft_strdup("");
		}
		tmp2 = tmp2->next;
	}
	return;
}

void	do_expand(char *value, t_ms *minishell, int index)
{
	int	i;

	i = 0;
	while (value[i])
	{
		if (value[i] == '$' && value[i + 1] == '?')
		{
				fill_expand_lst(minishell, ft_itoa(minishell->status));
				i++;
				i++;
		}
		if (value[i] == '$')
		{
			dollar_expand(value, minishell, &i);
			if (!value[i])
				break ;
		}
		else if (value[i] != '$')
		{
			word_expand(value, minishell, &i);
			if (!value[i])
				break ;
		}
		i++;
	}
	expand(minishell);
	join_expand(minishell, index);
}

void	delete_token(t_ms *minishell, char *value)
{
	t_token	*tmp;
	t_token	*prev;

	tmp = minishell->token;
	prev = NULL;

	while (tmp)
	{
		if (!ft_strcmp(tmp->value, value))
		{
			if (prev == NULL)
				minishell->token = tmp->next;
			else
				prev->next = tmp->next;
			free(tmp->value);
			free(tmp);
			return;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}


void	expand_token(t_token *token, t_ms *minishell)
{
	t_token	*tmp;

	minishell->expand = NULL;
	tmp = token;
	while (tmp)
	{
		if (tmp->type == S_QUOTE)
			tmp = tmp->next;
		else if (tmp->type == WORD)
		{
			if (!ft_strcmp("$", tmp->value))
			{
				tmp = tmp->next;
				continue ;
			}
			do_expand(tmp->value, minishell, tmp->index);
			if (!ft_strcmp(tmp->value, ""))
				delete_token(minishell, tmp->value);
			tmp = tmp->next;
		}
		else if (tmp->type == D_QUOTE)
		{
			do_expand(tmp->value, minishell, tmp->index);
			tmp = tmp->next;
		}
		else
			tmp = tmp->next;
	}
}
