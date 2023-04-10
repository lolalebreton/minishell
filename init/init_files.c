/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtoure <dtoure@student42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 01:59:33 by dtoure            #+#    #+#             */
/*   Updated: 2023/02/11 16:39:59 by dtoure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_files	*copy_files(t_data *data, char *to_parse, int k, int j)
{
	t_files	*redirect;
	int		i;
	int		m;

	i = -1;
	m = k;
	while (to_parse[++k] && to_parse[k] == to_parse[k - 1])
		;
	while (to_parse[k] && (to_parse[k] == ' ' || to_parse[k] == '\t'))
		k++;
	redirect = ft_calloc(sizeof(t_files), 1);
	is_error(data, redirect, MALLOC_ERR, 0);
	redirect -> files = ft_calloc(sizeof(char), (j - k + 1));
	is_error(data, redirect -> files, MALLOC_ERR, 0);
	file_type(redirect, to_parse[m], to_parse[m + 1]);
	while (k < j)
		redirect -> files[++i] = to_parse[k++];
	return (redirect);
}

int	skip_to_redirect(char *to_parse, size_t i)
{
	while (to_parse[i])
	{
		if ((to_parse[i] == '"' && valid_double(to_parse, i))
			|| to_parse[i] == '\'')
			skip_(to_parse, &i, to_parse[i]);
		if (ft_strchr(R_COMBO, to_parse[i]))
			return (i);
		if (ft_strchr(STOP_, to_parse[i]))
			return (-1);
		i++;
	}
	return (-1);
}

int	find_tab_length(t_cmd *cmd, char *to_parse)
{
	size_t	i;
	int		k;

	k = 0;
	i = -1;
	while (to_parse[++i])
	{
		if ((to_parse[i] == '"' && valid_double(to_parse, i))
			|| to_parse[i] == '\'')
			skip_(to_parse, &i, to_parse[i]);
		else if (ft_strchr(R_COMBO, to_parse[i]))
		{
			k++;
			i += (ft_strchr(R_COMBO, to_parse[i + 1]) != 0);
		}
		else if (ft_strchr(STOP_, to_parse[i]))
			break ;
	}
	if (to_parse[i] && is_same_token(to_parse[i], to_parse[i + 1]))
		ft_memcpy(cmd-> stop, &to_parse[i], 2);
	cmd -> stop[0] = to_parse[i] - (to_parse[i] == 0);
	if (!k)
		return (-1);
	return (k);
}

void	set_file_tabs(t_cmd *cmd, char *to_parse, int length)
{
	int		i;
	int		j;
	int		k;

	i = -1;
	j = 0;
	while (++i < length)
	{
		j = skip_to_redirect(to_parse, j);
		if (j == -1 || to_parse[j] == 0)
			return ;
		k = j;
		j += (ft_strchr(R_COMBO, to_parse[j + 1]) != NULL) + 1;
		j = skip_spaces(to_parse, j, 1);
		j = length_word(to_parse, j);
		cmd -> tab[i] = copy_files(cmd -> data, to_parse, k, j);
		j -= (to_parse[j] == 0);
	}
}

void	set_redirect_cmd(t_cmd *cmd, char *to_parse)
{
	t_data	*data;
	int		length;

	data = cmd -> data;
	length = find_tab_length(cmd, to_parse);
	if (length == -1)
		return ;
	cmd -> tab = ft_calloc(sizeof(t_files *), length + 1);
	is_error(data, cmd -> tab, MALLOC_ERR, 0);
	set_file_tabs(cmd, to_parse, length);
}
