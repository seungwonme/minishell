/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isquote.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seunan <seunan@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 23:05:08 by seunan            #+#    #+#             */
/*   Updated: 2023/09/14 16:31:24 by seunan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int ft_isquote(char c)
{
	return (c == '\'' || c == '\"');
}
