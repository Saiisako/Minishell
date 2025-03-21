/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:18:40 by skock             #+#    #+#             */
/*   Updated: 2025/03/21 14:13:30 by skock            ###   ########.fr       */
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

// void	merge_inception(t_ms *minishell)
// {
// 	t_token	*tmp;

// 	tmp = minishell->token;
// 	while (tmp)
// 	{
// 		if (tmp->is_next_space == true)
// 		{
// 			t_token *next = tmp->next; // Sauvegarde du prochain élément
// 			merge_token(minishell);
// 			tmp = next; // Continuer après la fusion
// 		}
// 		else
// 			tmp = tmp->next;
// 	}
// }

void	do_expand(t_token *token, t_ms *minishell)
{
	int		i;
	int		start;
	char	*exp;
	i = 0;

	if (token->type == WORD || token->type == D_QUOTE)
	{
		while (token->value[i])
		{
			if (token->value[i] == '$')
			{
				start = i;
				while (token->value[i] && (ft_isalnum(token->value[i])
					|| token->value[i] == '_' || token->value[i] == '$' || token->value[i] == '$'))
						i++;
				exp = ft_substr(token->value, start, i);
				printf("exp = %s\n", exp);
				t_env	*tmp = minishell->env_lst;
				bool	flag = false;
				while (tmp)
				{
					if (!ft_strcmp(tmp->key, exp + 1))
					{
						flag = true;
						exp = ft_strdup(tmp->value);
					}
					tmp = tmp->next;
				}
				if (flag == false)
					ft_strclear(token->value, exp);
				printf("to be replace %s\n", ft_substr(token->value, start, i));
				printf("to replace %s\n", exp);
				printf("%d\n", flag);
				printf("\nonce expand -> : %s\n", token->value);
			}
			i++;
		}
	}
	printf("%s\n", token->value);
	printf("After\n");
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
	// printf("before\n");
	// print_tokens(minishell->token);
	// merge_inception(minishell);
	// merge_token(minishell);
	// print_tokens(minishell->token);
	// printf("before\n");
	// merge_token(minishell);
	// print_tokens(minishell->token);
	// printf("before\n");
	// merge_token(minishell);
	// print_tokens(minishell->token);
	// printf("before\n");
	// merge_token(minishell);
	// print_tokens(minishell->token);
	// printf("before\n");
	// merge_token(minishell);
	// print_tokens(minishell->token);
	// printf("AFTERRRRRRRRRRRRRRRRR\n\n\n");
	// print_tokens(minishell->token);
	free_token_list(minishell->token);
	minishell->is_next_space = false;
	return (1);
}
