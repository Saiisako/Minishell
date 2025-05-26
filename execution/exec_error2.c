/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_error2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:26:43 by skock             #+#    #+#             */
/*   Updated: 2025/05/26 14:28:11 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	next_print(t_ms *ms, const char *msg)
{
	if (ms->is_pipe_error == true)
	{
		ms->is_pipe_error = false;
		printf("Invalid Token entered '||'\n");
	}
	else
		printf("%s\n", msg);
}

void	print_error_message(const char *msg, t_ms *minishell)
{
	char	*special;

	if (minishell->unexpected)
	{
		printf("bash: syntax error near unexpected token 'newline'\n");
		minishell->unexpected = false;
		return ;
	}
	if (minishell->first_special != 69 && minishell->second_special != 69)
	{
		if (minishell->second_special == HEREDOC)
			special = ft_strdup("<<");
		if (minishell->second_special == APPEND)
			special = ft_strdup(">>");
		if (minishell->second_special == REDIR_IN)
			special = ft_strdup("<");
		if (minishell->second_special == REDIR_OUT)
			special = ft_strdup(">");
		if (minishell->second_special == PIPE)
			special = ft_strdup("|");
		printf("bash: syntax error near unexpected token '%s'\n", special);
		free(special);
	}
	else
		next_print(minishell, msg);
}
