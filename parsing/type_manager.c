/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_manager.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 11:23:31 by skock             #+#    #+#             */
/*   Updated: 2025/05/24 18:18:47 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	verif_first_token(t_ms *minishell)
{
	t_token	*tmp;

	tmp = minishell->token;
	if (tmp && tmp->type != WORD && tmp->type != D_QUOTE
		&& tmp->type != S_QUOTE && !tmp->next)
	{
		minishell->unexpected = true;
		return (1);
	}
	return (0);
}

void	select_is_space(t_ms *minishell)
{
	t_token	*tmp;

	tmp = minishell->token;
	while (tmp)
	{
		if (tmp->is_next_space == true)
		{
			if (tmp->type == REDIR_OUT || tmp->type == REDIR_IN
				|| tmp->type == PIPE || tmp->type == HEREDOC
				|| tmp->type == APPEND)
				tmp->is_next_space = false;
			else if (tmp->next && (tmp->type == D_QUOTE || tmp->type == S_QUOTE
					|| tmp->type == WORD) && (tmp->next->type == REDIR_OUT
					|| tmp->next->type == REDIR_IN || tmp->next->type == PIPE
					|| tmp->next->type == HEREDOC || tmp->next->type == APPEND))
				tmp->is_next_space = false;
		}
		tmp = tmp->next;
	}
	tmp = minishell->token;
}

void	select_type(t_ms *minishell)
{
	t_token	*tmp;

	tmp = minishell->token;
	while (tmp)
	{
		if (!ft_strcmp(tmp->value, ">>"))
			tmp->type = APPEND;
		else if (!ft_strcmp(tmp->value, "<<"))
			tmp->type = HEREDOC;
		else if (!ft_strcmp(tmp->value, "|"))
			tmp->type = PIPE;
		else if (!ft_strcmp(tmp->value, "<"))
			tmp->type = REDIR_IN;
		else if (!ft_strcmp(tmp->value, ">"))
			tmp->type = REDIR_OUT;
		else if (tmp->type == D_QUOTE)
			tmp->type = D_QUOTE;
		else if (tmp->type == S_QUOTE)
			tmp->type = S_QUOTE;
		else
			tmp->type = WORD;
		tmp = tmp->next;
	}
	select_is_space(minishell);
}
