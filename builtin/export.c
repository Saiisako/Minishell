/* ************************************************************************** */
/*	                                                                        */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:06:33 by cmontaig          #+#    #+#             */
/*   Updated: 2025/04/12 16:17:42 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	export_print_sorted(t_env *env_lst)
{
	t_env	**array;
	t_env	*env;
	int		count;

	env = env_lst;
	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	array = malloc(sizeof(t_env *) * (count + 1));
	if (!array)
		return ;
	fill_env_array(env_lst, array);
	sort_env_array(array);
	print_env_array(array);
	free(array);
}

char	*concat_env_var(char *key, char *value)
{
	char	*result;
	char	*temp;

	if (!value)
		return (ft_strdup(key));
	result = ft_strjoin(key, "=");
	if (!result)
		return (NULL);
	temp = ft_strjoin(result, value);
	free(result);
	if (!temp)
		return (NULL);
	return (temp);
}

int	process_export_arg(t_ms *ms, t_token *arg)
{
	char	*equal_sign;
	char	*key;

	if (arg->type != WORD)
		return (0);
	equal_sign = ft_strchr_lib(arg->value, '=');
	if (equal_sign)
		key = ft_substr(arg->value, 0, equal_sign - arg->value);
	else
		key = ft_strdup(arg->value);
	if (!valid_name(key))
	{
		ft_putstr_fd("export: `", STDERR_FILENO);
		ft_putstr_fd(arg->value, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		free(key);
		return (ms->status = 1, 1);
	}
	export_set_var(ms, arg->value);
	free(key);
	return (0);
}

int	ft_export(t_ms *ms, t_cmd *cmd)
{
	t_token	*arg;

	arg = cmd->token->next;
	ms->status = 0;
	if (!arg)
	{
		export_print_sorted(ms->env_lst);
		return (ms->status = 0, 0);
	}
	while (arg)
	{
		if (arg->type == WORD)
			process_export_arg(ms, arg);
		arg = arg->next;
	}
	update_envp(ms);
	return (ms->status);
}
