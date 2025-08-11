#ifndef FT_SSL_H
# define FT_SSL_H

# include "../libft/ft_printf/ft_printf.h"
# include "../libft/libft.h"
# include "md5.h"
# include <fcntl.h>

# define INIT_DATA_A (unsigned long)0x67452301L
# define INIT_DATA_B (unsigned long)0xefcdab89L
# define INIT_DATA_C (unsigned long)0x98badcfeL
# define INIT_DATA_D (unsigned long)0x10325476L

typedef char			*(*t_hash_func)(const char *input);

typedef struct s_command
{
	const char			*name;
	int					(*func)(int argc, char **argv);
}						t_command;

typedef struct s_ssl_ctx
{
	t_command			*commands;
	size_t				command_count;
}						t_ssl_ctx;

typedef struct s_flags
{
	int					p;
	int					q;
	int					r;
	int					s;
}						t_flags;

typedef struct s_input
{
	char				*cmd_name;
	t_flags				flags;
	char				*str_arg;
	int					arg_i;
}						t_input;

typedef struct s_md5
{
	unsigned int		state[4];
	unsigned long long	bitcount;
	unsigned char		buffer[64];
}						t_md5;

// Main
int						parse_flags(t_input *input, int argc, char **argv);
int						md5_main(int argc, char **argv);
int						sha256_main(int argc, char **argv);
int						process_input(t_input *input, int argc, char **argv,
							t_hash_func hash);

// Utils
void					print_hash(const char *label, const char *hash,
							t_input *input);
void					print_usage(void);
void					print_invalid_cmd(const char *cmd);
void					print_invalid_option(char option);
void	print_file_error(const char *cmd_name,
						const char *filename);
void					cleanup(t_ssl_ctx *ctx);

#endif
