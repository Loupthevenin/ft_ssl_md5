#include "../includes/ft_ssl.h"

void	print_quoted_data_with_hash(const char *data, const char *digest,
		int is_stdin_with_p_flag)
{
	size_t	len;

	len = ft_strlen(data);
	if (is_stdin_with_p_flag)
		ft_putstr_fd("(", STDOUT_FILENO);
	else
		ft_putstr_fd("MD5 (", STDOUT_FILENO);
	ft_putstr_fd("\"", STDOUT_FILENO);
	if (len > 0 && data[len - 1] == '\n')
		write(STDOUT_FILENO, data, len - 1);
	else
		ft_putstr_fd((char *)data, STDOUT_FILENO);
	ft_putstr_fd("\")", STDOUT_FILENO);
	if (is_stdin_with_p_flag)
		ft_putstr_fd("= ", STDOUT_FILENO);
	else
		ft_putstr_fd(" = ", STDOUT_FILENO);
	ft_putendl_fd((char *)digest, STDOUT_FILENO);
}

void	print_hash(const char *label, const char *hash, t_input *input,
		int file)
{
	if (input->flags.q)
		ft_putendl_fd((char *)hash, STDOUT_FILENO);
	else if (input->flags.r)
	{
		ft_putstr_fd((char *)hash, STDOUT_FILENO);
		ft_putchar_fd(' ', STDOUT_FILENO);
		ft_putendl_fd((char *)label, STDOUT_FILENO);
	}
	else
	{
		if (file)
		{
			ft_putstr_fd("MD5 (", STDOUT_FILENO);
			ft_putstr_fd((char *)label, STDOUT_FILENO);
			ft_putstr_fd(") = ", STDOUT_FILENO);
		}
		else
		{
			ft_putstr_fd((char *)label, STDOUT_FILENO);
			ft_putstr_fd("=", STDOUT_FILENO);
			ft_putchar_fd(' ', STDOUT_FILENO);
		}
		ft_putendl_fd((char *)hash, STDOUT_FILENO);
	}
}

void	print_usage(void)
{
	ft_printf("usage: ft_ssl command [flags] [file/string]\n");
	ft_printf("Commands:\n md5\n sha256\n");
	ft_printf("Flags:\n -p -q -r -s\n");
}

void	print_invalid_cmd(const char *cmd)
{
	ft_putstr_fd("ft_ssl: Error: '", 2);
	ft_putstr_fd((char *)cmd, 2);
	ft_putstr_fd("' is an invalid command.\n", 2);
}

void	print_invalid_option(char option)
{
	ft_putstr_fd("ft_ssl: illegal option '", 2);
	ft_putchar_fd(option, 2);
	ft_putstr_fd("'\n", 2);
}

void	print_file_error(const char *cmd_name, const char *filename)
{
	ft_putstr_fd("ft_ssl: ", STDERR_FILENO);
	if (cmd_name)
	{
		ft_putstr_fd((char *)cmd_name, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putstr_fd((char *)filename, STDERR_FILENO);
	ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
}

void	cleanup(t_ssl_ctx *ctx)
{
	if (ctx->commands)
	{
		free(ctx->commands);
		ctx->commands = NULL;
	}
	ctx->command_count = 0;
}
