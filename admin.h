#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ADMIN_NUM 5

typedef struct Admin {
	int id;
	char password[50];
	char username[50];
	struct Admin* next;
} Admin;

typedef struct {
	Admin* head;
	int count;
	int cur;
} AdminManager;

void initAdminManager(AdminManager* manager) {
	manager->head = NULL;
	manager->count = 0;
	manager->cur = -1;
}

void insertAdmin(AdminManager* manager, Admin* newAdmin) {
	if (manager->count >= MAX_ADMIN_NUM) {
		printf("管理员已满。\n");
		return;
	}
	if (manager->head == NULL) {
		manager->head = newAdmin;
	} else {
		Admin* current = manager->head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = newAdmin;
	}
	manager->count++;
}

Admin* createAdmin(int id, const char* password, const char* username) {
	Admin* newAdmin = (Admin*)malloc(sizeof(Admin));
	newAdmin->id = id;
	strcpy(newAdmin->password, password);
	strcpy(newAdmin->username, username);
	newAdmin->next = NULL;
	return newAdmin;
}

void modifyAdminPassword(AdminManager* manager, int id) {
	char newpassword[30];
	printf("新密码： ");
	scanf("%s", newpassword);

	Admin* current = manager->head;
	while (current != NULL) {
		if (current->id == id) {
			strcpy(current->password, newpassword);
			printf("成功修改。\n");
			return;
		}
		current = current->next;
	}
	printf("找不到ID为%d的管理员。\n", id);
}

void readAdminsFromFile(AdminManager* manager, const char* filename) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("无法打开文件 %s\n", filename);
		return;
	}

	int id;
	char password[50];
	char username[50];

	while (fscanf(file, "%d %s %s\n", &id, password, username) == 3) {
		Admin* newAdmin = createAdmin(id, password, username);
		insertAdmin(manager, newAdmin);
	}

	fclose(file);
}

void writeAdminsToFile(const char* filename, const AdminManager* manager) {
	FILE* file = fopen(filename, "w");
	if (file == NULL) {
		printf("Failed to open file: %s\n", filename);
		return;
	}

	Admin* current = manager->head;
	while (current != NULL) {
		fprintf(file, "%d %s %s\n", current->id, current->password, current->username);
		current = current->next;
	}

	fclose(file);
}
void printAllAdmins(const AdminManager* manager) {
	if (manager->count == 0) {
		printf("暂无管理员。\n");
		return;
	}

	Admin* current = manager->head;
	printf("当前管理员列表：\n");
	while (current != NULL) {
		printf("ID: %d, 用户名: %s\n", current->id, current->username);
		current = current->next;
	}
}
int loginAdmin(AdminManager* manager) {
	char acc[50];
	char password[50];
	printf("账户： ");
	scanf("%s", acc);
	printf("密码： ");
	scanf("%s", password);
	Admin* current = manager->head;
	while (current != NULL) {
		if (strcmp(current->username, acc) == 0) {
			if (strcmp(current->password, password) == 0) {
				printf("登录成功\n");
				manager->cur = current->id;
				return 1;  // 登录成功
			} else {
				printf("密码错误\n");
				return 0; // 密码错误
			}
		}
		current = current->next;
	}
	printf("管理员不存在\n");
	return 0;  // 管理员不存在
}
