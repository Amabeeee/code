#ifndef MINISHELL_H
# define MINISHELL_H

typedef struct s_cmd
{
    struct s_token *in;
    struct s_token *out;
    // struct s_token *arg;
	//struct s_token *file;
	char **argv;
	int argc;
}t_cmd;

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
	t_cmd *cmd;
}t_token;

// 	# define INPUT		1	//"<"
// # define HEREDOC	2	//"<<"
// # define TRUNC		3	//">"
// # define APPEND		4	//">>"
// # define PIPE		5	//"|"
// # define CMD		6	//"|"
// # define ARG

int	ft_strcmp (char *s1, char *s2);
int	ft_valid (t_cmd *s);
int ft_check_env (char *str);
int	ft_command(t_cmd *cmd);
int main();
#endif


