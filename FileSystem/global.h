#ifndef  _GLOBAL_H
#define _GLOBAL_H
#define BLKSIZE 512 //数据块大小
#define BLKNUM 512 //数据块数目
#define INODESIZE 32 //i节点大小
#define INODENUM 32 //i节点数目
#define NBLOCKS 2 //文件所占数据块数
#define MAXLEN 10 //文件名最大长度
#define DIRNUM 10 //目录项数
#define FBLNUM 50 //空闲块堆栈深度
#define FINNUM 50 //空闲i节点数组宽度
#define UNAMELEN 10 //用户名长度
#define UPWDLEN 10 //用户密码长度
#define DNAMELEN 10 //目录名长度
#define FILENUM 10 //打开文件数
#define SYSOFILE 10 //系统打开表长度
#define USERNUM 8 //用户数量

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

/**
 * \brief 超级块
 */
typedef struct
{
	//unsigned short s_size; //数据块所占盘块数
	unsigned int s_inode_size; //inode表所占盘块数

	//unsigned short s_inodes_count; //i节点总数
	//unsigned short s_blocks_count; //盘块总数

	unsigned int s_free_blocks_count; //空闲数据块块数
	unsigned short s_next_free_block; //空闲数据块指针
	unsigned int s_free_blocks_stack[FBLNUM]; //空闲数据块堆栈

	unsigned int s_free_inodes_count; //空闲i节点数量
	unsigned short s_next_free_inode; //空闲i节点指针
	unsigned int s_free_inodes_stack[FINNUM]; //空闲i节点数组

	unsigned short *s_rinode;

	//unsigned short s_first_data_block; //第一个数据块地址
	//unsigned short s_block_size; //数据块大小
	//unsigned short s_mtime; //挂载时间
	//unsigned short s_state; //文件系统状态

	char s_flag; //修改标志
}SuperBlock;

/**
 * \brief 磁盘i节点
 */
typedef struct
{
	//short fi_id; //i节点号
	unsigned short fi_state; //占用情况
	unsigned short fi_mode; //文件类型：目录/普通文件
	unsigned short fi_mtime; //上一次文件修改时间
	unsigned short fi_ctime; //上一次inode修改时间
	unsigned short fi_atime; //上一次文件访问时间
	unsigned short fi_uid; //所属用户id
	unsigned short fi_gid; //所属用户组id
	unsigned int fi_size; //文件字节数
	unsigned int fi_block[NBLOCKS]; //数据block的地址指针数组
	unsigned short fi_links_count; //链接数，当为0时意味着被删除
	char fi_acl[3]; //存取权限
}Finode;

/**
 * \brief 内存i节点
 * \details 内存索引结点是保存在内存中索引结点的数据结构，
 * 当文件第一次被打开时，文件的索引结点从模拟磁盘上读出，
 * 并保存在内存中，方便下一次文件的打开。 
 */
typedef struct inode
{
	unsigned short i_ino; //索引结点号
	char i_flag; //i节点状态
	unsigned short i_count; //引用计数
	Finode finode; //磁盘i结点结构，保存从磁盘读出的i结点信息 
	struct inode *prevptr; //上一个内存i结点指针
	struct inode *nextptr; //下一个内存i结点指针
}Inode;

/**
 * \brief 用户
 */
typedef struct
{
	unsigned short u_uid; //用户id
	unsigned short u_gid; //用户组id
	char u_name[UNAMELEN]; //用户名
	char u_password[UPWDLEN]; //密码
}User;

/**
 * \brief 目录项
 * \details 文件目录项由文件名和文件索引结点号组成
 */
typedef struct
{
	char d_name[DNAMELEN]; //文件或目录名
	unsigned int d_ino; //i节点号
}Dentry;

/**
* \brief 目录
* \details 对于目录类，它的内容都是以dir结构保存在磁盘中的，并以dir结构读取
*/
typedef struct
{
	Dentry dentry[DIRNUM]; //目录项数组
	unsigned short size; //目录项数目
}Dir;

/**
 * \brief 系统打开表
 * \details 当文件被打开时创建
 */
typedef struct
{
	char sf_flag; //文件操作标志
	unsigned short sf_count; //引用计数
	inode *sf_inode; //指向文件的内存i节点
	unsigned int sf_curpos; //当前读写指针
}Sys_OFile;

/**
 * \brief 用户打开表
 */
typedef struct
{
	unsigned char uf_default_acl[3]; //默认存取权限
	unsigned short uf_uid; //用户id
	unsigned short uf_gid; //用户组id
	unsigned short uf_ofile[FILENUM]; //用户打开文件表
}User_OFile;

//全局变量
extern Dir dire;
extern Sys_OFile sys_ofile;
extern SuperBlock superblk;
extern User users[USERNUM];
extern FILE *fp;
extern Inode *cur_path_inode;
extern unsigned short user_id;
extern unsigned int file_block;


//申明函数
extern void login(void);
extern void init(void);
extern int  analyse(char *);
extern void save_inode(int);
extern int	get_blknum(void);
extern void read_blk(int);
extern void write_blk(int);
extern void release_blk(int);
extern void pathset();
extern void delet(int innum);
extern int  check(int i);

//用户命令处理函数
extern void help(void);
extern void cd(void);
extern void dir(void);
extern void mkdir(void);
extern void creat(void);
extern void open(void);
extern void read(void);
extern void write(void);
extern void close(void);
extern void del(void);
extern void logout(void);
extern void command(void);
extern void rd();
extern void quit();



#endif