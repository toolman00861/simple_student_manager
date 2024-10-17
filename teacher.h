#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEACHER_NUM 30

typedef struct Teacher {
	int id;
	char name[50];
	char major[50];
	char password[50]; // 添加密码字段
	struct Teacher* next;
} Teacher;

typedef struct {
	Teacher* head;
	int count;
	int cur;
	char curmajor[50];
} TeacherManager;

void initTeacherManager(TeacherManager* manager) {
	manager->head = NULL;
	manager->count = 0;
	manager->cur = 0;
}

void insertTeacher(TeacherManager* manager, Teacher* newTeacher) {
	if (manager->count >= MAX_TEACHER_NUM) {
		printf("老师管理类已满。\n");
		return;
	}
	if (manager->head == NULL) {
		manager->head = newTeacher;
	} else {
		Teacher* last = manager->head;
		while (last->next != NULL) {
			last = last->next;
		}
		last->next = newTeacher;
	}

	manager->count++;
}

Teacher* createTeacher(int id, const char* name, const char* major, const char* password) {
	Teacher* newTeacher = (Teacher*)malloc(sizeof(Teacher));
	newTeacher->id = id;
	strcpy(newTeacher->name, name);
	strcpy(newTeacher->major, major);
	strcpy(newTeacher->password, password);
	newTeacher->next = NULL;
	return newTeacher;
}
// 新增功能：打印所有老师信息
void printAllTeachers(TeacherManager* manager) {
	if (manager->count == 0) {
		printf("没有老师信息。\n");
		return;
	}

	Teacher* cur = manager->head;
	while (cur != NULL) {
		printf("老师信息：\n");
		printf("ID：%d\n", cur->id);
		printf("姓名：%s\n", cur->name);
		printf("专业：%s\n", cur->major);
		printf("密码：%s\n\n", cur->password);
		cur = cur->next;
	}
}
void readTeachersFromFile(TeacherManager* manager, const char* filename) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("无法打开文件 %s\n", filename);
		return;
	}

	int id;
	char name[50];
	char major[50];
	char password[50];

	while (fscanf(file, "%d %s %s %s", &id, name, major, password) == 4) {
		Teacher* newTeacher = createTeacher(id, name, major, password);

		insertTeacher(manager, newTeacher);
	}

	fclose(file);
}


void writeTeachersToFile(const char* filename, const TeacherManager* manager) {
	FILE* file = fopen(filename, "w");
	if (file == NULL) {
		printf("Failed to open file: %s\n", filename);
		return;
	}

	Teacher* current = manager->head;
	while (current != NULL) {
		fprintf(file, "%d %s %s %s\n", current->id, current->name, current->major, current->password );
		current = current->next;
	}

	fclose(file);
}

void modifyTeacherPassword(TeacherManager* manager, int id) {
	char newpassword[30];
	printf("新密码： ");
	scanf("%s", newpassword);
	Teacher* current = manager->head;
	while (current != NULL) {
		if (current->id == id) {
			strcpy(current->password, newpassword);
			printf("成功修改。\n");
			return;
		}
		current = current->next;
	}
	printf("找不到ID为%d的老师。\n", id);
}

void deleteTeacher(TeacherManager* manager) {
	int id = -1;
	printf("输入ID删除: ");
	scanf("%d", &id);
	Teacher* current = manager->head;
	Teacher* prev = NULL;

	while (current != NULL) {
		if (current->id == id) {
			if (prev == NULL) {
				// 要删除的节点是头节点
				manager->head = current->next;
			} else {
				prev->next = current->next;
			}
			free(current);
			manager->count--;
			printf("成功删除ID为%d的老师。\n", id);
			return;
		}
		prev = current;
		current = current->next;
	}

	printf("找不到ID为%d的老师。\n", id);
}

int modifyTeacherInfo(TeacherManager* manager) {
	int id = -1;
	printf("输入ID修改: ");
	scanf("%d", &id);
	// 获取老师信息
	Teacher* current = manager->head;
	while (current != NULL) {
		if (current->id == id) {
			char newName[50];
			char newMajor[50];
			char newPassword[50];

			printf("请输入新的姓名：");
			scanf("%s", newName);
			printf("请输入新的专业：");
			scanf("%s", newMajor);
			printf("请输入新的密码：");
			scanf("%s", newPassword);

			// 修改老师信息
			strcpy(current->name, newName);
			strcpy(current->major, newMajor);
			strcpy(current->password, newPassword);

			return 1;  // 修改成功
		}
		current = current->next;
	}
	printf("找不到ID为%d的老师。\n", id);
	return 0;  // 老师不存在或 id 错误
}
int loginTeacher(TeacherManager* manager) {
	int id;
	char password[30];
	printf("ID: ");
	scanf("%d", &id);
	printf("Password: ");
	scanf("%s", password);
	Teacher* current = manager->head;
	while (current != NULL) {
		if (current->id == id) {

			if (strcmp(current->password, password) == 0) {
				printf("登录成功\n");
				manager->cur = id;
				strcpy(manager->curmajor, current->major);
				return 1;  // 登录成功
			} else {
				printf("密码错误\n");
				return 0;  // 密码错误
			}
		}
		current = current->next;
	}
	printf("用户不存在\n");
	return 0;  // 用户不存在
}
void freeTeacherList(TeacherManager* manager) {
	Teacher* current = manager->head;
	while (current != NULL) {
		Teacher* next = current->next;
		free(current);
		current = next;
	}
	manager->head = NULL;
	manager->count = 0;
}
