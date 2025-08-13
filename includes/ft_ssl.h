#ifndef FT_SSL_H
# define FT_SSL_H

# include "../libft/ft_printf/ft_printf.h"
# include "../libft/libft.h"
# include <fcntl.h>

typedef char	*(*t_hash_func)(const char *input);

typedef struct s_command
{
	const char	*name;
	int			(*func)(int argc, char **argv);
}				t_command;

typedef struct s_ssl_ctx
{
	t_command	*commands;
	size_t		command_count;
}				t_ssl_ctx;

typedef struct s_flags
{
	int			p;
	int			q;
	int			r;
	int			s;
}				t_flags;

typedef struct s_input
{
	char		*cmd_name;
	t_flags		flags;
	char		*str_arg;
	int			arg_i;
}				t_input;

// Main
int				parse_flags(t_input *input, int argc, char **argv);
int				md5_main(int argc, char **argv);
int				sha256_main(int argc, char **argv);
int				process_input(t_input *input, int argc, char **argv,
					t_hash_func hash);

// Utils
void	print_quoted_data_with_hash(const char *data,
									const char *digest,
									int is_stdin_with_p_flag);
void			print_hash(const char *label, const char *hash, t_input *input,
					int file);
void			print_usage(void);
void			print_invalid_cmd(const char *cmd);
void			print_invalid_option(char option);
void	print_file_error(const char *cmd_name,
						const char *filename);
void			cleanup(t_ssl_ctx *ctx);

#endif
