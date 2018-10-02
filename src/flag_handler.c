/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 22:52:09 by rfontain          #+#    #+#             */
/*   Updated: 2018/10/02 19:36:29 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ls.h"

static void	deal_unsort(t_fg *e_fg)
{
	*e_fg |= UNSORT;
	if (*e_fg & REVERSE)
		*e_fg &= ~REVERSE;
	if (*e_fg & DATE_SORT)
		*e_fg &= ~DATE_SORT;
	if (*e_fg & SIZE_SORT)
		*e_fg &= ~SIZE_SORT;
}

static void	put_error_flag(char fl, t_fg *e_fg)
{
	if (fl == '1')
	{
		*e_fg &= ~LONG_LISTING;
		return ;
	}
	ft_putstr_fd("ft_ls: illegal option -- ", 2);
	ft_putchar_fd(fl, 2);
	ft_putchar_fd('\n', 2);
	ft_putstr_fd("usage: ./ft_ls [-ARSTaflnort1] [file ...]\n", 2);
	exit(1);
}

static void	set_flags(char fl, t_fg *e_fg)
{
	if (fl == 'R')
		*e_fg |= RECURSIVE;
	else if (fl == 'l' || fl == 'n')
		*e_fg |= LONG_LISTING;
	else if (fl == 'o')
		*e_fg |= LONGO;
	else if (fl == 'a' || fl == 'f')
		*e_fg |= ALL_FILE;
	else if (fl == 'A')
		*e_fg |= HIDEN_FILE;
	else if (fl == 'r' && !(*e_fg & UNSORT))
		*e_fg |= REVERSE;
	else if (fl == 't' && !(*e_fg & UNSORT))
		*e_fg |= DATE_SORT;
	else if (fl == 'T')
		*e_fg |= LONG_T;
	else if (fl == 'S' && !(*e_fg & UNSORT))
		*e_fg |= SIZE_SORT;
	else if (fl != '-' && fl != 'r' && fl != 't' && fl != 'S')
		put_error_flag(fl, e_fg);
	if (fl == 'f')
		deal_unsort(e_fg);
	else if (fl == 'n')
		*e_fg |= GET_ID;
}

void		get_flag(char *flags, t_fg *e_fg)
{
	int i;

	i = 1;
	if (ft_strlen(flags) == 2)
		set_flags(flags[1], e_fg);
	else
	{
		if (ft_strstr(flags + 1, "-"))
			put_error_flag('-', e_fg);
		while (flags[i])
			set_flags(flags[i++], e_fg);
	}
}
