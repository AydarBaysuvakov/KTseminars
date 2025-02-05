#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>

int main() {
	uid_t uid = getuid();
	struct passwd *pwd = getpwuid(uid);

	gid_t gid = getgid();
	struct group  *grp = getgrgid(gid);

	gid_t groups[100];
	int group_num = getgroups(100, groups);

	printf("uid: %d, name: %s \n", uid, pwd->pw_name);
	printf("gid: %d, name: %s \n", gid, grp->gr_name);
	printf("groups: %d\n", group_num);

	for (int i = 0; i < group_num; ++i) {
		grp = getgrgid(groups[i]);
		printf("%d, %s \n", groups[i], grp->gr_name);
	}
}
