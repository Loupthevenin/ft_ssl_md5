#include "../includes/ft_ssl.h"

char	*read_stdin(void)
{
	char	*line;
	char	*result;
	char	*tmp;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	while ((line = get_next_line(STDIN_FILENO)) != NULL)
	{
		tmp = ft_strjoin(result, line);
		free(result);
		result = tmp;
		free(line);
		if (!result)
			return (NULL);
	}
	return (result);
}

void	print_usage(void)
{
	ft_printf("usage: ft_ssl command [flags] [file/string]\n");
	ft_printf("Commands:\n md5\n sha256\n");
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

void	cleanup(t_ssl_ctx *ctx)
{
	if (ctx->commands)
	{
		free(ctx->commands);
		ctx->commands = NULL;
	}
	ctx->command_count = 0;
}
