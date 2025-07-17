/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_commands_AVEC_MAIN.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdevezi <amdevezi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 19:44:24 by amdevezi          #+#    #+#             */
/*   Updated: 2025/07/17 14:57:12 by amdevezi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<stdlib.h>
#include <stdio.h>

typedef struct s_cmd
{
	struct	s_token	*in;
	struct	s_token	*out;
	char		**argv;
	int		argc;
}t_cmd;

typedef enum TYPE
{
	IN,
	OUT,
	HEREDOC,
	APPEND,
	PIPE,
	ARG
}TYPE;

typedef struct s_token
{
	struct	s_token	*prev;
	struct	s_token	*next;
	char		*value;
	TYPE		type;
	t_cmd		*cmd;
}t_token;

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
	{
		i++;
	}
	return (s1[i] - s2[i]);
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
	while (s[i] != 0)
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

int is_builtin(char *str)
{
    return ((ft_strcmp(str, "echo") == 0) || (ft_strcmp(str, "cd") == 0) 
    || (ft_strcmp(str, "pwd") == 0) || (ft_strcmp(str, "export") == 0) 
    || (ft_strcmp(str, "unset") == 0) || (ft_strcmp(str, "env") == 0) ||
    (ft_strcmp(str, "exit") == 0));
}

void init_cmd(t_token *token)
{
    if (!token || token->cmd != NULL)
        return;
    token->cmd = malloc(sizeof(t_cmd));
    if (!token->cmd)
        return;
    token->cmd->in = NULL;
    token->cmd->out = NULL;
    token->cmd->argv = NULL;
	token->cmd->argc = 0;
}

t_token *find_command_token(t_token *token)
{
    while (token && token->prev && token->prev->type == ARG)
        token = token->prev;
    return (token);
}

void redirect_command(t_token *list)
{
	t_token *cmd_token;
	t_token *file_token;
	
    while (list)
    {
        if (list->type == IN || list->type == HEREDOC ||
            list->type == OUT || list->type == APPEND)
        {
            file_token = list->next;
            if (!file_token || file_token->type != ARG)
                list = list->next;
			cmd_token = find_command_token(list->prev);
            while (cmd_token && cmd_token->type != ARG)
                cmd_token = cmd_token->prev;
            if (!cmd_token)
                list = list->next;
            init_cmd(cmd_token);
			init_cmd(file_token);
            if (list->type == IN || list->type == HEREDOC)
                cmd_token->cmd->in = file_token;
            else
                cmd_token->cmd->out = file_token;
        }
        list = list->next;
    }
}

int count_args(t_token *token)
{
    int argc;
    t_token *current;

    argc = 1;
    current = token->next;
    while (current)
    {
        if (current->type == IN || current->type == OUT 
            || current->type == HEREDOC || current->type == APPEND 
            || current->type == PIPE)
            break;
        if (current->type == ARG)
            argc++;
        current = current->next;
    }
    return (argc);
}

void fill_argv(t_token *token, int argc)
{
    int i;
    t_token *current;

    i = 1;
    current = token->next;
    token->cmd->argv[0] = ft_strdup(token->value);
    while (current && i < argc)
    {
        if (current->type == ARG)
        {
            token->cmd->argv[i] = ft_strdup(current->value);
            i++;
        }
        current = current->next;
    }
    token->cmd->argv[i] = NULL;
}

void parse_command(t_token *token)
{
    int argc;

    if (!token)
        return;
    if (!token->cmd)
        init_cmd(token);
    argc = count_args(token);
    token->cmd->argv = malloc(sizeof(char *) * (argc + 1));
    if (!token->cmd->argv)
        return;
    fill_argv(token, argc);
    token->cmd->argc = argc;
}

void build_commands(t_token *list)
{
    t_token *current;
    
    current = list;
    while (current)
    {
        init_cmd(current);
        if (current->type == IN || current->type == OUT 
            || current->type == HEREDOC || current->type == APPEND)
        {
            redirect_command(current);
        }
        if (current->type == ARG && is_builtin(current->value))
        {
            parse_command(current);
        }
        current = current->next;
    }
}
//MAIN POUR TESTER: 

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// void free_cmd(t_cmd *cmd)
// {
//     if (!cmd)
//         return;
//     for (int i = 0; cmd->argv && cmd->argv[i]; i++)
//         free(cmd->argv[i]);
//     free(cmd->argv);
//     free(cmd);
// }

// // Création d'un token
// t_token *create_token(char *value, TYPE type)
// {
//     t_token *new = malloc(sizeof(t_token));
//     if (!new)
//         return NULL;
//     new->value = ft_strdup(value);
//     new->type = type;
//     new->cmd = NULL;
//     new->next = NULL;
//     new->prev = NULL;
//     return new;
// }

// // Ajoute un token à la liste
// void append_token(t_token **head, t_token *new)
// {
//     t_token *tmp;

//     if (!*head)
//     {
//         *head = new;
//         return;
//     }
//     tmp = *head;
//     while (tmp->next)
//         tmp = tmp->next;
//     tmp->next = new;
//     new->prev = tmp;
// }

// // Affiche les résultats pour debug
// void print_commands(t_token *list)
// {
//     while (list)
//     {
//         if (list->cmd)
//         {
//             printf("Commande liée à [%s]:\n", list->value);
//             for (int i = 0; list->cmd->argv && list->cmd->argv[i]; i++)
//                 printf("  argv[%d] = %s\n", i, list->cmd->argv[i]);
//             if (list->cmd->in)
//                 printf("  input  = %s\n", list->cmd->in->value);
//             if (list->cmd->out)
//                 printf("  output = %s\n", list->cmd->out->value);
//         }
//         list = list->next;
//     }
// }

// void free_token_list(t_token *list)
// {
//     t_token *tmp;
//     while (list)
//     {
//         tmp = list->next;
//         free(list->value);
//         free_cmd(list->cmd);
//         free(list);
//         list = tmp;
//     }
// }

// int main(void)
// {
//     t_token *list = NULL;

//     // Simule la commande : echo hello world > file.txt
//     append_token(&list, create_token("echo", ARG));
//     append_token(&list, create_token("hello", ARG));
//     append_token(&list, create_token("world", ARG));
//     append_token(&list, create_token(">", OUT));
//     append_token(&list, create_token("file.txt", ARG));

//     build_commands(list);
//     print_commands(list);

//     free_token_list(list);
//     return 0;
// }
