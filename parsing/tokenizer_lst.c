/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_lst.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:20:19 by skock             #+#    #+#             */
/*   Updated: 2025/03/19 15:21:17 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
