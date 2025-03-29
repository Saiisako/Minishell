/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:18:40 by skock             #+#    #+#             */
/*   Updated: 2025/03/29 13:19:45 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	merge_inception(t_ms *minishell)
{
	t_token	*tmp;
	t_token	*next;

	tmp = minishell->token;
	while (tmp)
	{
		if (tmp->is_next_space == true)
		{
			next = tmp->next;
			merge_token(minishell);
			tmp = next;
		}
		else
			tmp = tmp->next;
	}
}

void do_expand(t_token *token, t_ms *minishell)
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
				while (token->value[i] && (ft_isalnum(token->value[i]) || token->value[i] == '_'))
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
			else if ((tmp->type == D_QUOTE || tmp->type == S_QUOTE
				|| tmp->type == WORD) && (tmp->next->type == REDIR_OUT
				|| tmp->next->type == REDIR_IN || tmp->next->type == PIPE
				|| tmp->next->type == HEREDOC || tmp->next->type == APPEND))
				tmp->is_next_space = false;
		}
		tmp = tmp->next;
	}
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

int is_special(char c)
{
	return (c == '>' || c == '<' || c == '|');
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
	select_type(minishell);
	expand_token(minishell->token, minishell);
	clear_quote(minishell);
	merge_inception(minishell);
	print_tokens(minishell->token);
	exec_line(minishell);
	return (1);
}