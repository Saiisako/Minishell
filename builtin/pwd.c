/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:58:35 by skock             #+#    #+#             */
/*   Updated: 2025/05/16 11:30:56 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// void	print_pwd(void)
// {
// 	char	*path;

// 	path = getcwd(NULL, 0);
// 	if (path)
// 		ft_printf("%s\n", path);
// 	else
// 		perror("getcwd");
// }

void	print_pwd(t_ms *ms)
{
	if (ms->pwd)
		printf("%s\n", ms->pwd);
	else
		printf("?/n");
}
