/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seunan <seunan@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 14:45:37 by seunan            #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/08/28 21:38:23 by seunan           ###   ########.fr       */
=======
/*   Updated: 2023/08/27 23:07:03 by seunan           ###   ########.fr       */
>>>>>>> 5410290b7d2ddc3ca8b0114d21a4e8cb63c408f1
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (lst == NULL)
		return ;
<<<<<<< HEAD
	del(lst->token);
=======
	del(lst->cmd);
>>>>>>> 5410290b7d2ddc3ca8b0114d21a4e8cb63c408f1
	free(lst);
}
