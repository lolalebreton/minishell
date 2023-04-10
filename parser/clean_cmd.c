/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtoure <dtoure@student42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 02:13:18 by dtoure            #+#    #+#             */
/*   Updated: 2023/03/17 09:24:44 by dtoure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	copy_tab_to_str(char **tab, char *to_clean)
{
	size_t	i;
	size_t	j;
	size_t	k;

	k = -1;
	i = -1;
	while (tab[++i])
	{
		j = -1;
		while (tab[i][++j])
			to_clean[++k] = tab[i][j];
		if (tab[i + 1])
			to_clean[++k] = ' ';
	}
}

char	*from_tab_to_line(t_cmd *cmd, char **tab)
{
	size_t	len;
	char	*to_clean;

	len = ft_str_tab_len(tab) + ft_tab_len(tab);
	to_clean = ft_calloc(sizeof(char), len + 1);
	is_error(cmd -> data, to_clean, MALLOC_ERR, 0);
	copy_tab_to_str(tab, to_clean);
	cmd -> args = ft_free_tab(tab);
	return (to_clean);
}

void	expanded_tab(t_cmd *cmd, char **tab)
{
	char	*to_clean;

	to_clean = from_tab_to_line(cmd, tab);
	to_clean = clean_(cmd -> data, to_clean, 1);
	tab = ft_split(to_clean, ' ');
	is_error(cmd -> data, tab, MALLOC_ERR, 0);
	ft_free_elem((void **)&to_clean);
	cmd -> args = tab;
	cmd -> cmd = tab[0];
}

int	check_tab(char **tab)
{
	size_t	i;
	size_t	j;
	int		quotes;

	quotes = 0;
	i = -1;
	while (tab[++i])
	{
		j = -1;
		while (tab[i][++j])
		{
			if (tab[i][j] == '$')
				return (1);
			else if ((tab[i][j] == '"' && valid_double(tab[i], j))
				|| (tab[i][j] == '\'' || tab[i][j] == '\\'))
				quotes = -1;
		}
	}
	return (quotes);
}

void	clean_cmd(t_cmd *cmd)
{
	int		to_clean;
	size_t	i;
	char	**tab;

	i = -1;
	if (cmd -> args == NULL)
		return ;
	tab = cmd -> args;
	from_alias_to_hero(cmd -> data, cmd, tab);
	to_clean = check_tab(cmd -> args);
	tab = cmd -> args;
	if (to_clean < 0)
		while (tab[++i])
			tab[i] = clean_lines(cmd -> data, tab[i], 0);
	else if (to_clean > 0)
		expanded_tab(cmd, cmd -> args);
	back_to_space(cmd -> args);
	if (to_clean > 0 && cmd -> cmd && cmd -> args[0] != cmd -> cmd)
		ft_free_elem((void **)&cmd -> cmd);
	cmd -> cmd = cmd -> args[0];
	init_path(cmd);
	cmd -> to_not_calloc = 1;
}
