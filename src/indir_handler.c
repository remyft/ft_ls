/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   indir_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/08 08:51:28 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/23 01:30:15 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

static char	set_type(mode_t file_stat)
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

char		*set_right(mode_t file_stat)
{
	char *tab;

	if (!(tab = (char*)malloc(sizeof(char) * 11)))
		return (NULL);
	tab[0] = set_type(file_stat);
	tab[1] = file_stat & S_IRUSR ? 'r' : '-';
	tab[2] = file_stat & S_IWUSR ? 'w' : '-';
	if (file_stat & S_ISUID)
		tab[3] = file_stat & S_IXUSR ? 's' : 'S';
	else
		tab[3] = file_stat & S_IXUSR ? 'x' : '-';
	tab[4] = file_stat & S_IRGRP ? 'r' : '-';
	tab[5] = file_stat & S_IWGRP ? 'w' : '-';
	if (file_stat & S_ISGID)
		tab[6] = file_stat & S_IXGRP ? 's' : 'S';
	else
		tab[6] = file_stat & S_IXGRP ? 'x' : '-';
	tab[7] = file_stat & S_IROTH ? 'r' : '-';
	tab[8] = file_stat & S_IWOTH ? 'w' : '-';
	if (file_stat & S_ISVTX)
		tab[9] = file_stat & S_IXOTH ? 't' : 'T';
	else
		tab[9] = file_stat & S_IXOTH ? 'x' : '-';
	tab[10] = '\0';
	return (tab);
}

static void	get_stat_indir(t_indir *lst, struct stat file_stat)
{
	struct passwd	*uid;
	struct group	*gid;

	lst->nb_link = file_stat.st_nlink;
	if (!(lst->right = set_right(file_stat.st_mode)))
		exit(2);
	if (!(uid = getpwuid(file_stat.st_uid)))
		lst->uid_user = ft_itoa(file_stat.st_uid);
	else
		lst->uid_user = ft_strdup(uid->pw_name);
	if (!(lst->uid_user))
		exit(2);
	if (!(gid = getgrgid(file_stat.st_gid)))
		lst->gid_user = ft_itoa(file_stat.st_gid);
	else
		lst->gid_user = ft_strdup(gid->gr_name);
	if (!(lst->gid_user))
		exit(2);
	lst->size = file_stat.st_size;
	if (S_ISBLK(file_stat.st_mode) || S_ISCHR(file_stat.st_mode))
		double_int(&(lst->major), &(lst->minor),
				major(file_stat.st_rdev), minor(file_stat.st_rdev));
	else
		double_int((&lst->major), &(lst->minor), -1, -1);
	assign_char(&(lst->time), ctime(&(file_stat.st_mtime)));
}

t_indir		*set_indir(char *name, unsigned char type)
{
	t_indir		*ret;

	if (!(ret = (t_indir*)ft_memalloc(sizeof(t_indir))))
		exit(2);
	assign_char(&(ret->name), name);
	ret->type = type;
	return (ret);
}

void		stat_fail(t_indir **lst, t_indir **begin, t_lst *par)
{
	t_indir *l_tmp;

	if ((*lst)->next)
	{
		l_tmp = (*lst)->next->next;
		(*lst) = (*lst)->next;
		(*lst)->next = l_tmp;
		if ((*lst)->prev->prev)
			(*lst)->prev->prev->next = (*lst);
		l_tmp = (*lst)->prev->prev;
		ft_strdel(&((*lst)->prev->name));
		ft_memdel((void**)&((*lst)->prev));
		(*lst)->prev = l_tmp;
	}
	else
	{
		if ((*lst)->prev)
			(*lst)->prev->next = NULL;
		else
			(*begin) = NULL;
		ft_strdel(&((*lst)->name));
		ft_memdel((void**)lst);
	}
	par->size -= 1;
}

t_indir		*set_stat_indir(t_indir **lst, t_indir *begin, t_lst *par, char *par_name)
{
	char *tmp;
	t_stat	stat;

	while ((*lst))
	{
		if ((*lst)->name[0] != '/')
		{
			join_char(&tmp, par_name, "/", 0);
			join_char(&tmp, tmp, (*lst)->name, 1);
		}
		else
			assign_char(&tmp, (*lst)->name);
		if ((lstat(tmp, &stat)) == -1)
			stat_fail(lst, &begin, par);
		else
		{
			(*lst)->itime = stat.st_mtime;
			par->nb_blk += ((*lst)->name[0] != '.' || *(par->g_fg) & ALL_FILE) ?
				stat.st_blocks : 0;
			get_stat_indir((*lst), stat);
			(*lst) = (*lst)->next;
		}
		free(tmp);
	}
	return (begin);
}
