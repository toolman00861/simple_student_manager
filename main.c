#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "admin.h"
#include "teacher.h"
#include "student.h"

#define MAX_SUBJECT_NUM 40

void menu() {
	printf(" 欢迎使用登录系统！\n");
	printf(" 请选择登录类型：\n");
	printf(" 1. 管理员\n");
	printf(" 2. 老师\n");
	printf(" 3. 学生\n");
	printf(" 0. 退出\n");
	printf(" 请输入选择: ");
}

void adminmenu() {
	printf("\n====== 菜单功能 ======\n");
	printf("1. 打印所有管理员\n");
	printf("2. 打印所有老师\n");
	printf("3. 打印所有学生\n");
	printf("4. 修改老师信息\n");
	printf("5. 删除老师信息\n");
	printf("6. 统计学生总数\n");
	printf("7. 修改自身密码\n");
	printf("0. 退出\n");
	printf("请选择一个操作: ");
}

void teachermenu() {
	printf("\n===== 学生管理系统菜单 =====\n");
	printf("1. 打印所在专业的学生信息\n");
	printf("2. 按总成绩排序查询学生的信息\n");
	printf("3. 修改自身密码\n");
	printf("4. 修改学生信息\n");
	printf("0. 退出\n");
	printf("请选择操作（数字0-4）: ");
}

void stumenu() {
	printf("\n===== 学生管理系统菜单 =====\n");
	printf("1. 修改自身密码\n");
	printf("2. 查看自身信息\n");
	printf("0. 退出\n");
	printf("请选择操作: ");
}

int main() {
	StudentManager stumanager;
	TeacherManager teachermanager;
	AdminManager adminmanager;
	initStudentManager(&stumanager);
	initTeacherManager(&teachermanager);
	initAdminManager(&adminmanager);
	readAdminsFromFile(&adminmanager, "admin.txt");
	readTeachersFromFile(&teachermanager, "teacher.txt");
	readStudentInfoFromFile(&stumanager, "student.txt");

	int choice;
	while (1) {
		menu();
		scanf("%d", &choice);
		int out = 0;
		int login = 0;
		switch (choice) {
			case 0:
				writeAdminsToFile( "admin.txt", &adminmanager);
				writeTeachersToFile("teacher.txt", &teachermanager);
				writeStudentToFile("student.txt", &stumanager);
				return 0;
			case 1:
				login = 0;
				login = loginAdmin(&adminmanager);
				out = 0;
				while (!out && login) {
					int choice1;
					adminmenu();
					scanf("%d", &choice1);
					switch (choice1) {
						case 1:
							printAllAdmins(&adminmanager);
							break;
						case 2:
							printAllTeachers(&teachermanager);
							break;
						case 3:
							printAllStudents(&stumanager);
							break;
						case 4:
							modifyTeacherInfo(&teachermanager);
							break;
						case 5:
							deleteTeacher(&teachermanager);
							break;
						case 6:
							printf("当前学生数量：%d\n\n", stumanager.count);
							break;
						case 7:
							modifyAdminPassword(&adminmanager, adminmanager.cur);
							break;
						case 0:
							out = 1;
							break;
						default:
							printf("无效的选择\n");
							break;
					}
				}
				break;
			case 2:
				login = 0;
				login = loginTeacher(&teachermanager);
				out = 0;
				while (!out && login) {
					int choice1;
					teachermenu();
					scanf("%d", &choice1);
					switch (choice1) {
						case 1:
							printStudentsByMajor(&stumanager, teachermanager.curmajor);
							break;
						case 2:
							sortByScore(&stumanager);
							printStudentsByMajor(&stumanager, teachermanager.curmajor);
							break;
						case 3:
							modifyTeacherPassword(&teachermanager, teachermanager.cur);
							break;
						case 4:
							modifyStudentInfo(&stumanager);
							break;
						case 0:
							out = 1;
							break;
						default:
							printf("无效的选择\n");
							break;
					}
				}
				break;
			case 3:
				login = 0;
				login = loginUser(&stumanager);
				out = 0;
				while (!out && login) {
					int choice1;
					stumenu();
					scanf("%d", &choice1);
					switch (choice1) {
						case 1:
							modifyStudentPassword(&stumanager, stumanager.cur);
							break;
						case 2:
							checkStuSelf(&stumanager, stumanager.cur);
							break;
						case 3:

							break;
						case 0:
							out = 1;
							break;
						default:
							printf("无效的选择\n");
							break;
					}
				}
				break;
			default:
				printf("无效的选择\n");
				break;
		}
		printf("\n");

		writeAdminsToFile( "admin.txt", &adminmanager);
		writeTeachersToFile("teacher.txt", &teachermanager);
		writeStudentToFile("student.txt", &stumanager);
	}
}
