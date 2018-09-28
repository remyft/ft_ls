/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   indir_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/08 08:51:28 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/28 20:29:18 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

static void	get_stat_indir(t_indir *lst, struct stat file_stat, char *file_name)
{
	struct passwd	*uid;
	struct group	*gid;

	lst->nb_link = file_stat.st_nlink;
	if (!(lst->right = set_right(file_stat.st_mode, file_name)))
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
		(*lst) = (*lst)->next;
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

static void	double_join(char **targ, char *src1, char *src2, char *src3)
{
	join_char(targ, src1, src2, 0);
	join_char(targ, *targ, src3, 1);
}

t_indir		*set_stat_indir(t_indir **lst, t_indir *begin, t_lst *par,
		char *par_name)
{
	char	*tmp;
	t_stat	stat;

	while ((*lst))
	{
		if ((*lst)->name[0] != '/')
			double_join(&tmp, par_name, "/", (*lst)->name);
		else
			assign_char(&tmp, (*lst)->name);
		if ((lstat(tmp, &stat)) == -1)
			stat_fail(lst, &begin, par);
		else
		{
			(*lst)->itime = stat.st_mtime;
			par->nb_blk += (((*lst)->name[0] != '.' ||
				*(par->g_fg) & ALL_FILE) || (cmp_file((*lst)->name) &&
					*(par->g_fg) & HIDEN_FILE)) ?
				stat.st_blocks : 0;
			get_stat_indir((*lst), stat, tmp);
			(*lst) = (*lst)->next;
		}
		free(tmp);
	}
	return (begin);
}
