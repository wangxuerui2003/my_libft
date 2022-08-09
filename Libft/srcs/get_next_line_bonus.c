/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wxuerui <wxuerui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 17:53:11 by wxuerui           #+#    #+#             */
/*   Updated: 2022/08/09 21:44:38 by wxuerui          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strcombine(char *s1, char *s2);
static char	**ft_split_line(char *buff);
static int	is_line(char *line);

static char	*ft_strcombine(char *s1, char *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	char	*res;
	size_t	i;

	if (!s2)
		return (s1);
	s1_len = ft_strlen((char *)s1);
	s2_len = ft_strlen((char *)s2);
	res = malloc((s1_len + s2_len + 1) * sizeof(char));
	if (!res)
		return (NULL);
	i = -1;
	while (++i < s1_len)
		res[i] = s1[i];
	i -= 1;
	while (++i < s1_len + s2_len)
		res[i] = s2[i - s1_len];
	res[i] = 0;
	if (s1)
		free(s1);
	return (res);
}

static char	**ft_split_line(char *buff)
{
	int		i;
	int		j;
	char	**res;

	i = 0;
	res = malloc(2 * sizeof(char *));
	while (buff[i] && buff[i] != '\n')
		i++;
	if (!is_line(buff))
		i--;
	res[0] = malloc((i + 2) * sizeof(char));
	j = -1;
	while (++j < i + 1)
		res[0][j] = buff[j];
	res[0][j] = 0;
	res[1] = NULL;
	if ((ft_strlen(buff) > 1) && is_line(buff))
	{
		res[1] = malloc(ft_strlen(buff + i + 1) + 1);
		j--;
		while (buff[++j])
			res[1][j - i - 1] = buff[j];
		res[1][j - i - 1] = 0;
	}
	return (res);
}

/* return 1 if the string contains '\n' else 0 */
static int	is_line(char *line)
{
	int	i;

	i = 0;
	if (!line)
		return (0);
	while (line[i])
	{
		if (line[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

/* read BUFFER by BUFFER and return the final string that contains \n 
 * or encounter EOF.
 */
static char	*read_line(int fd, char *buff)
{
	char	*temp;
	int		ret;

	temp = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!temp)
		return (NULL);
	ret = 1;
	while (!is_line(buff) && ret)
	{
		ret = read(fd, temp, BUFFER_SIZE);
		if (!ret)
			break ;
		if (ret == -1)
		{
			free(temp);
			return (NULL);
		}
		temp[ret] = 0;
		buff = ft_strcombine(buff, temp);
	}
	free(temp);
	return (buff);
}

/* get the new line and update the static variable buff */
char	*get_next_line(int fd)
{
	static char	*buff[1024];
	char		*line;
	char		**splitted;

	if (fd < 0 || fd > 1024 || BUFFER_SIZE <= 0)
		return (NULL);
	buff[fd] = read_line(fd, buff[fd]);
	if (!buff[fd])
		return (NULL);
	if (!buff[fd][0])
	{
		free(buff[fd]);
		buff[fd] = NULL;
		return (NULL);
	}
	splitted = ft_split_line(buff[fd]);
	line = ft_strdup(splitted[0]);
	free(buff[fd]);
	buff[fd] = ft_strdup(splitted[1]);
	free(splitted[0]);
	if (splitted[1])
		free(splitted[1]);
	free(splitted);
	return (line);
}
