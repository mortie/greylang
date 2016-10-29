#include <stdio.h>

#ifdef _WIN32
#else
	#include <readline/readline.h>
	#include <readline/history.h>
#endif

char* l_plat_readline(char* prompt)
{
#ifdef _WIN32
	printf("%s", prompt);
	int allocd = 16;
	int len = 1;
	char* str = malloc(allocd);
	while (1)
	{
		char c = getc();
		if (c == '\n')
			return str;
		else if (c == '\r')
			continue;

		len += 1;
		if (allocd < len)
		{
			allocd *= 2;
			str = realloc(str, allocd);
		}
		str[len - 2] = getc();
		str[len - 1] = '\0';
	}
#else
	char* str = readline(prompt);
	if (str != NULL && str[0] != '\0')
		add_history(str);
	return str;
#endif
}
