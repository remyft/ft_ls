/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/08 08:54:09 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/12 07:48:58 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

void	free_list(t_indir *curr)
{
	t_indir *prev;

	while (curr)
	{
		prev = curr->next;
		if (g_fg & LONG_LISTING)
		{
			free(curr->right);
			free(curr->uid_user);
			free(curr->gid_user);
			free(curr->time);
		}
		free(curr->name);
		free(curr);
		curr = prev;
	}
}

t_lst	*lst_new(char *file)
{
	t_lst *new;

	new = ft_memalloc(sizeof(t_lst));
	new->name = ft_strdup(file);
	return (new);
}

int		is_long(t_indir *lst)
{
	while (lst)
	{
		if (lst->size >= 100000)
			return (1);
		lst = lst->next;
	}
	return (0);
}

void	put_llist(t_indir *list, int size, int nb_blk)
{
	int		i;
	char	*time;
	int		uid_size;
	int		gid_size;
	char	*space;
	int		tab;

	i = 0;
	uid_size = 0;
	gid_size = 0;
	(void)size;
	i = is_major(list);
	max_size(list, &uid_size, &gid_size);
	tab = is_long(list);
	ft_putstr("total ");
	ft_putnbr(nb_blk);
	ft_putchar('\n');
	while (list)
	{
		if (!(list->name[0] == '.' && !(g_fg & ALL_FILE)))
		{
			ft_putend(list->right, "  ");
			ft_putnbr(list->nb_link);
			ft_putchar('\t');
			space = nb_space(list->uid_user, uid_size);
			ft_putend(list->uid_user, space);
			free(space);
			space = nb_space(list->gid_user, gid_size);
			ft_putend(list->gid_user, space);
			free(space);
			if (list->minor >= 0)
			{
				ft_putnbend(list->major, ",  ");
				ft_putnbend(list->minor, "  ");
			}
			else
				if (tab && list->size < 100000)
					ft_putnbend(list->size, "\t\t");
				else
					ft_putnbend(list->size, "\t");
			if (i)
				ft_putchar('\t');
			time = get_time(list->time, list->itime);
			ft_putend(time, "  ");
			free(time);
			ft_putendl(list->name);
		}
		list = (g_fg & REVERSE) ? list->prev : list->next;
	}
	ft_putchar('\n');
}

void	put_dlist(t_indir *list, int size, char *name)
{
	int		i;
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
		else if ((list->type & DT_DIR || (g_fg & LONG_LISTING && list->right[0] == 'd')) && cmp_file(list->name) && list->right[0] != 'b')
		{
			tmp  = ft_strjoinfree(tmp, "/", 1);
			tmp = ft_strjoinfree(tmp, list->name, 1);
			ft_putend(tmp, ":\n");
			file = lst_new(tmp);
			deal_file(file);
			list = (g_fg & REVERSE) ? list->prev :  list->next;
			i++;
		}
		else
		{
			list = (g_fg & REVERSE) ? list->prev : list->next;
			i++;
		}
		free(tmp);
	}
}

void	put_list(t_indir *list, int size)
{
	int i;

	i = 0;
	while (i < size)
	{
		if (!(list->name[0] == '.' && !(g_fg & ALL_FILE)))
			ft_putendl(list->name);
		list = (g_fg & REVERSE) ? list->prev : list->next;
		i++;
	}
	ft_putchar('\n');
}
