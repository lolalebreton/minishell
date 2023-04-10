/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner_utility.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtoure <dtoure@student42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 01:47:46 by dtoure            #+#    #+#             */
/*   Updated: 2023/02/22 05:19:13 by dtoure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

size_t	copy(char *line, char *res, int quoted)
{
	size_t	i;
	size_t	j;
	char	quote;

	j = 0;
	i = -1;
	while (line[++i])
	{
		if ((line[i] == '"' && valid_double(line, i)) || line[i] == '\'')
		{
			quote = line[i];
			while (line[++i] && line[i] != quote)
			{
				if (quoted && ft_isspace(line[i]))
					line[i] = -1 * line[i];
				res[j++] = line[i];
			}
		}
		else
			res[j++] = line[i];
		i -= (line[i] == 0);
	}
	return (j);
}

void	back_to_space(char **tab)
{
	int		i;
	char	space;
	size_t	j;

	i = -1;
	while (tab[++i])
	{
		j = -1;
		while (tab[i][++j])
		{
			space = tab[i][j] * -1;
			if (ft_isspace(space))
				tab[i][j] *= -1;
		}
	}
}

int	find_other_half(char *line, int to_find, size_t i)
{
	while (line[++i])
	{
		if ((to_find == '"' && line[i] == '"') && valid_double(line, i))
			return (1);
		else if (to_find == '\'' && line[i] == '\'')
			return (1);
	}
	return (0);
}
