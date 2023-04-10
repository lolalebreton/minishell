/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtoure <dtoure@student42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 22:43:33 by dtoure            #+#    #+#             */
/*   Updated: 2023/02/18 19:26:15 by dtoure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	set_last_setup(t_cmd *cmd)
{
	size_t	i;
	t_files	**tab;

	i = -1;
	tab = cmd -> tab;
	while (tab[++i])
	{
		if (tab[i]-> type == IN || tab[i]-> type == DOC)
			cmd -> last_in = tab[i];
		else if (tab[i]-> type == OUT || tab[i]-> type == APPEND)
			cmd -> last_out = tab[i];
	}
}

void	fill_cmds_par(t_cmd **cmds, char *to_parse, int length)
{
	int		i;
	size_t	j;

	i = -1;
	j = 0;
	j = skip_spaces(to_parse, j, 0);
	while (++i < length)
	{
		set_parenthese(cmds[i], &to_parse[j]);
		j = skip_char_letter_str(to_parse, j);
		j = skip_char_token_str(to_parse, STOP_, j);
		j = skip_spaces(to_parse, j, 0);
	}
}

int	how_many_cmds(char *to_process)
{
	size_t	i;
	int		count;

	count = 0;
	i = -1;
	while (to_process[++i])
	{
		if ((to_process[i] == '"' && valid_double(to_process, i))
			|| to_process[i] == '\'')
			skip_(to_process, &i, to_process[i]);
		if (ft_strchr(STOP_, to_process[i])
			&& to_process[i + 1] == to_process[i])
		{
			++i;
			count++;
		}
		else if (ft_strchr(STOP_, to_process[i]))
			count++;
	}
	count += add_command(to_process, i);
	return (count);
}

void	fill_cmds(t_cmd **cmds, char *to_parse, int length)
{
	int		i;
	size_t	j;

	i = -1;
	j = skip_spaces(to_parse, 0, 0);
	while (++i < length)
	{
		set_redirect_cmd(cmds[i], &to_parse[j]);
		set_commands(cmds[i], &to_parse[j]);
		j = skip_char_letter_str(to_parse, j);
		j = skip_char_token_str(to_parse, STOP_, j);
		j = skip_spaces(to_parse, j, 0);
		if (cmds[i]-> prev_cmd)
			cmds[i]-> prev_stop = cmds[i]-> prev_cmd -> stop;
	}
}

void	init_cmd(t_data *data, char *to_process)
{
	size_t	len;

	data -> cp_to_parse = ft_strdup(to_process);
	is_error(data, data -> cp_to_parse, MALLOC_ERR, 1);
	len = how_many_cmds(data -> cp_to_parse);
	data -> cmds = ft_calloc(sizeof(t_cmd *), len + 1);
	is_error(data, data -> cmds, MALLOC_ERR, 1);
	par_to_space(data -> cp_to_parse);
	set_default_data(data, len);
	fill_cmds(data -> cmds, data -> cp_to_parse, len);
	fill_cmds_par(data -> cmds, to_process, len);
	real_subshell_or_not(data -> cmds);
}
