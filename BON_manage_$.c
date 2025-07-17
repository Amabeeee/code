/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BON_manage_$.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdevezi <amdevezi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 13:49:23 by amdevezi          #+#    #+#             */
/*   Updated: 2025/07/17 14:57:04 by amdevezi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	i++;
	return (i);
}

int	ft_isalnum(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	else if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

char	*ft_strdup(const char *s)
{
	char	*dest;
	int		i;
	size_t	size;

	size = 0;
	while (s[size] != 0)
	{
		size++;
	}
	i = 0;
	dest = malloc(size + 1);
	if (!dest)
		return (NULL);
	while (s[i] != 0)
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

// int find_dollar(const char *str) 
// {
//     int i;

// 	i = 0;
//     while (str[i] != '\0')
// 	{
// 		if (str[i] == '$')
// 			return (i);
//         i++;
// 	}
//     return (-1);
// }

static void	copy_replace(char *res, char *replace, int *k)
{
	int	j;

	j = 0;
	while (replace[j])
	{
		res[*k] = replace[j];
		(*k)++;
		j++;
	}
}

static void	copy_after_var(char *res, char *str, int *k, int i)
{
	while (str[i])
	{
		res[*k] = str[i];
		(*k)++;
		i++;
	}
}

char	*result(char *str, char *replace, int var_start, int var_end)
{
	int		k;
	int		i;
	int		len;
	char	*res;

	k = 0;
	i = 0;
	len = ft_strlen(str) - (var_end - var_start) + ft_strlen(replace);
	res = malloc((len + 1) * sizeof(char));
	if (!res)
		return (0);
	while (i < var_start)
		res[k++] = str[i++];
	copy_replace(res, replace, &k);
	i = var_end;
	copy_after_var(res, str, &k, i);
	res[k] = '\0';
	return (res);
}

static int	is_special_var(char c)
{
	return (c == '?' || c == '$' || c == '0');
}

static char	*expand_special(char *str, int var_start, char c)
{
	if (c == '?')
		return (result(str, "<error>", var_start, var_start + 2));
	else if (c == '$')
		return (result(str, "<error>", var_start, var_start + 2));
	else if (c == '0')
		return (result(str, "<pwd>", var_start, var_start + 2));
	return (ft_strdup(str));
}

static char	*expand_normal(char *str, char *replace, int var_start)
{
	int	var_end;

	var_end = var_start + 1;
	while (str[var_end] && (ft_isalnum(str[var_end]) || str[var_end] == '_'))
		var_end++;
	return (result(str, replace, var_start, var_end));
}

char	*expand(char *str, char *replace)
{
	int	i;
	int	var_start;
	int	in_single_quote;
	int	in_double_quote;

	i = 0;
	var_start = -1;
	in_single_quote = 0;
	in_double_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[i] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (str[i] == '$' && !in_single_quote)
		{
			var_start = i;
			if (is_special_var(str[i + 1]))
				return (expand_special(str, var_start, str[i + 1]));
			return (expand_normal(str, replace, var_start));
		}
		i++;
	}
	return (ft_strdup(str));
}

// int main()
// {
//     //char *res = expand("Ceci est un '$test' on va $voir", "bonjour");
// 	char *res = expand("ceci est $?", "ignored");
//     printf("%s\n", res);
//     free(res);
//     return 0;
// }
