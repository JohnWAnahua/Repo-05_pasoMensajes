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
    message_buf rbuf, sbuf;
    size_t buf_length;

    if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0) {
        perror("msgget"); exit(1);
    }

    printf("Servidor listo. Esperando mensajes...\n");

    while(1) {
        if (msgrcv(msqid, &rbuf, MSGSZ, 0, 0) < 0) {
            perror("msgrcv"); exit(1);
        }

        rbuf.mtext[MSGSZ - 1] = '\0';
        printf("Servidor recibio: '%s' (Tipo: %ld)\n", rbuf.mtext, rbuf.mtype);

        if (rbuf.mtype == 1) {
            sbuf.mtype = 10;
            snprintf(sbuf.mtext, MSGSZ, "Hola Cliente 1, recibi: %.100s", rbuf.mtext);
        } else if (rbuf.mtype == 2) {
            sbuf.mtype = 20;
            snprintf(sbuf.mtext, MSGSZ, "Hola Cliente 2, recibi: %.100s", rbuf.mtext);
        } else {
            continue;
        }

        buf_length = strlen(sbuf.mtext) + 1;
        if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0) {
            perror("msgsnd"); exit(1);
        }
    }
    return 0;
}
