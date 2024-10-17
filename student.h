#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STUDENT_NUM 400
#define MAX_SUBJECT_NUM 40

// 学生结构体
typedef struct Student {
	int id;                     // 学生ID
	char name[50];              // 学生姓名
	char major[50];             // 所学专业的名称
	float score;                // 成绩
	char achievements[100];     // 素质类项目对应成果
	float bonusPoints;          // 加分
	float totalPoints;          // 总成绩
	char password[30];          // 新增：密码字段
	struct Student* next;       // 指向下一个学生的指针
} Student;


// 学生管理类
typedef struct StudentManager {
	Student* head;              // 学生链表的头指针
	int count;                  // 学生数量
	int cur;
} StudentManager;

// 初始化学生管理类
void initStudentManager(StudentManager* manager) {
	manager->head = NULL;
	manager->count = 0;
	manager->cur = -1;
}

// 创建学生节点
Student* createStudent(int id, const char* name, const char* major, float score, const char* achievements, float bonusPoints, const char* password) {
	Student* student = (Student*)malloc(sizeof(Student));
	student->id = id;
	strcpy(student->name, name);
	strcpy(student->major, major);
	student->score = score;
	strcpy(student->achievements, achievements);
	student->bonusPoints = bonusPoints;
	student->totalPoints = score + bonusPoints;
	strcpy(student->password, password);  // 新增：复制密码
	student->next = NULL;
	return student;
}

void sortByScore(StudentManager* manager) {
	if (manager->head == NULL || manager->count < 2) {
		return; // 如果链表为空或只有一个学生，不需要排序
	}

	Student* cur = manager->head;
	Student* next;
	while (cur != NULL) {
		next = cur->next;
		while (next != NULL) {
			if (cur->totalPoints < next->totalPoints) {
				// 交换所有的学生字段
				Student temp = *cur;
				temp.next = cur->next; // 保持原链表的连接

				// 交换所有字段，除了指针到下一个节点的 'next' 字段
				cur->id = next->id;
				strcpy(cur->name, next->name);
				strcpy(cur->major, next->major);
				cur->score = next->score;
				strcpy(cur->achievements, next->achievements);
				cur->bonusPoints = next->bonusPoints;
				cur->totalPoints = next->totalPoints;
				strcpy(cur->password, next->password);
				//确保next节点指针字段不变
				Student* tempNext = next->next;

				*next = temp;

				next->next = tempNext;
			}
			next = next->next;
		}
		cur = cur->next;
	}
}

// 在学生管理类中插入学生节点
void insertStudent(StudentManager* manager, Student* newStudent) {
	if (manager->count >= MAX_STUDENT_NUM) {
		printf("学生管理类已满，无法再添加学生。\n");
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


// 检测用户是否存在并登录
int loginUser(StudentManager* manager) {
	int id;
	char password[30];
	printf("ID： ");
	scanf("%d", &id);
	printf("密码： ");
	scanf("%s", password);
	Student* current = manager->head;
	while (current != NULL) {
		if (current->id == id) {
			if (strcmp(current->password, password) == 0) {
				printf("登录成功\n");
				manager->cur = id;
				return 1;  // 登录成功
			} else {
				printf("密码错误\n");
				return 0; // 密码错误
			}
		}
		current = current->next;
	}
	printf("用户不存在\n");
	return 0;  // 用户不存在
}

// 打印学生管理类中的学生信息
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

// 释放学生管理类中的学生节点内存
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

// 从文件中读取学生信息
void readStudentInfoFromFile(StudentManager* manager, const char* filename) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("无法打开文件 %s\n", filename);
		return;
	}

	int id;
	char name[50];
	char major[50];
	float score;
	char achievements[100];
	float bonusPoints;
	char password[30];  // 新增：用于临时存储读取的密码

	while (fscanf(file, "%d %s %s %f %s %f %s\n", &id, name, major, &score, achievements, &bonusPoints, password) == 7) {  // 更新fscanf参数以包含密码
		// 对输入数据进行验证
		if (score < 0 || score > 4.0 || bonusPoints < 0 || bonusPoints > 1.0) {
			printf("文件中的数据不符合规范！\n");
			continue;
		}

		// 创建学生节点并插入学生管理类中，将读取到的密码作为参数传递
		Student* newStudent = createStudent(id, name, major, score, achievements, bonusPoints, password);
		insertStudent(manager, newStudent);
	}

	fclose(file);
}


void writeStudentToFile(const char* filename, const StudentManager* manager) {
	FILE* file = fopen(filename, "w");
	if (file == NULL) {
		printf("打开文件失败: %s\n", filename);
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
	printf("学生信息：\n");
	printf("学生ID: %d\n", cur->id);
	printf("姓名: %s\n", cur->name);
	printf("所学专业: %s\n", cur->major);
	printf("成绩: %.5f\n", cur->score);
	printf("素质类项目对应成果: %s\n", cur->achievements);
	printf("加分: %.5f\n", cur->bonusPoints);
	printf("总成绩: %.5f\n\n", cur->bonusPoints + cur->score);
}

void searchStudentById(const StudentManager* manager) {
	int id = -1;
	printf("输入查询的学生id：\n");
	scanf("%d", &id);
	Student* current = manager->head;
	while (current != NULL) {
		if (current->id == id) {
			printStu(current);
			return;
		}
		current = current->next;
	}
	printf("未找到学号为 %d 的学生\n", id);
}

void modifyStudentPassword(StudentManager* manager, int id) {
	char newpassword[30];
	printf("新密码： ");
	scanf("%29s", newpassword);

	Student* current = manager->head;
	while (current != NULL) {
		if (current->id == id) {
			strcpy(current->password, newpassword);
			printf("密码修改成功。\n");
			return;
		}
		current = current->next;
	}
	printf("找不到ID为%d的学生。\n", id);
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
	printf("未找到学号为 %d 的学生\n", id);
}

// 修改指定 id 的密码
int changePassword(StudentManager* manager, int id) {
	char newpassword[30];
	printf("密码： ");
	scanf("%s", newpassword);
	Student* current = manager->head;
	while (current != NULL) {
		if (current->id == id) {
			strcpy(current->password, newpassword);
			printf("修改成功\n ");
			return 1;  // 修改成功
		}
		current = current->next;
	}
	printf("用户不存在或 id 错误\n ");
	return 0;  // 用户不存在或 id 错误
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

// 修改指定 id 的学生信息
int modifyStudentInfo(StudentManager* manager) {
	int id = -1;
	printf("输入修改的学生id：\n");
	scanf("%d", &id);
	// 获取学生信息
	Student* current = manager->head;
	while (current != NULL) {
		if (current->id == id) {
			char newName[50];
			char newMajor[50];
			float newScore;
			char newAchievements[100];
			float newBonusPoints;

			printf("请输入新的姓名：");
			scanf("%s", newName);
			printf("请输入新的专业：");
			scanf("%s", newMajor);
			printf("请输入新的成绩：");
			scanf("%f", &newScore);
			printf("请输入新的素质类项目对应成果：");
			scanf("%s", newAchievements);
			printf("请输入新的加分：");
			scanf("%f", &newBonusPoints);

			// 修改学生信息
			strcpy(current->name, newName);
			strcpy(current->major, newMajor);
			current->score = newScore;
			strcpy(current->achievements, newAchievements);
			current->bonusPoints = newBonusPoints;
			current->totalPoints = newScore + newBonusPoints;

			return 1;  // 修改成功
		}
		current = current->next;
	}
	return 0;  // 学生不存在或 id 错误
}

void deleteTeacherById(StudentManager* manager) {
	int id = -1;
	printf("输入删除的学生id：\n");
	scanf("%d", &id);
	Student* current = manager->head;
	Student* previous = NULL;

	while (current != NULL) {
		if (current->id == id) {
			if (previous == NULL) {
				// 要删除的节点是头节点
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

	printf("未找到ID为 %d 的老师\n", id);
}
