/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 19:50:06 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/05 12:24:57 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

int		str_swap(t_indir *names)
{
	char *tmp;
	unsigned int type;

	tmp = names->name;
	type = names->type;
	names->name = names->next->name;
	names->type = names->next->type;
	names->next->name = tmp;
	names->next->type = type;
	return (0);
}

void	sort_alpha(t_indir *names, int size)
{
	int i;
	int continuer;
	t_indir *curr;

	continuer = 0;
	while (!continuer)
	{
		continuer = 1;
		i = 0;
		curr = names;
		while (i < size)
		{
			if (i + 1 != size && strcmp(curr->name, curr->next->name) > 0)
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

void	deal_file(t_lst *lst)
{
	int				i;
	t_indir			*prev;
	t_indir			*curr;
	DIR				*dir;
	struct dirent	*indir;

//	lst->indir = (t_indir*)malloc(sizeof(t_indir));
//	lst->indir->next = NULL;
//	lst->indir->prev = NULL;
	curr = NULL;
	dir = opendir((char*)lst->name);
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
	sort_alpha(lst->indir, i);
	(g_fg & REVERSE) ? put_list(prev, i) : put_list(lst->indir, i);
	if (g_fg & RECURSIVE)
		(g_fg & REVERSE) ? put_dlist(prev, i, lst->name) : put_dlist(lst->indir, i, lst->name);
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
