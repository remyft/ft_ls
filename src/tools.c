/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 23:14:40 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/18 03:15:15 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

void	deal_solo(int i, int ac, char **av)
{
	t_lst	*file;
	t_list	*nosuch;
	DIR		*dir;

	if (!(dir = opendir(av[i])) && errno == ENOENT)
	{
		nosuch = (t_list*)ft_memalloc(sizeof(t_list));
		nosuch->content = ft_strdup(av[i]);
		deal_such(nosuch);
	}
	else
	{
		file = (ac - i == 1) ? lst_new(av[i]) : lst_new(".");
		deal_file(file);
		free(file->name);
		free(file);
	}
	if (dir)
		closedir(dir);
}

int		str_swap(t_indir *curr)
{
	char	*tmp_name;
	int		tmp_type;
	int		tmp_itime;

	tmp_name = curr->name;
	tmp_type = curr->type;
	tmp_itime = curr->itime;
	curr->name = curr->next->name;
	curr->type = curr->next->type;
	curr->itime = curr->next->itime;
	curr->next->name = tmp_name;
	curr->next->type = tmp_type;
	curr->next->itime = tmp_itime;
	return (0);
}

void	put_ferror(int error, t_lst *lst)
{
	t_indir *indir;

	if (error & F_STAT_FAIL)
		return ;
	else if (error & F_IS_LINK)
	{
		if (g_fg & LONG_LISTING)
		{
			indir = set_indir(lst->name, '-', ".");
			indir = set_stat_indir(indir, ".", lst);
			put_llist(indir, 1, -1, ".");
			if (indir->name && indir->name[0])
				ft_strdel(&(indir->name));
			free_list(indir);
		}
		else
			ft_putendl(lst->name);
	}
}

void	put_error(int error, t_lst *lst)
{
	t_indir		*indir;

	if (errno == ENOTDIR)
	{
		if (g_fg & LONG_LISTING)
		{
			indir = set_indir(lst->name, '-', ".");
			indir = set_stat_indir(indir, ".", lst);
			put_llist(indir, 1, -1, ".");
			if (indir)
				free_list(indir);
		}
		else
			ft_putendl(lst->name);
	}
	else if (error & F_ACCESS_FAIL && errno == EACCES)
	{
		ft_putstr_fd("ft_ls: ", 2);
		ft_putname(lst->name);
		ft_putstr_fd(": Permission denied\n", 2);
	}
	else
		put_ferror(error, lst);
}
