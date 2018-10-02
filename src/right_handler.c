/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   right_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/28 20:28:34 by rfontain          #+#    #+#             */
/*   Updated: 2018/10/02 19:39:24 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

static void	extended_attribut(char *file_name, char *tab)
{
	acl_t		acl;
	acl_entry_t	entry;
	ssize_t		xattr;

	xattr = 0;
	acl = acl_get_link_np(file_name, ACL_TYPE_EXTENDED);
	if (acl && acl_get_entry(acl, ACL_FIRST_ENTRY, &entry) == -2)
	{
		acl_free(acl);
		acl = NULL;
	}
	xattr = listxattr(file_name, NULL, 0, XATTR_NOFOLLOW);
	if (xattr > 0)
		*tab = '@';
	else if (acl != NULL)
		*tab = '+';
	else
		*tab = ' ';
	acl_free(acl);
	*(tab + 1) = '\0';
}

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

char		*set_right(mode_t file_stat, char *file_name)
{
	char *tab;

	if (!(tab = (char*)malloc(sizeof(char) * 12)))
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
	extended_attribut(file_name, &tab[10]);
	return (tab);
}
