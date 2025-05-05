/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:18:40 by skock             #+#    #+#             */
/*   Updated: 2025/05/05 14:03:47 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	parsing_error(t_ms *minishell)
{
	t_token	*tmp;

	tmp = minishell->token;
	while (tmp && tmp->next)
	{
		if (tmp->type != 1 && tmp->next->type != 1)
		{
			minishell->first_special = tmp->type;
			minishell->second_special = tmp->next->type;
			return (0);
		}
		tmp = tmp->next;
	}
	if (tmp && tmp->type != 1)
		return (0);
	tmp = minishell->token;
	if (tmp && tmp->type == PIPE)
		return (0);
	return (1);
}

void	ft_cmd(t_ms *minishell)
{
	fill_cmd_lst(minishell);
	cut_weird(minishell->cmd_list);
	print_cmd(minishell->cmd_list);
}

char	**malloc_file(char *filepath)
{
	int		fd;
	int		i;
	char	*line;
	char	**lines;
	char	**tmp;

	fd = open(filepath, O_RDONLY);
	if (fd < 0)
		return (NULL);
	lines = malloc(sizeof(char *) * 1);
	lines[0] = NULL;
	i = 0;
	line = get_next_line(fd);
	while (line)
	{
		tmp = malloc(sizeof(char *) * (i + 2));
		int j = 0;
		while (j < i)
		{
			tmp[j] = lines[j];
			j++;
		}
		tmp[i] = line;
		tmp[i + 1] = NULL;
		free(lines);
		lines = tmp;
		i++;
		line = get_next_line(fd);
	}
	close(fd);
	return (lines);
}

int	parsing_input(char *input, t_ms *minishell)
{
	int	i;
	minishell->expand = NULL;
	minishell->token = NULL;

	i = 0;
	i = 0;
	while (input[i])
	{
		while (input[i] && ft_iswhitespace(input[i]))
			i++;
		if (!input[i])
			break ;
		if (!process_token(input, &i, minishell))
			return (0);
		if (!input[i])
			break ;
		i++;
	}
	select_type(minishell);
	expand_token(minishell->token, minishell);
	clear_quote(minishell);
	merge_inception(minishell);
	if (!parsing_error(minishell))
		return (0);
	ft_cmd(minishell);
	return (1);
}
