/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtoure <dtoure@student42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 04:53:07 by dtoure            #+#    #+#             */
/*   Updated: 2023/03/09 03:55:47 by dtoure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

t_collector	*g_collector = NULL;

void	shell_routine(t_data *data)
{
	int	i;
	int	skip;

	i = -1;
	skip = 0;
	while (data -> tab_[++i])
	{
		data -> cp_to_parse = data -> tab_[i];
		init_cmd(data, data -> cp_to_parse);
		open_here_doc(data, data -> cmds);
		skip = fork_docs(data, &data -> here_docs);
		close_all_pipes(data, &data -> here_docs, 0, 1);
		if (skip == 0)
			executing(data, data -> cmds);
		clean_struct(data);
	}
	while (--i > -1)
		add_history(data -> tab_[i]);
	data -> tab_ = ft_free_tab(data -> tab_);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	*data;

	init_struct(&data);
	if (argc == 2)
		populate(data, argv[1]);
	init_env(data, envp);
	g_collector = ft_calloc(sizeof(t_collector), 1);
	is_error(data, g_collector, MALLOC_ERR, 0);
	g_collector -> data = data;
	lets_read(data);
}
