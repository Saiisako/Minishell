/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:37:09 by skock             #+#    #+#             */
/*   Updated: 2025/05/17 20:13:58 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	join_expand(t_ms *minishell, int index)
{
	int		size;
	char	*word_expand;
	t_token	*tmp;

	expand(minishell);
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

void	expand_env(t_env *tmp_env, t_token **tmp2, int *found)
{
	while (tmp_env)
	{
		if (((*tmp2))->value[0] != '$')
			break ;
		if (!ft_strcmp(tmp_env->key, ((*tmp2))->value + 1))
		{
			free((*tmp2)->value);
			(*tmp2)->value = ft_strdup(tmp_env->value);
			*found = 1;
			break ;
		}
		tmp_env = tmp_env->next;
	}
}

void	expand(t_ms *minishell)
{
	t_env	*tmp_env;
	t_token	*tmp2;
	int		found;

	tmp2 = minishell->expand;
	while (tmp2)
	{
		tmp_env = minishell->env_lst;
		found = 0;
		if (!ft_strcmp(tmp2->value, "$"))
		{
			tmp2 = tmp2->next;
			continue ;
		}
		expand_env(tmp_env, &tmp2, &found);
		if (!found && tmp2->value[0] == '$')
		{
			free(tmp2->value);
			tmp2->value = ft_strdup("");
		}
		tmp2 = tmp2->next;
	}
	return ;
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
			i += 2;
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
	join_expand(minishell, index);
}
