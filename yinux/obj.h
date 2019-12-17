#pragma once
#include <string>
using namespace std;


//超级块
class superblock
{
public:
	superblock();

	unsigned short inodeNum; //inode的总数
	unsigned short inodeFree;//inode的空闲数
	unsigned short inodeSize;//inode的大小
	bool inodeBit[128]; //inode的空闲
	unsigned short blockNum;//block的数量
	unsigned short blockFree;//block的空闲数量
	unsigned short blockSize;//block的大小
	unsigned short currentStack[51];//    stack
	unsigned short userNum;   //the num of user
	

};

class inode
{
public:


	unsigned short inodeNum;          //inode号
	unsigned short dirSize;          //文件/目录大小
	unsigned short dirConnect;  //文件链接数
	unsigned short dirAddr[4];  //直接寻址
	unsigned short dirAddr1;  //一级间接寻址
	unsigned short dirAddr2;	//二级间接寻址
	unsigned short dirOwner;//文件拥有者ID
	unsigned short dirGroup;//belongs to which group
	unsigned short dirMode;
	unsigned short dirPermission;  //file/directory permission
	char dirTime[64];//latest modified time


};

class directory
{
public:


	char fname[10];//directory or file name
	unsigned short index;//inode num
	bool isdir;//whether directory  1=dir    0=file
	string content;//directory is empty
	unsigned short parent;//parent's inode num
	unsigned short firstChild;//first child's inode num(file is NULL)
	unsigned short nextSibling;	//next brother directory or file

};



class user
{


public:
	unsigned short userId;//0=admin 1,2,3,4,5,6.。。9=guest
	char userName[6];
	char password[6];
	char group[6];
	unsigned short umask[3];

};



class yinux
{
	/*file system
	author : noobying
	*/
public:
	yinux();//1、format disk and read file from disk
	bool format();//format the filedisk
	bool readDisk();//read the filedisk
	bool login();//login function


	void allocateSpace(unsigned short inodeNum, directory * dir);//allocate space to new file or directory
	inode* readInode(unsigned short inodeNum);//read a inode form disk
	void writeInode(unsigned short inodeNum, inode* newInode);//write a inode into disk
	void writeDirectory(unsigned short inodeNum, directory* dir);//write directory into disk
	directory* readDirectory(unsigned short inodeNum);//read directory into disk
	
	FILE* file;



	user User;
	
	superblock Superblock;
	directory* currDirectory;//Current directory or file
	unsigned short stackPtr;//ponit to which block group









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

//const unsigned int INODE_PER_FILE = 6;                //文件可使用的最大数量节点
//const unsigned int BLOCK_SIZE = 512;              //一个block的大小
//const unsigned int BLOCK_NUM = 512;              //block的数量
//const unsigned int INODE_SIZE = 128;              //inode大小
//const unsigned int INODE_NUM = 256;              //inode数量
//const unsigned int INODE_START = 3 * BLOCK_SIZE;   //inode的起始位置
//const unsigned int DATA_START = INODE_START + INODE_NUM * INODE_SIZE; //数据起始位置
//const unsigned int ACCOUNT_NUM = 10;               //文件系统支持的用户数量
//
//const unsigned int DIRECTORY_NUM = 16;         //子目录的最大数量
//const unsigned int FILE_NAME_LENGTH = 14;         //文件名的最大长度
//const unsigned int USER_NAME_LENGTH = 14;         //用户名的最大长度
//const unsigned int USER_PASSWORD_LENGTH = 14;         //用户密码的最大长度
//
//const unsigned int MAX_PERMISSION = 511;        //文件最大权限
//const unsigned int MAX_OWNER_PERMISSION = 448;        //用户最大权限M 


