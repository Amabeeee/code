/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_token_type.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdevezi <amdevezi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 14:57:56 by amdevezi          #+#    #+#             */
/*   Updated: 2025/07/16 19:30:08 by amdevezi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include<string.h> //pour debugger
#include<stdlib.h>

typedef enum TYPE
{
	IN,
	OUT,
	HEREDOC,
	APPEND,
	PIPE,
	ARG
} TYPE;


typedef struct s_token
{
	struct s_token *prev;
	struct s_token *next;
	char *value;
    TYPE type;
}t_token;

int	ft_strcmp (char *s1, char *s2)
{
	int i = 0;

	while (s1[i] == s2[i] && s1[i]!= '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}

t_token *token_list(char **tokens)
{
    t_token *first = NULL;
    t_token *prev = NULL;
    t_token *new_node;
    int i;

    i = 0;
    while(tokens[i])
    {
    new_node = malloc(sizeof(t_token));
        if (!new_node)
            return (NULL);
        new_node->value = strdup(tokens[i]);
        if(!new_node)
            return(NULL);
        new_node->type = ARG;
        new_node->prev = prev;
        new_node->next = NULL;
        if (prev)
            prev->next = new_node;
        else
            first = new_node;
        prev = new_node;
	i++;
    }
    return (first);
}

void init_type(t_token *token)
{        
    if(ft_strcmp(token->value, ">>") == 0)
        token->type = APPEND;
    else if(ft_strcmp(token->value, "<<") == 0)
        token->type = HEREDOC;
    else if(ft_strcmp(token->value, "<") == 0)
        token->type = IN;
    else if(ft_strcmp(token->value, ">") == 0)
        token->type = OUT;
    else if(ft_strcmp(token->value, "|") == 0)
        token->type = PIPE;
    else
        token->type = ARG;
}

void free_token_list(t_token *first)
{
    t_token *tmp;

    while (first)
    {
        tmp = first;
        first = first->next;
        free(tmp->value);
        free(tmp);
    }
}

void type_foreach (t_token *list)
{
    while (list)
    {
        init_type(list);
        list = list -> next; 
    }
}

const char *type_to_str(TYPE type)
{
    if (type == IN)
        return "IN";
    else if (type == OUT)
        return "OUT";
    else if (type == HEREDOC)
        return "HEREDOC";
    else if (type == APPEND)
        return "APPEND";
    else if (type == PIPE)
        return "PIPE";
    else if (type == ARG)
        return "ARG";
    else
        return "UNKNOWN";
}

// int main(void)
// {
//     // Simuler un découpage d'entrée utilisateur
//     char *tokens[] = {
//         "cat", "<<", "eof", "|", "grep", "foo", ">", "file.txt", NULL
//     };
//     t_token *list = token_list(tokens);
//     if (!list)
//     {
//         fprintf(stderr, "Erreur: échec de la création de la liste\n");
//         return (1);
//     }
//     t_token *tmp = list;
//     type_foreach(list);
//     printf("Liste des tokens:\n");
//     tmp = list;
//     if (tmp->value)
//     printf("Valeur: %-10s | Type: %s\n", tmp->value, type_to_str(tmp->type));
//     else
//     printf("Valeur: (null)     | Type: %s\n", type_to_str(tmp->type));
//     while (tmp)
//     {
//         printf("Valeur: %-10s | Type: %s\n", tmp->value, type_to_str(tmp->type));
//         tmp = tmp->next;
//     }
//     free_token_list(list);
//     return 0;
// }