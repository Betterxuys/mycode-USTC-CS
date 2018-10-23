#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() {
    /* ����������� */
    char cmd[256];
    /* �����в��ɵĸ����֣��Կ�ָ���β */
    char *args[128];
    while (1) {
        /* ��ʾ�� */
        printf("# ");
        fflush(stdin);
        fgets(cmd, 256, stdin);
        /* �����β�Ļ��з� */
        int i;
        for (i = 0; cmd[i] != '\n'; i++)
            ;
        cmd[i] = '\0';
        /* ��������� */
        args[0] = cmd;
        for (i = 0; *args[i]; i++)
            for (args[i+1] = args[i] + 1; *args[i+1]; args[i+1]++)
                if (*args[i+1] == ' ') {
                    *args[i+1] = '\0';
                    args[i+1]++;
                    break;
                }
        args[i] = NULL;

        /* û���������� */
        if (!args[0])
            continue;

        /* �ڽ����� */
        if (strcmp(args[0], "cd") == 0) {
            if (args[1])
                chdir(args[1]);
            continue;
        }
        if (strcmp(args[0], "pwd") == 0) {
            char wd[4096];
            puts(getcwd(wd, 4096));
            continue;
        }
        if (strcmp(args[0], "exit") == 0)
            return 0;

        /* �ⲿ���� */
        pid_t pid = fork();
        if (pid == 0) {
            /* �ӽ��� */
            execvp(args[0], args);
            /* execvpʧ�� */
            return 255;
        }
        /* ������ */
        wait(NULL);
    }
}