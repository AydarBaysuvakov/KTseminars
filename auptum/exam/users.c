#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>

const size_t BUFFER_SIZE = 1024;

struct node {
    uid_t uid;
    int count;
    struct node *next;
};

int main() {
    int user_n = 1;
    int exist = 0;

	struct node *head = (struct node*) malloc(sizeof(struct node));
    struct node *tail = head;
    head->uid = getuid();
    head->count = 0;
    head->next = NULL;


    uid_t uid;
    pid_t pid;
    struct passwd* pwd;

    while (pwd = getpwent()) {
        if (pwd->pw_uid != head->uid) {
            user_n++;
            tail->next = (struct node*) malloc(sizeof(struct node));
            tail = tail->next;
            tail->uid = pwd->pw_uid;
            tail->count = 0;
            tail->next = NULL;
        }
    }

    char buffer[BUFFER_SIZE] = "";
    
    FILE* ps = popen("ps --no-header -eo uid,pid", "r");
    if (!ps) {
        perror("popen failed");
        return 0;
    }
    
    while (fgets(buffer, BUFFER_SIZE, ps)) {
        sscanf(buffer, "%d %d", &uid, &pid);
        exist = 0;
        struct node *curent = head;
        for (int i = 0; i < user_n; ++i) {
            if (curent->uid == uid) {
                exist = 1;
                curent->count += 1;
            }
            curent = curent->next;
        }
        if (!exist) {
            user_n++;
            tail->next = (struct node*) malloc(sizeof(struct node));
            tail = tail->next;
            tail->uid = uid;
            tail->count = 1;
            tail->next = NULL;
        }
    }
    pclose(ps);
    
    struct node* to_del;
    struct node *curent = head;
    for (int i = 0; i < user_n; ++i) {
        struct passwd* pwd = getpwuid(curent->uid);
        printf("%s\t%d\n", pwd->pw_name, curent->count);
        to_del = curent;
        curent = curent->next;
        free(to_del);
    }
}
