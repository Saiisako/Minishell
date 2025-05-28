/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:37:09 by cmontaig          #+#    #+#             */
/*   Updated: 2025/05/28 15:37:43 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_minishell(t_ms *minishell)
{
	if (!minishell)
		return ;
	if (minishell->cmd_list)
		free_cmd_list(minishell->cmd_list);
	if (minishell->token)
		free_token_list(minishell->token);
	if (minishell->expand)
		free_token_list(minishell->expand);
	if (minishell->env_lst)
		free_env(minishell);
	if (minishell->envp)
		free_array(minishell->envp);
	if (minishell->current_prompt)
		free(minishell->current_prompt);
	if (minishell->pwd)
		free(minishell->pwd);
	if (minishell->const_pwd)
		free(minishell->const_pwd);
	free(minishell);
}
