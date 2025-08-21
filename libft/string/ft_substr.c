/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <juhyeonl@student.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 15:59:45 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/02/28 15:51:43 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/*
	Extracts a substring from a given string. (주어진 문자열에서 부분 문자열 추출)
*/
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*rst;

	if (!s)
		return (0);
	if (ft_strlen(s) < start)
		len = 0;
	if (ft_strlen(s + start) < len)
		len = ft_strlen(s + start);
	rst = malloc(sizeof(char) * (len + 1));
	if (!rst)
		return (0);
	ft_strlcpy(rst, s + start, len + 1);
	return (rst);
}
