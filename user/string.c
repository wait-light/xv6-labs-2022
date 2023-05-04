#include "user/string.h"
#include "kernel/types.h"
#include "user/user.h"
int readLine(int fd, char *buffer, int maxSize)
{
    int index = 0;
    while (index < maxSize)
    {
        if (read(fd, buffer + index, 1) == 0)
        {
            buffer[index] = '\0';
            return index;
        }
        if (buffer[index] == '\n')
        {
            buffer[index] = '\0';
            return index;
        }
        index++;
    }
    buffer[index] = '\0';
    return index;
}

int slipLine(char *line, char **words)
{
    int wordCount = 0;
    int index = 0, wordIndex = 0;
    while (line[index] != '\0')
    {
        while (line[index] != ' ' && line[index] != '\n' && line[index] != '\0')
        {
            words[wordCount][wordIndex++] = line[index++];
        }
        words[wordCount][wordIndex] = '\0';
        if (line[index] == '\0')
        {
            break;
        }
        wordCount++;
        index++;
        wordIndex = 0;
    }
    return wordCount + 1;
}