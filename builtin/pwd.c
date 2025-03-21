/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:58:35 by skock             #+#    #+#             */
/*   Updated: 2025/03/12 15:31:46 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_pwd(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (path)
		printf("%s\n", path);
	else
		perror("getcwd");
}
