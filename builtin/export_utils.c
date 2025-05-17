/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 15:56:05 by cmontaig          #+#    #+#             */
/*   Updated: 2025/05/17 15:16:33 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	parse_export_var(char *arg, char **key, char **value)
{
	char	*equal_sign;

	equal_sign = ft_strchr_lib(arg, '=');
	if (equal_sign)
	{
		*key = ft_substr(arg, 0, equal_sign - arg);
		*value = ft_strdup(equal_sign + 1);
		if (!*key || (!*value && equal_sign[1] != '\0'))
			return (1);
	}
	else
	{
		*key = ft_strdup(arg);
		if (!*key)
			return (1);
	}
	return (0);
}

void	export_set_var(t_ms *ms, char *arg)
{
	char	*key;
	char	*value;

	key = NULL;
	value = NULL;
	if (parse_export_var(arg, &key, &value))
	{
		free(key);
		free(value);
		ms->status = 1;
		return ;
	}
	if (env_update_or_add(&ms->env_lst, key, value, ms) != 0)
		ms->status = 1;
	free(key);
	free(value);
}

void	fill_env_array(t_env *env_lst, t_env **array)
{
	t_env	*env;
	int		i;

	env = env_lst;
	i = 0;
	while (env)
	{
		array[i++] = env;
		env = env->next;
	}
	array[i] = NULL;
}

void	sort_env_array(t_env **array)
{
	t_env	*temp;
	int		i;
	int		swapped;

	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		i = 0;
		while (array[i] && array[i + 1])
		{
			if (ft_strcmp(array[i]->key, array[i + 1]->key) > 0)
			{
				temp = array[i];
				array[i] = array[i + 1];
				array[i + 1] = temp;
				swapped = 1;
			}
			i++;
		}
	}
}

void	print_env_array(t_env **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(array[i]->key, STDOUT_FILENO);
		if (array[i]->value)
		{
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(array[i]->value, STDOUT_FILENO);
			ft_putstr_fd("\"\n", STDOUT_FILENO);
		}
		else
			ft_putstr_fd("\n", STDOUT_FILENO);
		i++;
	}
}
