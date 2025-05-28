/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:42:17 by skock             #+#    #+#             */
/*   Updated: 2025/05/28 15:24:32 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_heredoc_error(t_ms *ms, char *input)
{
	free_cmd_list(ms->cmd_list);
	ms->cmd_list = NULL;
	free(input);
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	handle_null_input(t_ms *ms)
{
	write(1, "exit\n", 5);
	free_env(ms);
	free(ms->current_prompt);
	free(ms->pwd);
	free_array(ms->envp);
	free(ms->const_pwd);
	free(ms);
	exit(0);
}
