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
    int msqid_envio, msqid_resp;
    key_t key_envio = 1234;
    key_t key_resp = 1235;
    message_buf rbuf, sbuf;
    size_t buf_length;

    if ((msqid_envio = msgget(key_envio, 0666)) < 0) {
        perror("msgget envio"); exit(1);
    }
    if ((msqid_resp = msgget(key_resp, 0666)) < 0) {
        perror("msgget resp"); exit(1);
    }

    if (msgrcv(msqid_envio, &rbuf, MSGSZ, 1, 0) < 0) {
        perror("msgrcv 1"); exit(1);
    }
    printf("P2 recibio: %s\n", rbuf.mtext);

    sbuf.mtype = 1;
    strcpy(sbuf.mtext, "Loud and clear");
    buf_length = strlen(sbuf.mtext) + 1;
    msgsnd(msqid_resp, &sbuf, buf_length, IPC_NOWAIT);
    printf("P2 envio: %s\n", sbuf.mtext);

    if (msgrcv(msqid_envio, &rbuf, MSGSZ, 1, 0) < 0) {
        perror("msgrcv 2"); exit(1);
    }
    printf("P2 recibio: %s\n", rbuf.mtext);

    return 0;
}
