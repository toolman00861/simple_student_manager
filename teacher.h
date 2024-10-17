#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEACHER_NUM 30

typedef struct Teacher {
	int id;
	char name[50];
	char major[50];
	char password[50]; // ��������ֶ�
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
		printf("��ʦ������������\n");
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
// �������ܣ���ӡ������ʦ��Ϣ
void printAllTeachers(TeacherManager* manager) {
	if (manager->count == 0) {
		printf("û����ʦ��Ϣ��\n");
		return;
	}

	Teacher* cur = manager->head;
	while (cur != NULL) {
		printf("��ʦ��Ϣ��\n");
		printf("ID��%d\n", cur->id);
		printf("������%s\n", cur->name);
		printf("רҵ��%s\n", cur->major);
		printf("���룺%s\n\n", cur->password);
		cur = cur->next;
	}
}
void readTeachersFromFile(TeacherManager* manager, const char* filename) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("�޷����ļ� %s\n", filename);
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
	printf("�����룺 ");
	scanf("%s", newpassword);
	Teacher* current = manager->head;
	while (current != NULL) {
		if (current->id == id) {
			strcpy(current->password, newpassword);
			printf("�ɹ��޸ġ�\n");
			return;
		}
		current = current->next;
	}
	printf("�Ҳ���IDΪ%d����ʦ��\n", id);
}

void deleteTeacher(TeacherManager* manager) {
	int id = -1;
	printf("����IDɾ��: ");
	scanf("%d", &id);
	Teacher* current = manager->head;
	Teacher* prev = NULL;

	while (current != NULL) {
		if (current->id == id) {
			if (prev == NULL) {
				// Ҫɾ���Ľڵ���ͷ�ڵ�
				manager->head = current->next;
			} else {
				prev->next = current->next;
			}
			free(current);
			manager->count--;
			printf("�ɹ�ɾ��IDΪ%d����ʦ��\n", id);
			return;
		}
		prev = current;
		current = current->next;
	}

	printf("�Ҳ���IDΪ%d����ʦ��\n", id);
}

int modifyTeacherInfo(TeacherManager* manager) {
	int id = -1;
	printf("����ID�޸�: ");
	scanf("%d", &id);
	// ��ȡ��ʦ��Ϣ
	Teacher* current = manager->head;
	while (current != NULL) {
		if (current->id == id) {
			char newName[50];
			char newMajor[50];
			char newPassword[50];

			printf("�������µ�������");
			scanf("%s", newName);
			printf("�������µ�רҵ��");
			scanf("%s", newMajor);
			printf("�������µ����룺");
			scanf("%s", newPassword);

			// �޸���ʦ��Ϣ
			strcpy(current->name, newName);
			strcpy(current->major, newMajor);
			strcpy(current->password, newPassword);

			return 1;  // �޸ĳɹ�
		}
		current = current->next;
	}
	printf("�Ҳ���IDΪ%d����ʦ��\n", id);
	return 0;  // ��ʦ�����ڻ� id ����
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
				printf("��¼�ɹ�\n");
				manager->cur = id;
				strcpy(manager->curmajor, current->major);
				return 1;  // ��¼�ɹ�
			} else {
				printf("�������\n");
				return 0;  // �������
			}
		}
		current = current->next;
	}
	printf("�û�������\n");
	return 0;  // �û�������
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
