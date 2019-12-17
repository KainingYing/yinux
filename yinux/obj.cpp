#pragma once
#include "obj.h"
#include <iostream>
#include <stdio.h>
#include <time.h>

using namespace std;



yinux::yinux()
{
	if ((file=fopen("DISK", "rb+")) == NULL)
	{
		cout << "Can't find file system，reinstall the file system now!" << endl;
		format();
	}
	readDisk();
	login();//after login we get the user information

}

bool yinux::format()
{
	if ((file = fopen("DISK", "wb+")) == NULL)
	{
		cout << "Sorry,format error!" << endl;
		return false;
	}
	else
	{
		//step1       write (and read) superblock
		fseek(file, BLOCK_SIZE, 0);     //the first block is empty
		fwrite(&Superblock, sizeof(superblock), 1, file);
		fseek(file, BLOCK_SIZE, 0);
		fread(&Superblock, sizeof(superblock), 1, file);
		//step end



		//step2       initial the block stack

		unsigned short stack[51];
		for (int i = 0; i < BLOCK_NUM/50; i++)
		{
			memset(stack, 0, sizeof(stack));
			for (int j = 1; j < 50; j++)
			{
				stack[50-j] = 50*i+j;
			}
			stack[50] = 49;//first is used to store stack in every 50 groups
			fseek(file, DATA_START + 50 * i*BLOCK_SIZE, 0);
			fwrite(stack, sizeof(stack), 1, file);
		}

		//first now to use first 50 group
		for (int i = 1; i <= 49; i++)
			Superblock.currentStack[i] = 50 - i;
		Superblock.currentStack[50] = 49;
		stackPtr = 0;

		/*fseek(file, DATA_START + 50 * 10*BLOCK_SIZE, 0);
		fwrite(stack, sizeof(stack), 1, file);
		for (int i = 0; i < 51; i++)
			cout << stack[i] << " ";
		cout << endl;*/
		
		//reamin 12 blocks 
		for (int i = 0; i < 12; i++)
		{
			stack[49 - i] = 511 - i;
		}
		stack[50] = 12;
		fseek(file, DATA_START + 500*BLOCK_SIZE, 0);
		fwrite(stack, sizeof(stack), 1, file);
		//step2 end

		//step3 create root directory
		inode inode_temp;
		inode_temp.inodeNum = 0;
		inode_temp.dirMode = 0;  //o represent directory
		inode_temp.dirSize = 24;
		inode_temp.dirPermission = MAX_OWNER_PERMISSION;
		inode_temp.dirGroup = 1;
		inode_temp.dirOwner = 0; //root user id = 0
		inode_temp.dirConnect = 0;
		time_t t = time(0);
		strftime(inode_temp.dirTime, sizeof(inode_temp.dirTime), "%Y/%m/%d %X %A %jday %z", localtime(&t));
		fseek(file, INODE_START, 0);
		fwrite(&inode_temp, sizeof(inode), 1, file);
		Superblock.inodeBit[inode_temp.inodeNum] = 1;
		Superblock.inodeFree--;
		fseek(file, BLOCK_SIZE, 0);
		fwrite(&Superblock, sizeof(superblock), 1, file);

		//step3 end


		//step4 create root directory

		directory droot_tmp;
		droot_tmp.fname[0] ='/';
		droot_tmp.index = 0;
		droot_tmp.isdir = 1;
		droot_tmp.content = "";
		droot_tmp.parent = -1;//-1 represent null
		droot_tmp.firstChild = -1;//-1 represent null
		droot_tmp.nextSibling = -1;
		allocateSpace(0, &droot_tmp);
		writeDirectory(0,&droot_tmp);
		//cout << Superblock.currentStack[50] << endl;
		
		//step4 end

		//step5 create and store usr file

		user* USER = new user[10];
		USER[0].userId = 0;
		strcpy(USER[0].group, "admin");
		strcpy(USER[0].userName, "admin");
		strcpy(USER[0].password, "admin");

		USER[1].userId = 1;
		strcpy(USER[1].group, "guest");
		strcpy(USER[1].userName, "guest");
		strcpy(USER[1].password, "guest");
		
		fseek(file, DATA_START + BLOCK_SIZE, 0);
		fwrite(USER, sizeof(user), 10, file);

		Superblock.userNum = 2;//a admin and a guest
		Superblock.blockFree--;
		Superblock.currentStack[50]--;
		fseek(file, BLOCK_SIZE, 0);
		fwrite(&Superblock, sizeof(superblock), 1, file);
		//step5 end
		return true;
		//format is done!!
		//Failure is the mother of success. - Thomas Paine  没错我就是这么有意境！！

		/*fseek(file, DATA_START, 0);
		fwrite(&droot_tmp, sizeof(directory), 1, file);*/
	}
}

bool yinux::readDisk()
{
	if ((file = fopen("DISK", "rb+")) == NULL)
	{
		cout << "Read the DISK error!" << endl;
		return false;
	}
	else
	{
		//step1 read superblockf
		fseek(file, BLOCK_SIZE, 0);
		fread(&Superblock, sizeof(superblock), 1, file);
		//cout << Superblock.userNum << endl;
		//cout << "meme" << endl;
		//step1 end

		//step2 read root
		inode* inode_root = readInode(0);
		currDirectory = readDirectory(0);
		//step2 end



		//Nothing seek, nothing find.
		return true;
	}
	
}

bool yinux::login()
{
	int m = 1;
	while (m)
	{
		int flag = 0;
		int t;
		cout << "Try not to become a man of success but rather try to become a man of value. -- A. Einstein" << endl;
		cout << "Please enter username:";
		char username[6], password[6];
		cin >> username;
		cout << "Please enter correspond password:";
		cin >> password;

		user* USER = new user[10];
		fseek(file, DATA_START + BLOCK_SIZE, 0);
		fread(USER, sizeof(user), 10, file);
		//cout << USER[0].userName << endl;
		//cout << "10000hours" << endl;
		for (int i = 0; i < 10; i++)
			if (strcmp(username, USER[i].userName) == 0)
			{
				t = i;
				flag = 1;
				break;
			}
		if (flag)
		{
			if (strcmp(password, USER[t].password) == 0)
			{
				system("cls");
				m = 0;//login successful!
				User = USER[t];
				return true;
			}
			else
			{
				cout << "Account and password do not match!Please try again!" << endl;
			}
		}
		else
		{
			cout << "Account dose not exist！Please try aagin" << endl;
		}
	}
}

void yinux::allocateSpace(unsigned short inodeNum, directory * dir)
{
	if (Superblock.currentStack[50] == 0)
	{
		//current stack is empty
		int stack[51];
		fseek(file, DATA_START + stackPtr * 50 * BLOCK_SIZE, 0);
		fwrite(Superblock.currentStack, sizeof(Superblock.currentStack), 1, file);

		for (int i = 0; i < BLOCK_NUM / 50 + 1; i++)
		{
			fseek(file, DATA_START + i * 50 * BLOCK_SIZE, 0);
			fread(stack, sizeof(stack), 1, file);
			if (stack[50] != 0)
			{
				for (int i = 0; i <= 50; i++)
					Superblock.currentStack[i] = stack[i];
				break;
			}
		}
	}
	inode* newInode = readInode(inodeNum);
	newInode->dirAddr[0] = Superblock.currentStack[Superblock.currentStack[50]--];
	writeInode(inodeNum, newInode);
	Superblock.blockFree--;
	fseek(file, BLOCK_SIZE, 0);
	fwrite(&Superblock, sizeof(superblock), 1, file);

}

inode * yinux::readInode(unsigned short inodeNum)
{
	inode* newInode = new inode();
	if (fseek(file, INODE_START + inodeNum * INODE_SIZE,0))
	{
		cout << "File point error!!" << endl;
		exit(1);
	}
	fread(&(newInode->inodeNum), sizeof(unsigned short), 1, file);
	fread(&(newInode->dirSize), sizeof(unsigned short), 1, file);
	fread(&(newInode->dirConnect), sizeof(unsigned short), 1, file);
	fread(newInode->dirAddr, sizeof(unsigned int), 4, file);
	fread(&(newInode->dirAddr1), sizeof(unsigned short), 1, file);
	fread(&(newInode->dirAddr2), sizeof(unsigned short), 1, file);
	fread(&(newInode->dirOwner), sizeof(unsigned short), 1, file);
	fread(&(newInode->dirGroup), sizeof(unsigned short), 1, file);
	fread(&(newInode->dirMode), sizeof(unsigned short), 1, file);
	fread(&(newInode->dirPermission), sizeof(unsigned short), 1, file);
	fread(newInode->dirTime, sizeof(char), 64, file);
	return newInode;
}

void yinux::writeInode(unsigned short inodeNum, inode* newInode)
{
	if (fseek(file, INODE_START + inodeNum * INODE_SIZE, 0))
	{
		cout << "File point error!!" << endl;
		exit(1);
	}
	fwrite(&(newInode->inodeNum), sizeof(unsigned short), 1, file);
	fwrite(&(newInode->dirSize), sizeof(unsigned short), 1, file);
	fwrite(&(newInode->dirConnect), sizeof(unsigned short), 1, file);
	fwrite(newInode->dirAddr, sizeof(unsigned int), 4, file);
	fwrite(&(newInode->dirAddr1), sizeof(unsigned short), 1, file);
	fwrite(&(newInode->dirAddr2), sizeof(unsigned short), 1, file);
	fwrite(&(newInode->dirOwner), sizeof(unsigned short), 1, file);
	fwrite(&(newInode->dirGroup), sizeof(unsigned short), 1, file);
	fwrite(&(newInode->dirMode), sizeof(unsigned short), 1, file);
	fwrite(&(newInode->dirPermission), sizeof(unsigned short), 1, file);
	fwrite(newInode->dirTime, sizeof(char), 64, file);
}

void yinux::writeDirectory(unsigned short inodeNum, directory* dir)
{
	inode* inode_temp = readInode(inodeNum);
	//no consider one block cant store
	fseek(file, DATA_START + inode_temp->dirAddr[0], 0);
	fwrite(dir->fname, sizeof(char), 20, file);
	fwrite(&(dir->index), sizeof(unsigned short), 1, file);
	fwrite(&(dir->isdir), sizeof(unsigned short), 1, file);
	fwrite(&(dir->parent), sizeof(unsigned short), 1, file);
	fwrite(&(dir->firstChild), sizeof(unsigned short), 1, file);
	fwrite(&(dir->nextSibling), sizeof(unsigned short), 1, file);
	if (!dir->isdir)//dir is not directory ,then store its content
	{
		int size = inode_temp->dirSize-24; //24 represent dir size without content
		char* a = new char[size];
		for (int i = 0; i < inode_temp->dirSize - 24; i++)
			a[i] = dir->content[i];
		fwrite(a, sizeof(char), inode_temp->dirSize - 24, file);
		delete[]a;
	}
}

directory* yinux::readDirectory(unsigned short inodeNum)
{
	inode* inode_temp = readInode(inodeNum);
	directory* dir = new directory();
	fseek(file, DATA_START + inode_temp->dirAddr[0], 0);
	fread(dir->fname, sizeof(char), 20, file);
	fread(&(dir->index), sizeof(unsigned short), 1, file);
	fread(&(dir->isdir), sizeof(unsigned short), 1, file);
	fread(&(dir->parent), sizeof(unsigned short), 1, file);
	fread(&(dir->firstChild), sizeof(unsigned short), 1, file);
	fread(&(dir->nextSibling), sizeof(unsigned short), 1, file);
	if (!dir->isdir)//dir is not directory ,then store its content
	{
		int size = inode_temp->dirSize - 24; //24 represent dir size without content
		char* a = new char[size];
		fread(a, sizeof(char), size, file);
		for (int i = 0; i < inode_temp->dirSize - 24; i++)
			dir->content += a[i];
		delete[]a;
	}
	return dir;

}

superblock::superblock()
{
	inodeNum = INODE_NUM;
	inodeFree = INODE_NUM;
	inodeSize = INODE_SIZE;
	for (int i = 0; i < INODE_NUM; i++)
		inodeBit[i] = 0;
	blockNum = BLOCK_NUM+INODE_NUM*INODE_SIZE/BLOCK_SIZE+2;//2 means one empty and one superblock
	blockFree = BLOCK_NUM;
}
