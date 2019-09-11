/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 23:14:40 by rfontain          #+#    #+#             */
/*   Updated: 2018/10/11 05:19:24 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

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

void	deal_solo(int i, int ac, char **av, t_fg *e_fg)
{
	t_lst	*file;
	t_list	*nosuch;
	t_stat	stat;

	if (ac - i == 1 && (lstat(av[i], &stat) == -1) && errno == ENOENT)
	{
		if (!(nosuch = (t_list*)ft_memalloc(sizeof(t_list))))
			exit(2);
		assign_char((char**)&(nosuch->content), av[i]);
		deal_such(nosuch);
	}
	else
	{
		file = (ac - i == 1) ? lst_new(av[i]) : lst_new(".");
		deal_file(file, e_fg);
		free(file->name);
		free(file);
	}
}

int		str_swap(t_indir *one, t_indir *two)
{
	t_indir *tmp_prev;
	t_indir *tmp_next;

	tmp_prev = one->prev;
	tmp_next = two->next;
	one->prev = two;
	one->next = tmp_next;
	two->next = one;
	two->prev = tmp_prev;
	if (one->next)
		one->next->prev = one;
	if (two->prev)
		two->prev->next = two;
	return (0);
}

void	put_ferror(int error, t_lst *lst)
{
	t_indir *indir;

	if (error & F_STAT_FAIL)
		return ;
	else if (error & F_IS_LINK)
	{
		if (*(lst->e_fg) & LONG_LISTING)
		{
			indir = set_indir(lst->name, '-');
			if (!(indir = set_stat_indir(&indir, indir, lst, ".")))
			{
				free(indir->name);
				free(indir);
				return ;
			}
			put_llist(indir, -1, lst, lst->e_fg);
			free_list(indir, lst->e_fg);
		}
		else
			ft_putendl(lst->name);
	}
}

void	put_error(int error, t_lst *lst, DIR *dir)
{
	t_indir		*indir;

	if (dir)
		closedir(dir);
	if (errno == ENOTDIR)
	{
		if (*(lst->e_fg) & LONG_LISTING)
		{
			indir = set_indir(lst->name, '-');
			if (!(indir = set_stat_indir(&indir, indir, lst, ".")))
				return (free_list(indir, lst->e_fg));
			put_llist(indir, -1, lst, lst->e_fg);
			free_list(indir, lst->e_fg);
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
