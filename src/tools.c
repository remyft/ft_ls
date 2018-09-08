/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 23:14:40 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/08 09:17:12 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

int		cmp_file(char *name)
{
	if (ft_strcmp(name, ".") != 0 && ft_strcmp(name, "..") != 0)
		return (1);
	return (0);
}

int		get_mon(char *month)
{
	char	*tab[12];
	int		i;
	char	*tmp;

	tmp = month;
	tmp[3] = '\0';
	i = 0;
	tab[0] = "Jan";
	tab[1] = "Feb";
	tab[2] = "Mar";
	tab[3] = "Apr";
	tab[4] = "May";
	tab[5] = "Jun";
	tab[6] = "Jul";
	tab[7] = "Aug";
	tab[8] = "Sep";
	tab[9] = "Oct";
	tab[10] = "Nov";
	tab[11] = "Dec";
	while (i <= 11 && strcmp(tmp, tab[i]) != 0)
		i++;
	return (i + 1);
}

char	*get_time(char *file)
{
	time_t	now;
	char	*ch_now;
	char	*ret;
	char	*tmp;
	int		i;


	ret = (char*)malloc(sizeof(char) * 13);
	now = time(&now);
	ch_now = ctime(&now);
	tmp = file;
	tmp[10] = '\0';
	ret = strcpy(ret, &tmp[4]);
	ret[6] = ' ';
	i = 6;
	if (get_mon(&file[4]) > 6 && get_mon(&file[4]) - get_mon(&ch_now[4]) <= 6)
		while (++i < 12)
			ret[i] = tmp[i + 4];
	else
		while(++i < 11)
			ret[i] = tmp[i + 13];
	ret[i] = '\0';
	return (ret);
}

int		max_int(int nb1, int nb2)
{
	return ((nb1 >= nb2) ? nb1 : nb2);
}

void	max_size(t_indir *lst, int *uid_size, int *gid_size)
{
	while (lst)
	{
		(*uid_size) = max_int(ft_strlen(lst->uid_user), (*uid_size));
		(*gid_size) = max_int(ft_strlen(lst->gid_user), (*gid_size));
		lst = lst->next;
	}
}

char	*nb_space(char *str, int size)
{
	int		stsize;
	int		i;
	char	*ret;

	i = 0;
	stsize = ft_strlen(str);
	ret = malloc(sizeof(char) * (3 + size - stsize));
	while (i < 2 + size - stsize + 2)
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
	ft_putstr_fd(&name[i - j - 1], 2);
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
			free_list(indir);
		}
		else
			ft_putendl(lst->name);
	}
	else if (error & F_access_fail && errno == EACCES)
	{
		ft_putstr_fd("ft_ls: ", 2);
		ft_putname(lst->name);
		ft_putstr_fd(": Permission denied\n", 2);
	}
	else
	{
		ft_putstr_fd("ft_ls: ", 2);
		ft_putname(lst->name);
		ft_putstr_fd(": No such file or directory\n", 2);
	}
	free(lst->name);
	free(lst);
}
