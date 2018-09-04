/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 23:14:40 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/04 09:10:26 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"
#include <stdio.h>

t_lst	*lst_new(char *file)
{
	t_lst *new;

	new = ft_memalloc(sizeof(t_lst));
	if (!(new->name = ft_strdup(file)))
		exit(2);
	return (new);
}

void	put_list(t_indir *list, int size)
{
	int i;
	t_lst	*file;

	i = 0;
	file = NULL;
	while (i < size)
	{
		if (list->name[0] != '.')
		{
			if (list->type & DT_DIR && g_fg & RECURSIVE)
			{
				ft_putchar('\n');
				ft_putstr(list->name);
				ft_putstr(":\n");
				file = lst_new(list->name);
				deal_file(file);
				ft_putchar('\n');
			}
			else
			{
				ft_putstr(list->name);
				ft_putchar('\n');
			}
		}
		list = list->next;
		i++;
	}
}

