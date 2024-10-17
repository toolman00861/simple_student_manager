#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "admin.h"
#include "teacher.h"
#include "student.h"

#define MAX_SUBJECT_NUM 40

void menu() {
	printf(" ��ӭʹ�õ�¼ϵͳ��\n");
	printf(" ��ѡ���¼���ͣ�\n");
	printf(" 1. ����Ա\n");
	printf(" 2. ��ʦ\n");
	printf(" 3. ѧ��\n");
	printf(" 0. �˳�\n");
	printf(" ������ѡ��: ");
}

void adminmenu() {
	printf("\n====== �˵����� ======\n");
	printf("1. ��ӡ���й���Ա\n");
	printf("2. ��ӡ������ʦ\n");
	printf("3. ��ӡ����ѧ��\n");
	printf("4. �޸���ʦ��Ϣ\n");
	printf("5. ɾ����ʦ��Ϣ\n");
	printf("6. ͳ��ѧ������\n");
	printf("7. �޸���������\n");
	printf("0. �˳�\n");
	printf("��ѡ��һ������: ");
}

void teachermenu() {
	printf("\n===== ѧ������ϵͳ�˵� =====\n");
	printf("1. ��ӡ����רҵ��ѧ����Ϣ\n");
	printf("2. ���ܳɼ������ѯѧ������Ϣ\n");
	printf("3. �޸���������\n");
	printf("4. �޸�ѧ����Ϣ\n");
	printf("0. �˳�\n");
	printf("��ѡ�����������0-4��: ");
}

void stumenu() {
	printf("\n===== ѧ������ϵͳ�˵� =====\n");
	printf("1. �޸���������\n");
	printf("2. �鿴������Ϣ\n");
	printf("0. �˳�\n");
	printf("��ѡ�����: ");
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
							printf("��ǰѧ��������%d\n\n", stumanager.count);
							break;
						case 7:
							modifyAdminPassword(&adminmanager, adminmanager.cur);
							break;
						case 0:
							out = 1;
							break;
						default:
							printf("��Ч��ѡ��\n");
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
							printf("��Ч��ѡ��\n");
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
							printf("��Ч��ѡ��\n");
							break;
					}
				}
				break;
			default:
				printf("��Ч��ѡ��\n");
				break;
		}
		printf("\n");

		writeAdminsToFile( "admin.txt", &adminmanager);
		writeTeachersToFile("teacher.txt", &teachermanager);
		writeStudentToFile("student.txt", &stumanager);
	}
}
