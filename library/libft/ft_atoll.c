/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 21:33:23 by cmontaig          #+#    #+#             */
/*   Updated: 2025/05/26 14:15:17 by skock            ###   ########.fr       */
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

void	assign_value(int *i, int *sign, long long *result)
{
	(*i) = 0;
	(*sign) = 1;
	(*result) = 0;
}

int	ft_atoll(const char *str, long long *out)
{
	int			i;
	int			sign;
	int			digit;
	long long	result;

	assign_value(&i, &sign, &result);
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
		digit = str[i] - '0';
		if (!check_overflow(result, digit, sign))
			return (0);
		result = result * 10 + digit;
		i++;
	}
	*out = result * sign;
	return (1);
}
