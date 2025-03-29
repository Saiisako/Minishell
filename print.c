/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 13:20:02 by skock             #+#    #+#             */
/*   Updated: 2025/03/29 13:21:52 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("TYPE = %d\n", tokens->type);
		printf("doit fusionner ? -> %d\n", tokens->is_next_space);
		printf("node = {%s}\n", tokens->value);
		tokens = tokens->next;
	}
}

