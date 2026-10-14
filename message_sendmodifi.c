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
    message_buf sbuf, rbuf;
    size_t buf_length;

    if ((msqid_envio = msgget(key_envio, IPC_CREAT | 0666)) < 0) {
        perror("msgget envio"); exit(1);
    }
    if ((msqid_resp = msgget(key_resp, IPC_CREAT | 0666)) < 0) {
        perror("msgget resp"); exit(1);
    }

    sbuf.mtype = 1;
    strcpy(sbuf.mtext, "Are you hearing me?");
    buf_length = strlen(sbuf.mtext) + 1;
    msgsnd(msqid_envio, &sbuf, buf_length, IPC_NOWAIT);
    printf("P1 envio: %s\n", sbuf.mtext);

    if (msgrcv(msqid_resp, &rbuf, MSGSZ, 1, 0) < 0) {
        perror("msgrcv resp"); exit(1);
    }
    printf("P1 recibio: %s\n", rbuf.mtext);

    strcpy(sbuf.mtext, "I can hear you too");
    buf_length = strlen(sbuf.mtext) + 1;
    msgsnd(msqid_envio, &sbuf, buf_length, IPC_NOWAIT);
    printf("P1 envio: %s\n", sbuf.mtext);

    msgctl(msqid_envio, IPC_RMID, NULL);
    msgctl(msqid_resp, IPC_RMID, NULL);
    
    return 0;
}
