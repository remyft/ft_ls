/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 19:50:06 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/04 09:03:08 by rfontain         ###   ########.fr       */
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
				continuer = str_swap(curr);
			else
			{
				i++;
				curr = curr->next;
			}
		}
		size--;
	}
}

t_indir	*set_indir(t_indir *ret, char *name, unsigned char type)
{
	struct stat *buf;

	buf = NULL;
	ret->name = strdup(name);
	ret->type = type;
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
	t_indir			*begin;
	DIR				*dir;
	struct dirent	*indir;

	dir = opendir((char*)lst->name);
	lst->indir = (t_indir*)malloc(sizeof(t_indir));
	lst->indir->next = NULL;
	lst->indir->prev = NULL;
	curr = lst->indir;
	prev = lst->indir;
	begin = lst->indir;
	dir = opendir((char*)lst->name);
	i = 0;
	while ((indir = readdir(dir)))
	{
		curr = set_indir(curr, indir->d_name, indir->d_type);
		curr = curr->next;
		curr = (t_indir*)malloc(sizeof(t_indir));
		curr->prev = prev;
		prev->next = curr;
		prev = curr;
		i++;
	}
	lst->size = i;
	curr->next = NULL;
	sort_alpha(lst->indir, i);
	put_list(begin, lst->size);
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
