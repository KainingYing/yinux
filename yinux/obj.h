#pragma once
#include <string>
using namespace std;


//������
class superblock
{
public:
	superblock();

	int inodeNum; //inode������
	int inodeFree;//inode�Ŀ�����
	int inodeSize;//inode�Ĵ�С
	bool inodeBit[128]; //inode�Ŀ���
	int blockNum;//block������
	int blockFree;//block�Ŀ�������
	int blockSize;//block�Ĵ�С
	int currentStack[51];//    stack
	int userNum;   //the num of user
	

};

class inode
{
public:


	int inodeNum;          //inode��
	int dirSize;          //�ļ�/Ŀ¼��С
	int dirConnect;  //�ļ�������
	int dirAddr[4];  //ֱ��Ѱַ
	int dirAddr1;  //һ�����Ѱַ
	int dirAddr2;	//�������Ѱַ
	int dirOwner;//�ļ�ӵ����ID
	char dirGroup[6];//belongs to which group
	int dirMode;
	int dirPermission;  //file/directory permission
	char dirTime[64];//latest modified time


};

class directory
{
public:


	char fname[10];//directory or file name
	int index;//inode num
	bool isdir;//whether directory  1=dir    0=file
	string content;//directory is empty
	int parent;//parent's inode num
	int firstChild;//first child's inode num(file is NULL)
	int nextSibling;	//next brother directory or file

};



class user
{


public:
	int userId;//0=admin 1,2,3,4,5,6.����9=guest
	char userName[6];
	char password[6];
	char group[6];
	int umask[3];

};



class yinux
{
	/*file system
	author : noobying
	*/
public:
	yinux();//1��format disk and read file from disk
	bool format();//format the filedisk
	bool readDisk();//read the filedisk
	bool login();//login function
	void kernal();//the most important part of linux


	void pwd(directory* dir);
	void passwd(user* User);
	void ls(directory* dir);
	void mkdir(string filename);//create the new directory
	void rmdir(string filename);//delete the directory
	void rm(string filename);//delete the file
	void touch(string filename);// create the new file
	void cd(string filename);//change the directory path
	void umask(int number);
	void cat(string filename);

	void releaseSpace(int inodeNum);
	int findPreviousDirectory(int inodeNum);
	int findBrotherDirectory(int inodeNum);
	directory* createDirectory(int inodeNum,char* filename,bool flag);
	int allocateInode();
	void allocateSpace(int inodeNum, directory * dir);//allocate space to new file or directory
	inode* readInode(int inodeNum);//read a inode form disk
	void writeInode(int inodeNum, inode* newInode);//write a inode into disk
	void writeDirectory(int inodeNum, directory* dir);//write directory into disk
	directory* readDirectory(int inodeNum);//read directory into disk
	void writeSuperblock();//update the Superblock
	int findDirectory(const char* filename);//finding correspond directory based on filename
	void allocateEnoughSpace(directory* dir);//the most important part    to the best of my knowledge	
	int findAvailableBlock();//a api to find proper BLOCK
	void writeEnoughDirectory(directory* dir);
	FILE* file;



	user User;
	
	superblock Superblock;
	directory* currDirectory;//Current directory or file
	int stackPtr;//ponit to which block group






	//ying@ying-TM1701:~$


};




#define INODE_PER_FILE 6
#define BLOCK_SIZE 512
#define BLOCK_NUM 512
#define INODE_SIZE 128
#define INODE_NUM 256
#define INODE_START 2*BLOCK_SIZE  //2 means one empty and one superblock
#define DATA_START INODE_START + INODE_NUM*INODE_SIZE

#define MAX_PERMISSION 511 //111 111 111
#define MAX_OWNER_PERMISSION 448 //111 000 000
//
//FILE* file;

//const unsigned int INODE_PER_FILE = 6;                //�ļ���ʹ�õ���������ڵ�
//const unsigned int BLOCK_SIZE = 512;              //һ��block�Ĵ�С
//const unsigned int BLOCK_NUM = 512;              //block������
//const unsigned int INODE_SIZE = 128;              //inode��С
//const unsigned int INODE_NUM = 256;              //inode����
//const unsigned int INODE_START = 3 * BLOCK_SIZE;   //inode����ʼλ��
//const unsigned int DATA_START = INODE_START + INODE_NUM * INODE_SIZE; //������ʼλ��
//const unsigned int ACCOUNT_NUM = 10;               //�ļ�ϵͳ֧�ֵ��û�����
//
//const unsigned int DIRECTORY_NUM = 16;         //��Ŀ¼���������
//const unsigned int FILE_NAME_LENGTH = 14;         //�ļ�������󳤶�
//const unsigned int USER_NAME_LENGTH = 14;         //�û�������󳤶�
//const unsigned int USER_PASSWORD_LENGTH = 14;         //�û��������󳤶�
//
//const unsigned int MAX_PERMISSION = 511;        //�ļ����Ȩ��
//const unsigned int MAX_OWNER_PERMISSION = 448;        //�û����Ȩ��M 


