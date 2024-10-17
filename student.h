#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STUDENT_NUM 400
#define MAX_SUBJECT_NUM 40

// ѧ���ṹ��
typedef struct Student {
	int id;                     // ѧ��ID
	char name[50];              // ѧ������
	char major[50];             // ��ѧרҵ������
	float score;                // �ɼ�
	char achievements[100];     // ��������Ŀ��Ӧ�ɹ�
	float bonusPoints;          // �ӷ�
	float totalPoints;          // �ܳɼ�
	char password[30];          // �����������ֶ�
	struct Student* next;       // ָ����һ��ѧ����ָ��
} Student;


// ѧ��������
typedef struct StudentManager {
	Student* head;              // ѧ�������ͷָ��
	int count;                  // ѧ������
	int cur;
} StudentManager;

// ��ʼ��ѧ��������
void initStudentManager(StudentManager* manager) {
	manager->head = NULL;
	manager->count = 0;
	manager->cur = -1;
}

// ����ѧ���ڵ�
Student* createStudent(int id, const char* name, const char* major, float score, const char* achievements, float bonusPoints, const char* password) {
	Student* student = (Student*)malloc(sizeof(Student));
	student->id = id;
	strcpy(student->name, name);
	strcpy(student->major, major);
	student->score = score;
	strcpy(student->achievements, achievements);
	student->bonusPoints = bonusPoints;
	student->totalPoints = score + bonusPoints;
	strcpy(student->password, password);  // ��������������
	student->next = NULL;
	return student;
}

void sortByScore(StudentManager* manager) {
	if (manager->head == NULL || manager->count < 2) {
		return; // �������Ϊ�ջ�ֻ��һ��ѧ��������Ҫ����
	}

	Student* cur = manager->head;
	Student* next;
	while (cur != NULL) {
		next = cur->next;
		while (next != NULL) {
			if (cur->totalPoints < next->totalPoints) {
				// �������е�ѧ���ֶ�
				Student temp = *cur;
				temp.next = cur->next; // ����ԭ���������

				// ���������ֶΣ�����ָ�뵽��һ���ڵ�� 'next' �ֶ�
				cur->id = next->id;
				strcpy(cur->name, next->name);
				strcpy(cur->major, next->major);
				cur->score = next->score;
				strcpy(cur->achievements, next->achievements);
				cur->bonusPoints = next->bonusPoints;
				cur->totalPoints = next->totalPoints;
				strcpy(cur->password, next->password);
				//ȷ��next�ڵ�ָ���ֶβ���
				Student* tempNext = next->next;

				*next = temp;

				next->next = tempNext;
			}
			next = next->next;
		}
		cur = cur->next;
	}
}

// ��ѧ���������в���ѧ���ڵ�
void insertStudent(StudentManager* manager, Student* newStudent) {
	if (manager->count >= MAX_STUDENT_NUM) {
		printf("ѧ���������������޷������ѧ����\n");
		return;
	}

	if (manager->head == NULL) {
		manager->head = newStudent;
	} else {
		Student* current = manager->head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = newStudent;
	}
	manager->count++;
}


// ����û��Ƿ���ڲ���¼
int loginUser(StudentManager* manager) {
	int id;
	char password[30];
	printf("ID�� ");
	scanf("%d", &id);
	printf("���룺 ");
	scanf("%s", password);
	Student* current = manager->head;
	while (current != NULL) {
		if (current->id == id) {
			if (strcmp(current->password, password) == 0) {
				printf("��¼�ɹ�\n");
				manager->cur = id;
				return 1;  // ��¼�ɹ�
			} else {
				printf("�������\n");
				return 0; // �������
			}
		}
		current = current->next;
	}
	printf("�û�������\n");
	return 0;  // �û�������
}

// ��ӡѧ���������е�ѧ����Ϣ
void printAllStudents(StudentManager* manager) {
	Student* current = manager->head;
	while (current != NULL) {
		printf("ID: %d\n", current->id);
		printf("Name: %s\n", current->name);
		printf("Major: %s\n", current->major);
		printf("Score: %.5f\n", current->score);
		printf("Achievements: %s\n", current->achievements);
		printf("Bonus Points: %.5f\n", current->bonusPoints);
		printf("Total Points: %.5f\n\n", current->totalPoints);
		current = current->next;
	}
}

// �ͷ�ѧ���������е�ѧ���ڵ��ڴ�
void freeStudents(StudentManager* manager) {
	Student* current = manager->head;
	while (current != NULL) {
		Student* next = current->next;
		free(current);
		current = next;
	}
	manager->head = NULL;
	manager->count = 0;
}

// ���ļ��ж�ȡѧ����Ϣ
void readStudentInfoFromFile(StudentManager* manager, const char* filename) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("�޷����ļ� %s\n", filename);
		return;
	}

	int id;
	char name[50];
	char major[50];
	float score;
	char achievements[100];
	float bonusPoints;
	char password[30];  // ������������ʱ�洢��ȡ������

	while (fscanf(file, "%d %s %s %f %s %f %s\n", &id, name, major, &score, achievements, &bonusPoints, password) == 7) {  // ����fscanf�����԰�������
		// ���������ݽ�����֤
		if (score < 0 || score > 4.0 || bonusPoints < 0 || bonusPoints > 1.0) {
			printf("�ļ��е����ݲ����Ϲ淶��\n");
			continue;
		}

		// ����ѧ���ڵ㲢����ѧ���������У�����ȡ����������Ϊ��������
		Student* newStudent = createStudent(id, name, major, score, achievements, bonusPoints, password);
		insertStudent(manager, newStudent);
	}

	fclose(file);
}


void writeStudentToFile(const char* filename, const StudentManager* manager) {
	FILE* file = fopen(filename, "w");
	if (file == NULL) {
		printf("���ļ�ʧ��: %s\n", filename);
		return;
	}

	Student* current = manager->head;
	while (current != NULL) {
		fprintf(file, "%d %s %s %.2f %s %.2f %s\n", current->id, current->name, current->major, current->score,
		        current->achievements, current->bonusPoints, current->password);
		current = current->next;
	}

	fclose(file);
}



void printStu(const Student* cur) {
	printf("ѧ����Ϣ��\n");
	printf("ѧ��ID: %d\n", cur->id);
	printf("����: %s\n", cur->name);
	printf("��ѧרҵ: %s\n", cur->major);
	printf("�ɼ�: %.5f\n", cur->score);
	printf("��������Ŀ��Ӧ�ɹ�: %s\n", cur->achievements);
	printf("�ӷ�: %.5f\n", cur->bonusPoints);
	printf("�ܳɼ�: %.5f\n\n", cur->bonusPoints + cur->score);
}

void searchStudentById(const StudentManager* manager) {
	int id = -1;
	printf("�����ѯ��ѧ��id��\n");
	scanf("%d", &id);
	Student* current = manager->head;
	while (current != NULL) {
		if (current->id == id) {
			printStu(current);
			return;
		}
		current = current->next;
	}
	printf("δ�ҵ�ѧ��Ϊ %d ��ѧ��\n", id);
}

void modifyStudentPassword(StudentManager* manager, int id) {
	char newpassword[30];
	printf("�����룺 ");
	scanf("%29s", newpassword);

	Student* current = manager->head;
	while (current != NULL) {
		if (current->id == id) {
			strcpy(current->password, newpassword);
			printf("�����޸ĳɹ���\n");
			return;
		}
		current = current->next;
	}
	printf("�Ҳ���IDΪ%d��ѧ����\n", id);
}
void checkStuSelf(const StudentManager* manager, int id) {
	Student* current = manager->head;
	while (current != NULL) {
		if (current->id == id) {
			printStu(current);
			return;
		}
		current = current->next;
	}
	printf("δ�ҵ�ѧ��Ϊ %d ��ѧ��\n", id);
}

// �޸�ָ�� id ������
int changePassword(StudentManager* manager, int id) {
	char newpassword[30];
	printf("���룺 ");
	scanf("%s", newpassword);
	Student* current = manager->head;
	while (current != NULL) {
		if (current->id == id) {
			strcpy(current->password, newpassword);
			printf("�޸ĳɹ�\n ");
			return 1;  // �޸ĳɹ�
		}
		current = current->next;
	}
	printf("�û������ڻ� id ����\n ");
	return 0;  // �û������ڻ� id ����
}
void printStudentsByMajor(StudentManager* manager, char major[]) {
	Student* cur = manager->head;
	while (cur != NULL) {
		if (strcmp(cur->major, major) == 0) {
			printStu(cur);
		}
		cur = cur->next;
	}
}

// �޸�ָ�� id ��ѧ����Ϣ
int modifyStudentInfo(StudentManager* manager) {
	int id = -1;
	printf("�����޸ĵ�ѧ��id��\n");
	scanf("%d", &id);
	// ��ȡѧ����Ϣ
	Student* current = manager->head;
	while (current != NULL) {
		if (current->id == id) {
			char newName[50];
			char newMajor[50];
			float newScore;
			char newAchievements[100];
			float newBonusPoints;

			printf("�������µ�������");
			scanf("%s", newName);
			printf("�������µ�רҵ��");
			scanf("%s", newMajor);
			printf("�������µĳɼ���");
			scanf("%f", &newScore);
			printf("�������µ���������Ŀ��Ӧ�ɹ���");
			scanf("%s", newAchievements);
			printf("�������µļӷ֣�");
			scanf("%f", &newBonusPoints);

			// �޸�ѧ����Ϣ
			strcpy(current->name, newName);
			strcpy(current->major, newMajor);
			current->score = newScore;
			strcpy(current->achievements, newAchievements);
			current->bonusPoints = newBonusPoints;
			current->totalPoints = newScore + newBonusPoints;

			return 1;  // �޸ĳɹ�
		}
		current = current->next;
	}
	return 0;  // ѧ�������ڻ� id ����
}

void deleteTeacherById(StudentManager* manager) {
	int id = -1;
	printf("����ɾ����ѧ��id��\n");
	scanf("%d", &id);
	Student* current = manager->head;
	Student* previous = NULL;

	while (current != NULL) {
		if (current->id == id) {
			if (previous == NULL) {
				// Ҫɾ���Ľڵ���ͷ�ڵ�
				manager->head = current->next;
			} else {
				previous->next = current->next;
			}

			free(current);
			manager->count--;
			return;
		}

		previous = current;
		current = current->next;
	}

	printf("δ�ҵ�IDΪ %d ����ʦ\n", id);
}
