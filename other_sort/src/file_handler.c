/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 23:14:40 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/06 19:07:41 by rfontain         ###   ########.fr       */
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

	i = 0;
	while (i < size)
	{
		if (list->name[0] == '.' && !(g_fg & ALL_FILE))
		{
			list = (g_fg & REVERSE) ? list->prev : list->next;
			i++;
		}
		else
		{
			ft_putstr(list->name);
			ft_putchar('\n');
			list = (g_fg & REVERSE) ? list->prev : list->next;
			i++;
		}
	}
	ft_putchar('\n');
}

void	put_dlist(t_indir *list, int size, char *name)
{
	int i;
	t_lst	*file;
	char	*tmp;


	i = 0;
	file = NULL;
	while (i < size)
	{
		tmp = ft_strdup(name);
		if (list->name[0] == '.' && !(g_fg & ALL_FILE))
		{
			list = (g_fg & REVERSE) ? list->prev : list->next;
			i++;
		}
		else if (list->type & DT_DIR && (ft_strcmp(list->name, ".") != 0) && (ft_strcmp(list->name, "..") != 0))
		{
			tmp  = ft_strjoin(tmp, "/");
			tmp = ft_strjoin(tmp, list->name);
			ft_putstr(tmp);
			ft_putstr(":\n");
			file = lst_new(tmp);
			deal_file(file);
			free(tmp);
			list = (g_fg & REVERSE) ? list->prev :  list->next;
			i++;
		}
		else
		{
			list = (g_fg & REVERSE) ? list->prev : list->next;
			i++;
		}
	}
}

