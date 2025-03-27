/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_lst.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:20:19 by skock             #+#    #+#             */
/*   Updated: 2025/03/27 14:57:59 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*new_token(char *str, t_ms *minishell, t_type type)
{
	t_token		*token;

	token = malloc(sizeof(t_token));
	token->value = str;
	token->next = NULL;
	token->type = type;
	token->is_next_space = false;
	if (minishell->is_next_space == true)
	{
		token->is_next_space = true;
		minishell->is_next_space = false;
	}
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

void	fill_token_list(t_ms *minishell, char *str, t_type type)
{
	t_token	*token;

	token = new_token(str, minishell, type);
	if (!token)
		return ;
	token_add_back(&minishell->token, token);
}

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("TYPE = %d\n", tokens->type);
		printf("doit fusionner ? -> %d\n", tokens->is_next_space);
		printf("node = {%s}\n", tokens->value);
		tokens = tokens->next;
	}
}

void	merge_token(t_ms *minishell)
{
	t_token	*tmp;
	t_token	*new_tk;
	char	*first_value;
	char	*second_value;
	char	*merged_value;

	tmp = minishell->token;
	while (tmp && tmp->next)
	{
		if (tmp->is_next_space == true)
		{
			first_value = tmp->value;
			second_value = tmp->next->value;
			if (first_value[0] == '\0')
				merged_value = ft_strdup(second_value);
			else if (second_value[0] == '\0')
				merged_value = ft_strdup(first_value);
			else
				merged_value = ft_strjoin(first_value, second_value);
			new_tk = new_token(merged_value, minishell, 0);
			new_tk->is_next_space = tmp->next->is_next_space;
			new_tk->next = tmp->next->next;
			free(tmp->value);
			free(tmp->next->value);
			tmp->value = new_tk->value;
			tmp->is_next_space = new_tk->is_next_space;
			tmp->next = new_tk->next;
			free(new_tk);
			return ;
		}
		tmp = tmp->next;
	}
}

