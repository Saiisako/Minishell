/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:17:56 by skock             #+#    #+#             */
/*   Updated: 2025/03/21 13:18:20 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_strclear(char *str, const char *remove)
{
	char	*pos;
	int		len_remove;

	if (!str || !remove)
		return;
	pos = ft_strstr(str, remove);
	if (!pos)
		return;
	len_remove = ft_strlen(remove);
	while (*pos)
	{
		*pos = *(pos + len_remove);
		pos++;
	}
}
