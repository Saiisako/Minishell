/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:18:40 by skock             #+#    #+#             */
/*   Updated: 2025/03/28 14:13:40 by skock            ###   ########.fr       */
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
}

int is_special(char c)
{
	return (c == '>' || c == '<' || c == '|');
}

char **split_special(const char *str)
{
    int i;
    int j;
    char **result;
    int start;

    i = 0;
    j = 0;
    result = malloc(sizeof(char *) * (strlen(str) + 1));  // allocate memory
    while (str[i])
    {
        start = i;

        // Skip over normal characters (non-special)
        while (str[i] && !is_special(str[i]))
            i++;

        // If we have some characters to add as a token
        if (i > start)
            result[j++] = strndup(&str[start], i - start);

        // Handle special characters
        while (str[i] && is_special(str[i]))
        {
            // Check for double special characters like ">>" or "<<"
            if ((str[i] == '>' && str[i + 1] == '>') || (str[i] == '<' && str[i + 1] == '<'))
            {
                result[j++] = strndup(&str[i], 2);  // handle ">>" or "<<"
                i += 2;  // Move past the special pair
            }
            else
            {
                result[j++] = strndup(&str[i], 1);  // handle single special character
                i++;  // Move past the special character
            }
        }
    }

    result[j] = NULL;  // Null terminate the array of strings
    return (result);
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

void divide_word(t_ms *minishell)
{
	t_token *tmp;
	char **divide;
	int i;

	minishell->exec = NULL;
	tmp = minishell->token;
	while (tmp)
	{
		if (tmp->type == WORD)
		{
			i = 0;
			divide = split_special(tmp->value);
			while (divide[i])
			{
				create_list(minishell, divide[i], WORD);
				i++;
			}
			tmp = tmp->next;
		}
		else
		{
			create_list(minishell, tmp->value, tmp->type);
			tmp = tmp->next;
		}
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
	expand_token(minishell->token, minishell);
	clear_quote(minishell);
	merge_inception(minishell);
	select_type(minishell);
	print_tokens(minishell->token);
	divide_word(minishell);
	print_exec(minishell->exec);
	exec_line(minishell);
	return (1);
}