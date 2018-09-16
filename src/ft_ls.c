/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 19:50:06 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/16 15:53:29 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

t_indir	*set_list(DIR *dir, int *i, t_lst *lst, t_indir **end)
{
	t_indir			*begin;
	t_indir			*curr;
	t_indir			*prev;
	struct dirent	*indir;

	prev = NULL;
	while ((indir = readdir(dir)))
	{
		if (!(curr = set_indir(indir->d_name, indir->d_type, lst->name)))
		{
			return (NULL);
			put_error(1 << 2, lst);
		}
		curr->prev = (*end);
		if ((*end))
			(*end)->next = curr;
		else
			begin = curr;
		(*end) = curr;
		curr = curr->next;
		(*i) += 1;
	}
	return (begin);
}

void	deal_flags(t_lst *lst, t_indir *end, int size)
{
	t_indir *curr;

	if (g_fg & DATE_SORT)
		sort_date(lst->indir, size);
	if (g_fg & LONG_LISTING || g_fg & DATE_SORT)
	{
		curr = lst->indir;
		while (curr)
		{
			if (!(curr = set_stat_indir(curr, lst->name, lst)))
				return (put_error(1 << 2, lst));
			curr = curr->next;
		}
	}
	if (g_fg & LONG_LISTING)
		(g_fg & REVERSE) ? put_llist(end, size, lst->nb_blk, lst->name) :
			put_llist(lst->indir, size, lst->nb_blk, lst->name);
	else
		(g_fg & REVERSE) ? put_list(end, size) : put_list(lst->indir, size);
	if (g_fg & RECURSIVE)
	{
		(g_fg & REVERSE) ? put_dlist(end, size, lst) :
			put_dlist(lst->indir, size, lst);
	}
}

void	deal_file(t_lst *lst)
{
	int				i;
	t_indir			*end;
	DIR				*dir;
	char			tmp[256];

	end = NULL;
	lst->nb_blk = 0;
	if (!(dir = opendir((char*)lst->name)))
		return (put_error(1, lst));
	if (readlink(lst->name, &tmp[0], 32) != -1)
		return (put_error(F_IS_LINK, lst));
	i = 0;
	lst->indir = set_list(dir, &i, lst, &end);
	lst->size = i;
	sort_alpha(lst->indir, i);
	deal_flags(lst, end, lst->size);
	if (dir)
		closedir(dir);
	while (lst->indir->prev)
		lst->indir = lst->indir->prev;
	free_list(lst->indir);
	if (lst && lst->name)
		ft_strdel(&(lst->name));
	if (lst)
		ft_memdel((void**)&lst);
}

void	list_swap(t_lst *curr, t_lst *next)
{
	char	*curr_name;
	int		curr_isdir;

	curr_name = curr->name;
	curr_isdir = curr->isdir;
	curr->name = next->name;
	curr->isdir = next->isdir;
	next->name = curr_name;
	next->isdir = curr_isdir;
}

t_lst	*sort_list(t_lst *list)
{
	t_lst	*begin;

	begin = list;
	while (list->next)
	{
		if (list->isdir == 1 && list->next->isdir == 0)
		{
			list_swap(list, list->next);
			list = begin;
		}
		else
			list = list->next;
	}
	return (begin);
}

t_lst	*sort_not_dir(t_lst *list)
{
	t_lst *begin;

	begin = list;
	while (list->next && list->next->isdir == 0)
	{
		if ((g_fg & REVERSE && ft_strcmp(list->name, list->next->name) < 0) ||
				(!(g_fg & REVERSE) &&
				 ft_strcmp(list->name, list->next->name) > 0))
		{
			list_swap(list, list->next);
			list = begin;
		}
		else
			list = list->next;
	}
	return (list->next);
}

void	sort_dir(t_lst *list)
{
	t_lst *begin;

	begin = list;
	while (list && list->next)
	{
		if ((g_fg & REVERSE && ft_strcmp(list->name, list->next->name) > 0) ||
				(!(g_fg & REVERSE) &&
				 ft_strcmp(list->name, list->next->name) < 0))
		{
			list_swap(list, list->next);
			list = begin;
		}
		else
			list = list->next;
	}
}

t_lst	*big_deal(t_lst *list)
{
	t_lst	*begin;
	t_lst	*mid;

	begin = list;
	list = begin;
	list = sort_list(list);
	mid = sort_not_dir(list);
	sort_dir(mid);
	while (begin)
	{
		if (begin->prev && begin->isdir == 1)
			ft_putchar('\n');
		if (list->size != 1 && begin->isdir == 1)
			ft_putend(begin->name, ":\n");
		deal_file(begin);
		mid = begin;
		begin = begin->next;
	}
	return (begin);
}

void	put_nosuch(t_list *such)
{
	t_list *curr;

	while (such)
	{
		curr = such->next;
		ft_putstr_fd("ft_ls: ", 2);
		ft_putstr_fd((char*)(such->content), 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		free(such->content);
		free(such);
		such = curr;
	}
}

void	sort_such(t_list *such)
{
	t_list	*begin;
	char	*tmp;

	begin = such;
	while (such->next)
	{
		if (ft_strcmp((char*)(such->content), (char*)(such->next->content)) > 0)
		{
			tmp = such->content;
			such->content = such->next->content;
			such->next->content = tmp;
			such = begin;
		}
		else
			such = such->next;
	}
	put_nosuch(begin);
}

int		main(int ac, char **av)
{
	int		i;
	t_lst	*file;
	t_lst	*prev;
	t_lst	*begin;
	DIR		*dir;
	t_list	*nosuch;
	t_list	*begsuch;
	t_list	*currsuch;

	i = 1;
	file = NULL;
	prev = NULL;
	begin = NULL;
	nosuch = NULL;
	begsuch = NULL;
	currsuch = NULL;
	while (av[i] && av[i][0] == '-' && ft_strlen(av[i]) >= 2)
		get_flag(av[i++]);
	if (ac - i >= 2)
	{
		while (i < ac)
		{
			if (!(dir = opendir(av[i])) && errno == ENOENT)
			{
				nosuch = ft_memalloc(sizeof(t_list));
				if (currsuch)
					currsuch->next = nosuch;
				if (!(begsuch))
					begsuch = nosuch;
				nosuch->content = ft_strdup(av[i]);
				currsuch = nosuch;
				nosuch = nosuch->next;
			}
			else
			{
				file = lst_new(av[i]);
				if (!begin)
					begin = file;
				if (prev)
				{
					file->prev = prev;
					prev->next = file;
				}
				prev = file;
				file = file->next;
			}
			i++;
		}
		if (begsuch)
			sort_such(begsuch);
		if (begin)
			file = big_deal(begin);
	}
	else
	{
		file = (ac - i == 1) ? lst_new(av[i]) : lst_new(".");
		deal_file(file);
	}
	return (0);
}
