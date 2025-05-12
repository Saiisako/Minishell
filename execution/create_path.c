/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ChloeMontaigut <ChloeMontaigut@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 07:25:17 by cmontaig          #+#    #+#             */
/*   Updated: 2025/05/11 17:08:21 by ChloeMontai      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*check_paths(char **paths, char *cmd)
{
	char	*tmp;
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		if (!tmp)
			return (NULL);
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!full_path)
			return (NULL);
		if (access(full_path, F_OK | X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*find_command_path(char *cmd, t_env *env)
{
	char	*path_env;
	char	**paths;
	char	*found;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_env = get_env_value(env, "PATH");
	if (!path_env || !(paths = ft_split(path_env, ':')))
		return (NULL);
	found = check_paths(paths, cmd);
	free_array(paths);
	return (found);
}

int	count_tokens(t_token *t)
{
	int	count = 0;

	while (t && t->type != PIPE)
	{
		if (t->type == WORD || t->type == EXPANDING
			|| t->type == S_QUOTE || t->type == D_QUOTE)
			count++;
		else if ((t->type == REDIR_IN || t->type == REDIR_OUT
			|| t->type == APPEND || t->type == HEREDOC) && t->next)
			t = t->next;
		if (!t)
			break;
		t = t->next;
	}
	return (count);
}

char	**tokens_to_args(t_token *token)
{
	char	**args;
	int		i = 0;
	int		count = count_tokens(token);

	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	while (token && token->type != PIPE)
	{
		if (token->type == WORD || token->type == EXPANDING
			|| token->type == S_QUOTE || token->type == D_QUOTE)
			args[i++] = ft_strdup(token->value);
		else if ((token->type == REDIR_IN || token->type == REDIR_OUT
			|| token->type == APPEND || token->type == HEREDOC) && token->next)
			token = token->next;
		if (!token)
			break;
		token = token->next;
	}
	args[i] = NULL;
	return (args);
}

char *get_env_value(t_env *env, char *key)
{
	t_env *current;
		
	current = env;
	while (current)
	{
		if (!ft_strcmp(current->key, key))
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

