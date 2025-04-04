/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 12:02:46 by skock             #+#    #+#             */
/*   Updated: 2025/04/04 18:38:21 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*new_expand(char *str)
{
	t_token		*expand;

	expand = malloc(sizeof(t_token));
	expand->value = str;
	expand->next = NULL;
	expand->type = EXPANDING;
	expand->is_next_space = true;
	return (expand);
}

void	expand_add_back(t_token **lst, t_token *new)
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

void	fill_expand_lst(t_ms *minishell, char *str)
{
	t_token	*expand;

	expand = new_expand(str);
	if (!expand)
		return ;
	expand_add_back(&minishell->expand, expand);
}

void	word_expand(char *value, t_ms *minishell, int *i)
{
	char	*word;
	int		start;

	start = *i;
	while (value[*i] && value[*i] != '$')
		(*i)++;
	word = ft_substr(value, start, (size_t)((*i) - start));
	fill_expand_lst(minishell, word);
	if (value[*i] == '$')
		(*i)--;
}

void	dollar_expand(char *value, t_ms *minishell, int *i)
{
	char	*word;
	int		start;

	start = *i;
	(*i)++;
	while ((ft_isalnum(value[(*i)]) || value[(*i)] == '_'))
		(*i)++;
	word = ft_substr(value, start, (size_t)((*i) - start));
	fill_expand_lst(minishell, word);
	(*i)--;
}

void	print_expand(t_ms *minishell)
{
	t_token	*tmp;

	tmp = minishell->expand;
	while(tmp)
	{
		printf("node = %s|\n", tmp->value);
		tmp = tmp->next;
	}
}

void	do_expand(char *value, t_ms *minishell)
{
	int	i;

	i = 0;
	while (value[i])
	{
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
}

void	expand(t_ms *minishell)
{
	t_env	*tmp_env;
	t_token	*tmp2;

	tmp2 = minishell->expand;
	while (tmp2)
	{
		tmp_env = minishell->env_lst;
		while (tmp_env)
		{
			if (!ft_strcmp(tmp_env->key, tmp2->value + 1))
			{
				free(tmp2->value);
				tmp2->value = ft_strdup(tmp_env->value);
			}
			else if (tmp2->value[0] == '$' && ft_strcmp(tmp_env->key, tmp2->value + 1) == 1)
			{
				free(tmp2->value);
				tmp2->value = ft_strdup("");
			}
			tmp_env = tmp_env->next;
		}
		tmp2 = tmp2->next;
	}
	print_expand(minishell);
	return ;
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
			do_expand(tmp->value, minishell);
			tmp = tmp->next;
		}
		else if (tmp->type == D_QUOTE)
		{
			do_expand(tmp->value, minishell);
			tmp = tmp->next;
		}
		else
			tmp = tmp->next;
	}
	expand(minishell);
}
