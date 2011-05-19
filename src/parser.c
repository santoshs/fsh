#include <string.h>

#include "include/shell.h"

int parse_line (command_t *cmd)
{
	char *line = cmd->line;
	char word[MAX_WORD_LENGTH];
	int i = 0, j = 0;

	if (!line)
		return -1;

	if (*line == '\0' || *line == '\n')
		return -1;

	while (*line) {
		if (*line == ' ') {
			word[i] = '\0';
			cmd->args[j++] = strdup(word);
		} else {
			word[i] = *line;
		}
		line++;
	}

	return 0;
}
