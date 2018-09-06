/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 19:50:06 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/06 19:18:24 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

#include <stdio.h>

int		str_swap(t_indir *curr)
{
	char	*tmp_name;
	int		tmp_type;

	tmp_name  = curr->name;
	tmp_type = curr->type;
	curr->name = curr->next->name;
	curr->type = curr->next->type;
	curr->next->name = tmp_name;
	curr->next->type = tmp_type;
	return (0);
}

void	sort_alpha(t_indir **names, int size)
{
	int i;
	int continuer;
	t_indir *curr;

	continuer = 0;
	while (!continuer)
	{
		continuer = 1;
		i = 0;
		curr = (*names);
		while (i < size)
		{
			if (i + 1 != size &&  strcmp(curr->name, curr->next->name) > 0)
			{
				continuer = str_swap(curr);
			}
			else
			{
				i++;
				curr = curr->next;
			}
		}
		size--;
	}
}

static t_indir	*set_indir(char *name, unsigned char type)
{
	struct stat *buf;
	t_indir	*ret;
	ret = (t_indir*)malloc(sizeof(t_indir));

	buf = NULL;
	ret->name = strdup(name);
	ret->type = type;
	ret->next = NULL;
	ret->prev = NULL;
	/*if (stat(ret->name, buf) == -1)
		exit(2);
	ret->type = buf->st_mode;*/
	return (ret);
}

void	put_error(int error, char *name)
{
	if (error & F_access_fail)
	{
		ft_putstr("ft_ls: ");
		ft_putstr(name);
		ft_putstr(": Permission denied\n");
	}	
}

void	deal_file(t_lst *lst)
{
	int				i;
	t_indir			*prev;
	t_indir			*curr;
	DIR				*dir;
	struct dirent	*indir;

	curr = NULL;
	if (!(dir = opendir((char*)lst->name)))
		return (put_error(1, lst->name));
	i = 0;
	if ((indir = readdir(dir)))
	{
		lst->indir = set_indir(indir->d_name, indir->d_type);
		i++;
	}
	while ((indir = readdir(dir)))
	{
		curr = set_indir(indir->d_name, indir->d_type);
		if (i == 1)
		{
			lst->indir->next = curr;
			curr->prev = lst->indir;
		}
		else
			curr->prev = prev;
		if (i != 1) 
			prev->next = curr;
		prev = curr;
		curr = curr->next;
		i++;
	}
	lst->size = i;
	sort_alpha(&(lst->indir), i);
	(g_fg & REVERSE) ? put_list(prev, i) : put_list(lst->indir, i);
	if (g_fg & RECURSIVE)
		(g_fg & REVERSE) ? put_dlist(prev, i, lst->name) : put_dlist(lst->indir, i, lst->name);
	if (dir)
		closedir(dir);
}

int		main(int ac, char **av)
{
	int		i;
	t_lst	*file;

	i = 1;
	file = NULL;
	while (av[i] && av[i][0] == '-' && ft_strlen(av[i]) >= 2)
		get_flag(av[i++]);
	while (i < ac)
	{
		file = lst_new(av[i]);
		deal_file(file);
		i++;
	}
	if (!file)
	{
		file = lst_new(".");
		deal_file(file);
	}
	return (0);
}
