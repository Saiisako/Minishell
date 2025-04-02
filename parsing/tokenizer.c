/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:18:40 by skock             #+#    #+#             */
/*   Updated: 2025/04/02 15:10:06 by skock            ###   ########.fr       */
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

int	is_special(char c)
{
	return (c == '>' || c == '<' || c == '|');
}

t_cmd	*new_cmd(void)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	new->path = NULL;
	new->infile_fd = -2;
	new->outfile_fd = -2;
	new->pid = -1;
	new->next = NULL;
	return (new);
}

t_cmd *create_cmd_until_pipe(t_token *tmp)
{
	t_cmd	*cmd;

	cmd = new_cmd();
	cmd->token = tmp;
	return (cmd);
}

void	fill_cmd_lst(t_ms *minishell)
{
	t_token	*tmp;
	t_cmd	*cmd_list = NULL;
	t_cmd	*last_cmd = NULL;
	t_cmd	*new_cmd_node;

	tmp = minishell->token;
	while (tmp)
	{
		new_cmd_node = create_cmd_until_pipe(tmp);
		if (new_cmd_node)
		{
			if (!cmd_list)
				cmd_list = new_cmd_node;
			else
				last_cmd->next = new_cmd_node;
			last_cmd = new_cmd_node;
		}
		while (tmp && tmp->type != PIPE)
			tmp = tmp->next;
		if (tmp)
			tmp = tmp->next;
	}
	minishell->cmd_list = cmd_list;
}

int	parsing_error(t_ms *minishell)
{
	t_token	*tmp;

	tmp = minishell->token;
	while (tmp && tmp->next)
	{
		if (tmp->type != 1 && tmp->next->type != 1)
			return (0);
		tmp = tmp->next;
	}
	if (tmp && tmp->type != 1)
		return (0);
	return (1);
}

void	cut_weird(t_cmd *cmd)
{
	t_cmd	*tmp_cmd;
	t_token	*tmp_token;
	int		i;

	i = 1;
	tmp_cmd = cmd;
	while (tmp_cmd)
	{
		tmp_token = tmp_cmd->token;
		while (tmp_token)
		{
			tmp_token = tmp_token->next;
			if (tmp_token && tmp_token->type == PIPE)
			{
				tmp_token->next = NULL;
				break ;
			}
		}
		tmp_cmd = tmp_cmd->next;
	}
}

void	print_cmd(t_cmd *cmd)
{
	t_cmd	*tmp_cmd;
	t_token	*tmp_token;
	int		i;

	i = 1;
	tmp_cmd = cmd;
	while (tmp_cmd)
	{
		printf("////// CMD %d///////\n", i++);
		printf("infile fd = %d\n", tmp_cmd->infile_fd);
		printf("outfile fd = %d\n", tmp_cmd->outfile_fd);
		printf("PID = %d\n", tmp_cmd->pid);
		printf("PATH = %s\n", tmp_cmd->path);
		printf(" ////  TOKEN /////\n");
		tmp_token = tmp_cmd->token;
		while (tmp_token)
		{
			printf("token value = %s\n", tmp_token->value);
			printf("token type = %d\n", tmp_token->type);
			printf("\n\n");
			tmp_token = tmp_token->next;
		}
		tmp_cmd = tmp_cmd->next;
	}
}

void free_token(t_token *token)
{
	if (token)
	{
		free(token->value);
		free(token);
	}
}

void free_all(t_ms *minishell)
{
	t_token *tmp_token;

	while (minishell->token)
	{
		tmp_token = minishell->token;
		minishell->token = minishell->token->next;
		free_token(tmp_token);
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
	if (!parsing_error(minishell))
	{
		// free la liste
		return (0);
	}
	// print_tokens(minishell->token);
	fill_cmd_lst(minishell);
	cut_weird(minishell->cmd_list);
	print_cmd(minishell->cmd_list);
	exec_line(minishell);
	free_all(minishell);
	return (1);
}
