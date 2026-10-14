#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSGSZ 128

typedef struct msgbuf {
    long mtype;
    char mtext[MSGSZ];
} message_buf;

int main() {
    int msqid;
    key_t key = 1234;
    message_buf sbuf, rbuf;
    size_t buf_length;

    if ((msqid = msgget(key, 0666)) < 0) {
        perror("msgget"); exit(1);
    }

    sbuf.mtype = 1; 
    strcpy(sbuf.mtext, "Soy el Cliente 1");
    buf_length = strlen(sbuf.mtext) + 1;
    msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT);
    printf("C1 envio: %s\n", sbuf.mtext);

    if (msgrcv(msqid, &rbuf, MSGSZ, 10, 0) < 0) {
        perror("msgrcv"); exit(1);
    }
    printf("C1 recibio: %s\n", rbuf.mtext);

    return 0;
}
