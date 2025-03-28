/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:18:40 by skock             #+#    #+#             */
/*   Updated: 2025/03/28 19:32:35 by skock            ###   ########.fr       */
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
		return (0);
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
		return (0);
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
		&& (input[*i] != 39 && input[*i] != 34) && ((input[*i] != '|') && (input[*i] != '>') && (input[*i] != '<')))
		(*i)++;
	token = ft_substr(input, start, (*i - start));
	if (input[(*i)] != ' ' && input[(*i)] != '\0')
		minishell->is_next_space = true;
	fill_token_list(minishell, token, WORD);
	(*i)--;
}

void	special_token(char *input, int *i, t_ms *minishell)
{
	int		start;
	char	*token;

	start = *i;
	if (input[*i] == '|')
	{
		token = ft_strdup("|");
		fill_token_list(minishell, token, WORD);
		(*i)++;
	}
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		token = ft_strdup(">>");
		fill_token_list(minishell, token, WORD);
		(*i)++;
		(*i)++;
	}
	else if (input[*i] == '<' && input[*i + 1] == '<')
	{
		token = ft_strdup("<<");
		fill_token_list(minishell, token, WORD);
		(*i)++;
		(*i)++;
	}
	else if (input[*i] == '>')
	{
		token = ft_strdup(">");
		fill_token_list(minishell, token, WORD);
		(*i)++;
	}
	else if (input[*i] == '<')
	{
		token = ft_strdup("<");
		fill_token_list(minishell, token, WORD);
		(*i)++;
	}
	(*i)--;
}

void	exit_error(const char *msg, t_ms *minishell)
{
	(void)minishell;
	printf("%s", msg);
	exit(1);
}

void	process_token(char *input, int *i, t_ms *minishell)
{
	if (input[*i] == 34)
	{
		if (!double_quote(input, i, minishell))
			exit_error("Error : Unclosed quote -> \"\n", minishell);
	}
	else if (input[*i] == 39)
	{
		if (!single_quote(input, i, minishell))
			exit_error("Error : Unclosed quote -> '\n", minishell);
	}
	else if (input[*i] == '>' || input[*i] == '<' || input[*i] == '|')
		special_token(input, i, minishell);
	else if (ft_isascii(input[*i]))
		word_token(input, i, minishell);
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
			else if (tmp->next && ((tmp->type == D_QUOTE || tmp->type == S_QUOTE    // peut etre probleme ici mais je suis pas sur.
				|| tmp->type == WORD) && (tmp->next->type == REDIR_OUT
				|| tmp->next->type == REDIR_IN || tmp->next->type == PIPE
				|| tmp->next->type == HEREDOC || tmp->next->type == APPEND)))
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

void create_list(t_ms *minishell, char *value, t_type type)
{
	t_exec	*new_node;
	t_exec	*current;

	new_node = malloc(sizeof(t_exec));
	new_node->value = strdup(value);
	new_node->type = type;
	new_node->next = NULL;
	if (!minishell->exec)
		minishell->exec = new_node;
	else
	{
		current = minishell->exec;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}



void	print_exec(t_exec *exec)
{
	while (exec)
	{
		printf("TYPE = %d\n", exec->type);
		printf("node = {%s}\n", exec->value);
		exec = exec->next;
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
	expand_token(minishell->token, minishell);
	clear_quote(minishell);
	merge_inception(minishell);
	print_tokens(minishell->token);
	exec_line(minishell);
	return (1);
}