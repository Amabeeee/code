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

char *ft_strcpy(char *s1, char *s2)
{
	int i = 0;

	while (s2[i] != '\0')
	{
		s1[i] = s2[i];
		i++;
	}
	s1[i] = s2[i];
	return (s1);
}

char *ft_strjoin (char *s1, char *s2)
{
	int i;
	int j;
	int len1;
	int len2;
	char *res;

	if (!s1 || !s2)
	       return (NULL);	
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	res = malloc ((len1 + len2 + 1) * sizeof(char));
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i] != '\0')
	{
		res[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
	{
		res[i] = s2[j];
		j++;
		i++;
	}
	res[i] = '\0';
	return (res);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t siz)
{
	size_t	i;
	size_t	j;

	if (!src)
		return (0);
	if (!dst || siz == 0)
		return (ft_strlen(src));
	i = 0;
	j = ft_strlen(src);
	while (src[i] != '\0' && i < siz -1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (j);
}

char *concat(char *s1, char *s2)
{
	char *res;
	
	if(!s1 || !s2)
		return(NULL);
	res = ft_strjoin(s1, s2);
	return (res);
}

int find_closing_quote(char *str, char last, int start)
{
	int i;
	
	i = start;
	while (str[i])
	{
		if (str[i] == last)
			return(i);
		i++;
	}
	return (-1);
}

char *extract_between(const char *str, int start, int end)
{
	int len;
	char *res;

	len = end - start;
	res = malloc((len + 1) * sizeof(char));
	if (!res)
		return (NULL);
	ft_strlcpy(res, str + start, len + 1);
	return res;
}

char *join_and_free(char *s1, char *s2)
{
	char *res; 
	
	if(!s1 || !s2)
		return(NULL);
	res = ft_strjoin(s1, s2);
	if(!res)
		return(NULL);
	free(s1);
	return (res);
}

int is_quote(char c)
{
	return (c == 34 || c == 39);
}

int process_quote(char *str, int *i, char **result, int *quote_count)
{
	char quote; 
	int start;
	int end;
	char *temp;
	char *new_result;

	quote = str[*i];
	start = *i + 1;
	end = find_closing_quote(str, quote, start);
	if (end == -1)
		return(free(*result), *result = NULL, 0);
	*quote_count = *quote_count + 2;
	temp = extract_between(str, start, end);
	if (!temp)
		return(free(*result), *result = NULL, 0);
	new_result = join_and_free(*result, temp);
	if (!new_result)
		return(free(temp), free(*result), *result = NULL, 0);
	*result = new_result;
	free(temp);
	*i = end;
	return(1);
}
	
char *copy_if_bad_number_or_no_quote(char *str, char *result, int quote_count)
{
	char *copy;

	if (quote_count % 2 != 0)
		{
			free(result);
			return(NULL);
		}
	if (quote_count == 0)
    {
        free(result);
        copy = malloc(ft_strlen(str) + 1);
        if (!copy)
            return (NULL);
        ft_strcpy(copy, str);
        return (copy);
    }
	return (result);
}

char *extract_all_quotes(char *str)
{
	int i;
	int quote_count;
	char *result;

	i = 0;
	quote_count = 0;
	result = malloc(1);
	if (!result)
		return NULL;
	result[0] = '\0';
	while (str[i])
	{
		if (is_quote(str[i]))
		{
			if (!process_quote(str, &i, &result, &quote_count))
			{
				free(result);
    			return (NULL);
			}
		}
		i++;
	}
	return (copy_if_bad_number_or_no_quote(str, result, quote_count));
}

int check (char *text)
{
	char *res;
	
	res = extract_all_quotes(text);
	if(res)
	{
		free(res);
		return(1);
	}
	else
		return(0);
}

// int main()
// {
//     char *text1 = "\'BONJOUR\"\'\"";     
//     char *text2 = "\'BONJOUR\"\"\"\'\'";   
//     char *text3 = "\"BON\'JOUR\"";      
//     char *text4 = "\"BO\"\"NJ\"\"OUR\""; 
//     char *text5 = "\"\"\"";
//     char *text6 = "BONJOUR";

//     char *res;

//     res = extract_all_quotes(text1);
//     if (res)
//     {
//         printf("%s\n", res);
//         free(res);
//     }
// 	res = extract_all_quotes(text2);
//     if (res)
//     {
//         printf("%s\n", res);
//         free(res);
//     }
// 	printf("%d\n", check(text1));
//     printf("%d\n", check(text2));
//     printf("%d\n", check(text3));
//     printf("%d\n", check(text4));
//     printf("%d\n", check(text5));
// 	 printf("%d\n", check(text6));

//     res = extract_all_quotes(text3);
//     if (res)
//     {
//         printf("%s\n", res);
//         free(res);
//     }

//     res = extract_all_quotes(text4);
//     if (res)
//     {
//         printf("%s\n", res);
//         free(res);
//     }

//     res = extract_all_quotes(text5);
//     if (res)
//     {
//         printf("%s\n", res);
//         free(res);
//     }

//     res = extract_all_quotes(text6);
//     if (res)
//     {
//         printf("%s\n", res);
//         free(res);
//     }

//     return 0;
// }

// int main()
// {
// 	char *text1 = "\'BONJOUR\"\'\"";     
// 	char *text2 = "\"BONJOUR\"\"\"\"";   
// 	char *text3 = "\"BON\'JOUR\"";      
// 	char *text4 = "\"BO\"\"NJ\"\"OUR\""; 
// 	char *text5 = "\"\"\"";
// 	char *text6 = "BONJOUR";

// 	printf("%s\n", extract_all_quotes(text1));
// 	printf("%d\n", check(text2));
// 	printf("%d\n", check(text3));
// 	printf("%d\n", check(text4));
// 	printf("%d\n", check(text5));
// 	printf("%s\n", extract_all_quotes(text3));
// 	printf("%s\n", extract_all_quotes(text4));
// 	printf("%s\n", extract_all_quotes(text5));
// 	printf("%s\n", extract_all_quotes(text6));
// 	return 0;
// }



