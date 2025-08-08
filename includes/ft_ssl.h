#ifndef FT_SSL_H
# define FT_SSL_H

# include "../libft/ft_printf/ft_printf.h"
# include "../libft/libft.h"

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
	t_flags		flags;
	int			arg_i;
}				t_input;

// Main
void			parse_flags(t_input *input, int argc, char **argv);
int				md5_main(int argc, char **argv);
int				sha256_main(int argc, char **argv);

// Utils
void			print_usage(void);
void			print_invalid_cmd(const char *cmd);
void			cleanup(t_ssl_ctx *ctx);

#endif
