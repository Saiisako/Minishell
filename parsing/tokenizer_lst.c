/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_lst.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:20:19 by skock             #+#    #+#             */
/*   Updated: 2025/03/19 18:43:46 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*new_token(char *str, t_ms *minishell)
{
	t_token		*token;

	token = malloc(sizeof(t_token));
	token->value = str;
	token->next = NULL;
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

void	fill_token_list(t_ms *minishell, char *str)
{
	t_token	*token;

	token = new_token(str, minishell);
	if (!token)
		return ;
	token_add_back(&minishell->token, token);
}

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
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

	tmp = minishell->token;
	while (tmp && tmp->next)
	{
		if (tmp->is_next_space == true)
		{
			first_value = tmp->value;
			second_value = tmp->next->value;
			if (tmp->next->is_next_space == false)
				minishell->is_next_space = false;
			new_tk = new_token(ft_strjoin(first_value, second_value), minishell);
			new_tk->next = tmp->next->next;
			free(tmp->value);
			free(tmp->next->value);
			tmp->value = new_tk->value;
			tmp->next = new_tk->next;
			tmp = tmp->next;
		}
		else
			tmp = tmp->next;
	}
}

