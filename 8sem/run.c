#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

const size_t BUFFER_SIZE = 1024;
struct my_msgbuf {
	long mtype;
	char mtext[BUFFER_SIZE];
};

int judge(int msgid, int runners) {
	struct my_msgbuf buf;
	printf("Судья начал регистрацию\n");
	for (int i = 0; i < runners; i++) {
		if (msgrcv(msgid, &buf, BUFFER_SIZE, 1024, 0) < 0) {
			perror("judge recieve error");
		}
		printf("Зарегестрировал участника %s\n", buf.mtext);
	}
	printf("Регистрация завершена\n");

	printf("Передаю палочку первому бегуну\n");
	buf.mtype = 1;
	if (msgsnd(msgid, &buf, BUFFER_SIZE, 0) < 0) {
		perror("judge send error");
	}
	if (msgrcv(msgid, &buf, BUFFER_SIZE, runners + 1, 0) < 0) {
		perror("judge recieve error");
	}
	printf("Забрал палочку у последнего игрока\n");
	printf("Завершил забег\n");
	return 0;
}

int runner(int msgid, int id, int runners) {
	struct my_msgbuf buf;
	buf.mtype = 1024;
	sprintf(buf.mtext, "%d", id);
	printf("Бегун %s пришел на регистрацию\n", buf.mtext);
	if (msgsnd(msgid, &buf, BUFFER_SIZE, 0) < 0) {
		perror("runner send error");
	}

	if (msgrcv(msgid, &buf, BUFFER_SIZE, id, 0) < 0) {
		perror("runner recieve error");
	}
	printf("Бегун %d принял палочку\n", id);
	buf.mtype = id + 1;
	if (msgsnd(msgid, &buf, BUFFER_SIZE, 0) < 0) {
		perror("runner send error");
	}
	printf("Бегун %d передал палочку следующему бегуну\n", id);
	return 0;
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Мало аргументов\n");
		return 0;
	}
	int runners_n = atoi(argv[1]);
	int msgid = msgget(IPC_PRIVATE, 0777 | IPC_CREAT);

	if (fork() == 0) {
		judge(msgid, runners_n);
		return 0;
	}

	for (int i = 0; i < runners_n; i++) {
		if (fork() == 0) {
			runner(msgid, i + 1, runners_n);
			return 0;
		}
	}


	for (int i = 0; i < runners_n + 1; i++) wait(NULL);
	msgctl(msgid, IPC_RMID, 0);
}
