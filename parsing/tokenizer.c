/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:18:40 by skock             #+#    #+#             */
/*   Updated: 2025/03/27 16:30:32 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// ligne 59 pourrait poser problème si input[i] est nul selon Claude.

int	double_quote(char *input, int *i, t_ms *minishell)
{
	int		start;
	char	*token;

	start = *i;
	(*i)++;
	while (input[*i] && input[*i] != 34)
		(*i)++;
	if (!input[*i])
		return (printf("unclosed quote : "), 0);
	token = ft_substr(input, start, (*i - start) + 1);
	if (input[(*i) + 1] != ' ' && input[(*i) + 1] != '\0')
		minishell->is_next_space = true;
	fill_token_list(minishell, token, D_QUOTE);
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
		return (printf("unclosed quote : "), 0);
	token = ft_substr(input, start, (*i - start) + 1);
	if (input[(*i) + 1] != ' ' && input[(*i) + 1] != '\0')
		minishell->is_next_space = true;
	fill_token_list(minishell, token, S_QUOTE);
	return (1);
}

void	word_token(char *input, int *i, t_ms *minishell)
{
	int		start;
	char	*token;

	start = *i;
	(*i)++;
	while (input[*i] && !ft_iswhitespace(input[*i])
		&& (input[*i] != 39 && input[*i] != 34))
		(*i)++;
	token = ft_substr(input, start, (*i - start));
	if (input[(*i)] != ' ' && input[(*i)] != '\0')
		minishell->is_next_space = true;
	fill_token_list(minishell, token, WORD);
	(*i)--;
}

void	process_token(char *input, int *i, t_ms *minishell)
{
	if (input[*i] == 34)
	{
		if (!double_quote(input, i, minishell))
			return ;
	}
	else if (input[*i] == 39)
	{
		if (!single_quote(input, i, minishell))
			return ;
	}
	else if (ft_isascii(input[*i]))
		word_token(input, i, minishell);
}

void free_token_list(t_token *head)
{
	t_token *temp;

	while (head != NULL)
	{
		temp = head;
		head = head->next;
		free(temp->value);
		free(temp);
	}
}

int	token_size(t_token *token)
{
	int		i;
	t_token	*tmp;

	i = 0;
	tmp = token;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

void	merge_inception(t_ms *minishell)
{
	t_token	*tmp;

	tmp = minishell->token;
	while (tmp)
	{
		if (tmp->is_next_space == true)
		{
			t_token *next = tmp->next;
			merge_token(minishell);
			tmp = next;
		}
		else
			tmp = tmp->next;
	}
}

void	ft_strreplace(char **token, const char *replace)
{
	char	*new_value;

	if (!token || !*token || !replace)
		return ;

	new_value = strdup(replace);
	if (!new_value)
		return ;

	free(*token);
	*token = new_value;
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

void	create_end_list(t_ms *minishell)
{
	(void)minishell;
	return ;
}

void	select_type(t_ms *minishell)
{
	t_token	*tmp;

	tmp = minishell->token;
	while (tmp)
	{
		if (!ft_strncmp(tmp->value, ">>", 2))
			tmp->type = APPEND;
		else if (!ft_strncmp(tmp->value, "<<", 2))
			tmp->type = HEREDOC;
		else if (!ft_strncmp(tmp->value, "|", 1))
			tmp->type = PIPE;
		else if (!ft_strncmp(tmp->value, "<", 1))
			tmp->type = REDIR_IN;
		else if (!ft_strncmp(tmp->value, ">", 1))
			tmp->type = REDIR_OUT;
		tmp = tmp->next;
	}
}

void	divide_word(t_ms *minishell)
{
	t_token		*tmp;

	tmp = minishell->token;
	while (tmp)
	{
		if (tmp->type == WORD)
		tmp = tmp->next;
	}
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
	// divide_word(minishell);
	expand_token(minishell->token, minishell);
	clear_quote(minishell);
	merge_inception(minishell);
	print_tokens(minishell->token);
	create_end_list(minishell);
	exec_line(minishell);
	return (1);
}
