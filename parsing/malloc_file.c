/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_file.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 11:22:16 by skock             #+#    #+#             */
/*   Updated: 2025/05/12 11:23:05 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**append_line(char **lines, char *line, int count)
{
	char	**tmp;
	int		i;

	tmp = malloc(sizeof(char *) * (count + 2));
	if (!tmp)
		return (NULL);
	i = 0;
	while (i < count)
	{
		tmp[i] = lines[i];
		i++;
	}
	tmp[i] = line;
	tmp[i + 1] = NULL;
	free(lines);
	return (tmp);
}

char	**malloc_file(char *filepath)
{
	int		fd;
	int		count;
	char	*line;
	char	**lines;

	count = 0;
	fd = open(filepath, O_RDONLY);
	if (fd < 0)
		return (NULL);
	lines = malloc(sizeof(char *) * 1);
	if (!lines)
		return (NULL);
	lines[0] = NULL;
	line = get_next_line(fd);
	while (line != NULL)
	{
		lines = append_line(lines, line, count);
		count++;
		line = get_next_line(fd);
	}
	close(fd);
	return (lines);
}
