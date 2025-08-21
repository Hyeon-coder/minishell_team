/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljh3900 <ljh3900@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 01:19:28 by ljh3900           #+#    #+#             */
/*   Updated: 2025/06/08 01:23:31 by ljh3900          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strndup(const char *src, size_t n)
{
	size_t	len;
	char	*dup;

	len = 0;
	while (src[len] && len < n)
		len++;
	dup = (char *)malloc(len + 1);
	if (!dup)
		return (NULL);
	while (len--)
		dup[len] = src[len];
	dup[ft_strlen(src) < n ? ft_strlen(src) : n] = '\0';
	return (dup);
}
