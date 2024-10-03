#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <stdlib.h>

const int N = 20;

int main(int argc, char* argv[]) {
	uid_t uid;
	gid_t gid;
	struct passwd *pwd;
	struct group *grp;
	int group_num = N;
	gid_t groups[N];

	if (argc == 1) {
		uid = getuid();
		pwd = getpwuid(uid);

		gid = getgid();
		grp = getgrgid(gid);

		group_num = getgroups(N, groups);
	}
	else {
		pwd = getpwnam(argv[1]);
		if (!pwd) {
			printf("Wrong name!\n");
			return 0;
		}
		uid = pwd->pw_uid;
		grp = getgrnam(argv[1]);
		gid = grp->gr_gid;
		getgrouplist(argv[1], grp->gr_gid, groups, &group_num);
	}

	printf("uid: %d, name: %s \n", uid, pwd->pw_name);
	printf("gid: %d, name: %s \n", gid, grp->gr_name);
	printf("groups: %d\n", group_num);

	for (int i = 0; i < group_num; ++i) {
		grp = getgrgid(groups[i]);
		printf("%d, %s \n", groups[i], grp->gr_name);
	}
}
