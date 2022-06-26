/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynakashi <ynakashi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 17:52:38 by ynakashi          #+#    #+#             */
/*   Updated: 2022/06/26 10:07:11 by ynakashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*dst;
	size_t	s1_len;
	size_t	s2_len;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	dst = malloc((sizeof(char) * (s1_len + s2_len + 1)));
	if (!dst)
		return (NULL);
	ft_strlcpy(dst, s1, s1_len + 1);
	ft_strlcat(dst, s2, s1_len + s2_len + 1);
	return (dst);
}
// note:The third argument of strlcat represents the total size of dst and src.
// "If it's a concatenation of s1 and "" (empty string),
// I guess I can just return s1,
// but I can't define "concatenation of s1 and NULL" properly.
/*
#include <libc.h>
int main(void)
{
	char *str = ft_strjoin("where is my ", "malloc ???");
	printf("where is my malloc ??? = %s\n", str);
	str = ft_strjoin("", "42");
	printf("42 = %s\n", str);
	str = ft_strjoin("42", "");
	printf("42 = %s\n", str);
	str = ft_strjoin("", "");
	printf(" "" = %s\n", str);
	str = ft_strjoin(NULL, ""); // It depends on the person. but not segfo
	printf("(null) = %s\n", str);
	str = ft_strjoin("hello", NULL); // It depends on the person. but not segfo
	printf("(null) = %s\n", str);
	str = ft_strjoin(NULL, NULL); // It depends on the person. but not segfo
	printf("(null) = %s\n", str);

// !!!!DO NOT USE!!!!
	// char	*str1;
	// char	*str2;
	// str1 = calloc(INT_MAX, sizeof(char));
	// str2 = calloc(INT_MAX, sizeof(char));
	// size_t	i = 0;
	// while (i < INT_MAX)
	// 	str1[i++] = 'A';
	// i = 0;
	// while (i < INT_MAX)
	// 	str2[i++] = 'B';
	// printf("%p\n", ft_strjoin(str1, str2));
	// free(str1);
	// free(str2);
	return 0;
}
 */
