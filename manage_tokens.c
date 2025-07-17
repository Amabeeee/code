/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdevezi <amdevezi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 20:09:16 by amdevezi          #+#    #+#             */
/*   Updated: 2025/07/16 19:30:18 by amdevezi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

size_t ft_strlen(const char *str)
{
	unsigned int counter;

	if (!str)
		return (0);
	counter = 0;
	while (str[counter] != '\0')
	{
		counter++;
	}
	return (counter);
}

size_t ft_strlcpy(char *dst, const char *src, size_t siz)
{
	size_t i;

	if (!src)
		return (0);
	if (siz == 0)
		return (ft_strlen(src));
	i = 0;
	while (src[i] != '\0' && i < siz - 1)
	{
		dst[i] = src[i];
		i++;
	}
	if (siz > 0)
		dst[i] = '\0';
	return (ft_strlen(src));
}

char *ft_strndup(const char *s, int n)
{
	char *dest;
	int i;
	size_t size;

	size = 0;
	while (s[size] != 0)
	{
		size++;
	}
	dest = malloc(n + 1);
	if (!dest)
		return (NULL);
	i = 0;
	while (i < n && s[i] != '\0')
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char *ft_remove_spaces(const char *s)
{
	int start;
	int end;
	int len;
	char *res;

	start = 0;
	end = ft_strlen(s) - 1;
	while (s[start] == ' ')
		start++;
	while (end >= start && s[end] == ' ')
		end--;
	len = end - start + 1;
	res = malloc(len + 1);
	if (!res)
		return (NULL);
	ft_strlcpy(res, s + start, len + 1);
	return (res);
}

static void add_token(char **tokens, const char *s, int start, int i, int *j)
{
	if (i > start)
	{
		tokens[*j] = ft_strndup(s + start, i - start);
		(*j)++;
	}
}

static int handle_double_operator(char **tokens, const char *s, int i, int *j)
{
	if ((s[i] == '<' || s[i] == '>') && s[i + 1] == s[i])
	{
		tokens[*j] = ft_strndup(s + i, 2);
		(*j)++;
		return (2);
	}
	return (0);
}

static int handle_single_operator(char **tokens, const char *s, int i, int *j)
{
	if (s[i] != ' ')
	{
		tokens[*j] = ft_strndup(s + i, 1);
		(*j)++;
	}
	return (0);
}

static int is_separator(char c)
{
	return (c == '<' || c == '>' || c == '|' || c == '$' || c == ' ');
}

int is_quote(char c)
{
	return (c == '\'' || c == '"');
}

int skip_quotes(const char *s, int i)
{
	char quote;

	quote = s[i];
	i++;
	while (s[i] && s[i] != quote)
		i++;
	if (s[i] == quote)
		i++; 
	return (i);
}

static void token_limit(char **tokens, const char *s, int *i, int *start, int *j)
{
	add_token(tokens, s, *start, *i, j);
	if (handle_double_operator(tokens, s, *i, j))
		*i = *i + 2;
	else
	{
		handle_single_operator(tokens, s, *i, j);
		(*i)++;
	}
	*start = *i;
}

static void last_token(char **tokens, const char *s, int i, int start, int *j)
{
	char *raw;

	if (i > start)
	{
		raw = ft_strndup(s + start, i - start);
		tokens[*j] = ft_remove_spaces(raw);
		(*j)++;
		free(raw);
	}
	tokens[*j] = (NULL);
}

static int	skip_spaces(const char *str, int i)
{
	while (str[i] == ' ')
		i++;
	return (i);
}

static int	handle_quotes(const char *str, int *i)
{
	if (is_quote(str[*i]))
	{
		*i = skip_quotes(str, *i);
		return (1);
	}
	return (0);
}

static int	handle_operator_or_word(const char *str, int *i)
{
	if ((str[*i] == '<' || str[*i] == '>') && str[*i + 1] == str[*i])
	{
		*i = *i + 2;
		return (1);
	}
	else if (is_separator(str[*i]))
	{
		(*i)++;
		return (1);
	}
	else
	{
		while (str[*i] && !is_separator(str[*i]) && !is_quote(str[*i]))
			(*i)++;
		return (1);
	}
}

static int	count_tokens(const char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		i = skip_spaces(str, i);
		if (!str[i])
			break;
		if (handle_quotes(str, &i))
			count++;
		else
			count = count + handle_operator_or_word(str, &i);
	}
	return (count);
}

static char	**init_tokens(const char *s, int *number)
{
	char **tokens;

	if (!s)
		return (NULL);
	*number = count_tokens(s);
	tokens = malloc(sizeof(char *) * (*number + 1));
	return (tokens);
}

static void	parse_char(const char *s, char **tokens, int *i, int *start, int *j)
{
	char quote;
	
	if (s[*i] == ' ')
	{
		add_token(tokens, s, *start, *i, j);
		while (s[*i] == ' ')
			(*i)++;
		*start = *i;
	}
	else if (is_quote(s[*i]))
	{
		quote = s[*i];
		(*i)++;
		while (s[*i] && s[*i] != quote)
			(*i)++;
		if (s[*i] == quote)
			(*i)++;
	}
	else if (is_separator(s[*i]))
		token_limit(tokens, s, i, start, j);
	else
		(*i)++;
}

char	**tokenize(const char *s)
{
	char	**tokens;
	int		i = 0, j = 0, start = 0, number;

	tokens = init_tokens(s, &number);
	if (!tokens)
		return (NULL);
	while (s[i])
		parse_char(s, tokens, &i, &start, &j);
	last_token(tokens, s, i, start, &j);
	tokens[j] = NULL;
	return (tokens);
}

// int main()
// {
// 	//char **result = tokenize("<<logs.txt cat|grep 'error' |wc > '$' error.txt -l");
// 	//char **result = tokenize("export HOLA='bonjour$HOME'");
// 	char **result = tokenize("export HOLA='bonjour   '/");
// 	//char **result = tokenize("echo 'Bonjour=HOME'");
// 	if (!result)
// 	{
// 		fprintf(stderr, "Erreur : re_tokenize a échoué.\n");
// 		return 1;
// 	}

// 	for (int i = 0; result[i]; i++)
// 	{
// 		printf("Token: [%s]\n", result[i]);
// 		free(result[i]);
// 	}
// 	free(result);
// 	return (0);
// }

