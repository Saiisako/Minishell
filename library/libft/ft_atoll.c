/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 21:33:23 by cmontaig          #+#    #+#             */
/*   Updated: 2025/05/19 21:35:46 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	check_overflow(long long result, int digit, int sign)
{
	if (sign == 1 && (result > (LLONG_MAX - digit) / 10))
		return (0);
	if (sign == -1 && (-result < (LLONG_MIN + digit) / 10))
		return (0);
	return (1);
}

int	ft_atoll(const char *str, long long *out)
{
	int			i = 0;
	int			sign = 1;
	long long	result = 0;

	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		int digit = str[i] - '0';
		if (!check_overflow(result, digit, sign))
			return (0);
		result = result * 10 + digit;
		i++;
	}
	*out = result * sign;
	return (1);
}
