/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 19:50:06 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/07 06:43:50 by rfontain         ###   ########.fr       */
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
			if (i + 1 != size &&  strcmp(curr->name, curr->next->name) > 0)
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

char	set_type(mode_t file_stat)
{
	if (S_ISREG(file_stat))
		return ('-');
	else if (S_ISDIR(file_stat))
		return ('d');
	else if (S_ISBLK(file_stat))
		return ('b');
	else if (S_ISCHR(file_stat))
		return ('c');
	else if (S_ISFIFO(file_stat))
		return ('p');
	else if (S_ISLNK(file_stat))
		return ('l');
	else if (S_ISSOCK(file_stat))
		return ('s');
	else
		return ('?');
}

char	*set_right(mode_t file_stat)
{
	char *tab;

	tab = (char*)malloc(sizeof(char) * 11);
	tab[0] = set_type(file_stat);
	tab[1] = file_stat & S_IRUSR ? 'r' : '-';
	tab[2] = file_stat & S_IWUSR ? 'w' : '-';
	tab[3] = file_stat & S_IXUSR ? 'x' : '-';
	tab[4] = file_stat & S_IRGRP ? 'r' : '-';
	tab[5] = file_stat & S_IWGRP ? 'w' : '-';
	tab[6] = file_stat & S_IXGRP ? 'x' : '-';
	tab[7] = file_stat & S_IROTH ? 'r' : '-';
	tab[8] = file_stat & S_IWOTH ? 'w' : '-';
	tab[9] = file_stat & S_IXOTH ? 'x' : '-';
	tab[10] = '\0';
	return (tab);
}

static t_indir	*set_indir(char *name, unsigned char type)
{
	t_indir		*ret;

	ret = (t_indir*)malloc(sizeof(t_indir));
	ret->name = strdup(name);
	ret->type = type;
	ret->next = NULL;
	ret->prev = NULL;
	return (ret);
}

static t_indir	*set_stat_indir(t_indir *lst, char *lst_name)
{
	struct stat	file_stat;
	struct passwd	*uid;
	struct group	*gid;
	char *tmp;

	tmp = ft_strjoin(lst_name, "/");
	tmp = ft_strjoinfree(tmp, lst->name, 1);
	if ((stat(tmp, &file_stat)) == -1)
		return (NULL);
	free(tmp);
	lst->nb_link = file_stat.st_nlink;
	lst->right = set_right(file_stat.st_mode);
	uid = getpwuid(file_stat.st_uid);
	gid = getgrgid(file_stat.st_gid);
	lst->uid_user = strdup(uid->pw_name);
	lst->gid_user = strdup(gid->gr_name);
	lst->size = file_stat.st_size;
	lst->time = ft_strdup(ctime(&(file_stat.st_mtime)));
	return (lst);
}

void	ft_putname(char *name)
{
	int		i;
	int		j;

	i = 0;
	while (name[i])
		i++;
	j = 0;

	while (i - j && name[i - j] != '/')
		j++;
	j--;
	ft_putstr_fd(&name[i - j], 2);
}

void	put_error(int error, t_lst *lst)
{
	t_indir		*indir;

	if (errno == ENOTDIR)
	{
		if (g_fg & LONG_LISTING)
		{
			indir = set_indir(lst->name, '-');
			indir = set_stat_indir(indir, ".");
			put_llist(indir, 1);
		}
		else
			ft_putendl(lst->name);
	}
	else if (error & F_access_fail)
	{
		ft_putstr_fd("ft_ls: ", 2);
		ft_putname(lst->name);
		ft_putstr_fd(": Permission denied\n", 2);
	}
	free(lst->name);
	free(lst);
}

void	deal_file(t_lst *lst)
{
	int				i;
	t_indir			*prev;
	t_indir			*curr;
	int				error;
	DIR				*dir;
	struct dirent	*indir;

	curr = NULL;
	error = 1;
	if (!(dir = opendir((char*)lst->name)))
		return (put_error(1, lst));
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
	if (g_fg & LONG_LISTING)
	{
		curr = lst->indir;
		while (curr)
		{
			if (!(curr = set_stat_indir(curr, lst->name)))
				return (put_error(1, lst));
			curr = curr->next;
		}
	}
	if (g_fg & LONG_LISTING && error)
		(g_fg & REVERSE) ? put_llist(prev, i) : put_llist(lst->indir, i);
	else
		(g_fg & REVERSE) ? put_list(prev, i) : put_list(lst->indir, i);
	if (g_fg & RECURSIVE)
		(g_fg & REVERSE) ? put_dlist(prev, i, lst->name) : put_dlist(lst->indir, i, lst->name);
	if (dir)
		closedir(dir);
	curr = lst->indir;
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
	free(lst->name);
	free(lst);
}

int		main(int ac, char **av)
{
	int		i;
	t_lst	*file;

	i = 1;
	file = NULL;
	while (av[i] && av[i][0] == '-' && ft_strlen(av[i]) >= 2)
		get_flag(av[i++]);
	if (ac - i >= 2)
		while (i < ac)
		{
			ft_putend(av[i], ":\n");
			file = lst_new(av[i]);
			deal_file(file);
			i++;
		}
	else if (ac - i == 1)
	{
		file = lst_new(av[2]);
		deal_file(file);
	}
	else
	{
		file = lst_new(".");
		deal_file(file);

	}
	return (0);
}
