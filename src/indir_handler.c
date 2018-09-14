/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   indir_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/08 08:51:28 by rfontain          #+#    #+#             */
/*   Updated: 2018/09/14 06:13:21 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

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

t_indir	*set_indir(char *name, unsigned char type, char *lst_name)
{
	t_indir		*ret;
	char		*tmp;
	struct stat	file_stat;

	ret = (t_indir*)ft_memalloc(sizeof(t_indir));
	ret->name = strdup(name);
	ret->type = type;
	if (g_fg & DATE_SORT || g_fg & LONG_LISTING)
	{
		if (name[0] != '/')
		{
			tmp = ft_strjoin(lst_name, "/");
			tmp = ft_strjoinfree(tmp, name, 1);
		}
		else
			tmp = name;
		if ((lstat(tmp, &file_stat)) == -1)
			return (NULL);
		free(tmp);
		ret->itime = file_stat.st_mtime;
	}
	else
		ret->itime = 0;
	return (ret);
}

void	get_stat_indir(t_indir *lst, struct stat file_stat)
{
	struct passwd	*uid;
	struct group	*gid;

	lst->nb_link = file_stat.st_nlink;
	lst->right = set_right(file_stat.st_mode);
	if (!(uid = getpwuid(file_stat.st_uid)))
		lst->uid_user = "root";
	else
		lst->uid_user = strdup(uid->pw_name);
	if (!(gid = getgrgid(file_stat.st_gid)))
		lst->gid_user = "wheel";
	else
		lst->gid_user = strdup(gid->gr_name);
	lst->size = file_stat.st_size;
	if (S_ISBLK(file_stat.st_mode) || S_ISCHR(file_stat.st_mode))
	{
		lst->major = major(file_stat.st_rdev);
		lst->minor = minor(file_stat.st_rdev);
	}
	else
	{
		lst->major = -1;
		lst->minor = -1;
	}
	lst->time = ft_strdup(ctime(&(file_stat.st_mtime)));
}

t_indir	*set_stat_indir(t_indir *lst, char *lst_name, t_lst *par)
{
	struct stat		file_stat;
	char			*tmp;

	if (lst->name[0] != '/')
	{
		tmp = ft_strjoin(lst_name, "/");
		tmp = ft_strjoinfree(tmp, lst->name, 1);
		if ((lstat(tmp, &file_stat)) == -1)
			return (NULL);
		free(tmp);
	}
	else
	{
		if ((lstat(lst->name, &file_stat)) == -1)
			return (NULL);
	}
	if (lst->name[0] != '.' || g_fg & ALL_FILE)
		par->nb_blk += file_stat.st_blocks;
	get_stat_indir(lst, file_stat);
	return (lst);
}
