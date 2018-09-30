/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_usefull.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/30 18:17:01 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/30 18:21:39 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

t_lst	*ret_list(t_lst *list)
{
	while (list->prev)
		list = list->prev;
	return (list);
}

t_lst	*set_stat_list(t_lst *list)
{
	t_lst	*begin;
	t_stat	stat;

	begin = list;
	while (list)
	{
		if ((lstat(list->name, &stat)) == -1)
		{
			list->stat = 0;
			list = list->next;
		}
		else
		{
			list->itime = stat.st_mtime;
			list->size_f = stat.st_size;
			if (stat.st_mode & S_IFDIR)
				list->isdir = 1;
			list->stat = 1;
			list = list->next;
		}
	}
	return (begin);
}

void	big_free(t_lst *list)
{
	t_lst *curr;

	while (list)
	{
		curr = list->next;
		free(list->name);
		free(list);
		list = curr;
	}
}
