/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 13:20:02 by skock             #+#    #+#             */
/*   Updated: 2025/05/26 14:39:57 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("doit fusionner ? -> %d\n", tokens->is_next_space);
		printf("node = {%s}\n", tokens->value);
		tokens = tokens->next;
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
		printf("PID= %d\n", tmp_cmd->pid);
		printf("PATH = %s\n", tmp_cmd->path);
		printf(" //// TOKEN /////\n");
		tmp_token = tmp_cmd->token;
		while (tmp_token)
		{
			printf("token value = |   |%s|   |\n", tmp_token->value);
			printf("token type = %d\n", tmp_token->type);
			printf("index token = %d\n", tmp_token->index);
			printf("\n\n");
			tmp_token = tmp_token->next;
		}
		tmp_cmd = tmp_cmd->next;
	}
}
