/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 23:14:40 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/16 19:35:40 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

int		cmp_file(char *name)
{
	if (ft_strcmp(name, ".") != 0 && ft_strcmp(name, "..") != 0)
		return (1);
	return (0);
}

char	*get_time(char *file, time_t itime)
{
	time_t	now;
	char	*ret;
	char	*tmp;
	int		i;

	ret = (char*)malloc(sizeof(char) * 13);
	now = time(&now);
	tmp = file;
	tmp[10] = '\0';
	ret = ft_strcpy(ret, &tmp[4]);
	ret[6] = ' ';
	i = 6;
	if (now - itime < 15778800 && now - itime >= 0)
		while (++i < 12)
			ret[i] = tmp[i + 4];
	else
	{
		while (++i < 11)
			ret[i] = tmp[i + 13];
		ret[i++] = ' ';
	}
	ret[i] = '\0';
	return (ret);
}

int		max_int(int nb1, int nb2)
{
	return ((nb1 > nb2) ? nb1 : nb2);
}

void	max_size(t_indir *lst, int *uid_size, int *gid_size)
{
	while (lst->prev)
		lst = lst->prev;
	while (lst)
	{
		(*uid_size) = max_int(ft_strlen(lst->uid_user), (*uid_size));
		(*gid_size) = max_int(ft_strlen(lst->gid_user), (*gid_size));
		lst = lst->next;
	}
}

char	*nb_space(char *str, int nb, int size)
{
	int		stsize;
	int		i;
	char	*ret;

	i = 0;
	stsize = 0;
	if (str)
		stsize = ft_strlen(str);
	else
		stsize = nb;
	ret = malloc(sizeof(char) * (3 + size - stsize));
	while (i < size - stsize + 2)
		ret[i++] = ' ';
	ret[i] = '\0';
	return (ret);
}

int		is_major(t_indir *lst)
{
	int ret;

	ret = 0;
	while (lst)
	{
		if (lst->size >= 1000 && lst->major < 0 && cmp_file(lst->name))
			return (0);
		if (lst->minor >= 0)
			ret = 1;
		lst = lst->next;
	}
	return (ret);
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

void	sort_alpha(t_indir *names, int size)
{
	int		i;
	int		continuer;
	t_indir	*curr;

	continuer = 0;
	while (!continuer)
	{
		continuer = 1;
		i = 0;
		curr = names;
		while (i < size)
		{
			if (i + 1 != size && ft_strcmp(curr->name, curr->next->name) > 0)
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

void	sort_date(t_indir *names, int size)
{
	int		i;
	int		continuer;
	t_indir	*curr;

	continuer = 0;
	while (!continuer)
	{
		continuer = 1;
		i = 0;
		curr = names;
		while (i < size)
		{
			if (i + 1 != size && curr->itime < curr->next->itime)
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
			free(lst->name);
			free(lst);
		}
		else
		{
			ft_putendl(lst->name);
			free(lst->name);
			free(lst);
		}
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
			{
				free(indir->name);
				free(indir->right);
				free(indir->uid_user);
				free(indir->gid_user);
				free(indir->time);
				free(indir);
			}
			free(lst->name);
			free(lst);
		}
		else
		{
			ft_putendl(lst->name);
			free(lst->name);
			free(lst);
		}
	}
	else if (error & F_ACCESS_FAIL && errno == EACCES)
	{
		ft_putstr_fd("ft_ls: ", 2);
		ft_putname(lst->name);
		ft_putstr_fd(": Permission denied\n", 2);
		free(lst->name);
		free(lst);
	}
	else
		put_ferror(error, lst);
}
