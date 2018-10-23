#define FUSE_USE_VERSION 26
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fuse.h>
#include <sys/mman.h>

struct filenode                         //�ļ��ڵ����������ʽ�洢
{
    char *filename;
    void *content;
    struct stat *st;
    struct filenode *next;
};

static const size_t size = 4 * 1024 * 1024 * (size_t)1024;      //��ʾ��С���ܹ�1G)
static void *mem[64 * 1024];                                    //mem��ʾһ��ָ�����飬����ÿ��block����Ӧ�ĵ�ָ��

static struct filenode *root = NULL;

static struct filenode *get_filenode(const char *name)          //Ѱ�Һ�name����һ�µ��ļ��ڵ㣬������return���Ҳ��������ؿ�
//��fileattr�е���
{
    struct filenode *node = root;
    while(node)
    {
        if(strcmp(node->filename, name + 1) != 0)
            node = node->next;
        else
            return node;
    }
    return NULL;
}

static void create_filenode(const char *filename, const struct stat *st)        //����һ���µ��ļ��ڵ㣨��mknod�е��ã�
{
    struct filenode *new = (struct filenode *)malloc(sizeof(struct filenode));
    //Ϊ�����ɵ��ļ��ڵ����ռ�
    new->filename = (char *)malloc(strlen(filename) + 1);
    //Ϊfilename����ռ䣺
    //Ϊʲô�����ǳ��ȼ�һ��
    memcpy(new->filename, filename, strlen(filename) + 1);
    //��filenamecopy����Ӧ�Ľڵ�����
    new->st = (struct stat *)malloc(sizeof(struct stat));
    //�½ڵ���ļ����Խṹ�����ռ�
    memcpy(new->st, st, sizeof(struct stat));
    //��st������copy���ṹ��Ľڵ�����
    new->next = root;
    //��ǰ��ƴ��
    new->content = NULL;
    root = new;
}

static void *oshfs_init(struct fuse_conn_info *conn)
{
    size_t blocknr = sizeof(mem) / sizeof(mem[0]);  //blocknr=64*1024
                                                    //mem�Ĵ�СΪ64*1024��size�Ĵ�СΪ4 * 1024 * 1024 * (size_t)1024 ��1G)
    size_t blocksize = size / blocknr;              //blocksize����mem��ÿ��ָ���Ӧ������Ĵ�С ��ÿһ���Ĵ�С��64kb��
    // Demo 1 ��һ��ʵ�ַ�ʽ
    for(int i = 0; i < blocknr; i++)
    {
        mem[i] = mmap(NULL, blocksize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        //���ڴ��н���ӳ�䣬��ϵͳ������ʼ�ռ䣬����Ϊһ��blocksize���趨ģʽΪҳ���ݿ��Ա���д��һ��ӳ��64kb
        //����˽��ӳ���������ӳ��
        //�����趨��������ӳ�䣬�����ļ��������趨Ϊ1
        //��ӳ���������Ϊ0
        memset(mem[i], 0, blocksize);
        //��ÿ��mem��ָ��������ֵȫ����Ϊ0
    }
    //�˴��൱������˴洢�ռ�ĳ�ʼ��
    for(int i = 0; i < blocknr; i++)
    {
        munmap(mem[i], blocksize);
    }
    //����ȡ����Ӧ���ڴ�ӳ��

    // Demo 2   �ڶ���ʵ�ַ�ʽ
    mem[0] = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    //��mem[0]����ռ�
    for(int i = 0; i < blocknr; i++)
    {
        mem[i] = (char *)mem[0] + blocksize * i;
        memset(mem[i], 0, blocksize);
    }
    //ÿһ��mem����ʼ��ַ������
    for(int i = 0; i < blocknr; i++)
        munmap(mem[i], blocksize);
    //ȡ����Ӧ���ڴ�ӳ��
    return NULL;
}

static int oshfs_getattr(const char *path, struct stat *stbuf)
{
    int ret = 0;
    struct filenode *node = get_filenode(path);         //����get_filenode������Ѱ����·��һ�µ��ļ��ڵ�
    if(strcmp(path, "/") == 0)                          //????????
    {
        memset(stbuf, 0, sizeof(struct stat));
        stbuf->st_mode = S_IFDIR | 0755;                //�������е��ļ�����ģʽ�趨ΪĿ¼
    }
    else if(node)                                       //����ڵ���ڣ���ô�Ͱѽڵ���ļ�����copy�����Ի�������
        memcpy(stbuf, node->st, sizeof(struct stat));
    else                                                //�������ڣ���ô�ͷ��ش���
        ret = -ENOENT;
    return ret;
}

static int oshfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
//���������ļ�����Ϣ
{
    struct filenode *node = root;
    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);
    while(node)
    {                                       //���ζ���ÿ���ļ�����Ϣ
        filler(buf, node->filename, node->st, 0);
        node = node->next;
    }
    return 0;
}

static int oshfs_mknod(const char *path, mode_t mode, dev_t dev)            //����һ���ڵ�
{
    struct stat st;                                                         //����һ��״̬�ṹ��
    st.st_mode = S_IFREG | 0644;                                            //����ģʽ����Ϊ��ͨ�ļ�
    st.st_uid = fuse_get_context()->uid;
    st.st_gid = fuse_get_context()->gid;
    st.st_nlink = 1;                                                        //Ӳ���Ӹ�������Ϊ1��
    st.st_size = 0;                                                         //��ʼ���ļ���СΪ0
    create_filenode(path + 1, &st);                                         //�����˴����ļ��ڵ�ĺ���������·���Ͷ����st�ʹ���ȥ
    return 0;
}

static int oshfs_open(const char *path, struct fuse_file_info *fi)          //���ļ�
{
    return 0;
}

static int oshfs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
//����������buf���е�����д��һ���򿪵��ļ���
{
    struct filenode *node = get_filenode(path);                     //���ļ�
    node->st->st_size = offset + size;                              //�޸��ļ��Ĵ�С��־
    node->content = realloc(node->content, offset + size);          //����Ϊ�ļ����ݷ���ռ�
    memcpy(node->content + offset, buf, size);                  //���������ڵ�����copy���µĿռ���
    return size;
}

static int oshfs_truncate(const char *path, off_t size)     //�����޸��ļ��Ĵ�С
{
    struct filenode *node = get_filenode(path);             //���ļ�
    node->st->st_size = size;                               //�ļ��Ĵ�С�趨Ϊsize��С
    node->content = realloc(node->content, size);           //����Ϊ���ݷ���һ����СΪsize�Ŀռ�
    return 0;
}

static int oshfs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
//��һ���Ѿ��򿪵��ļ��ж�������
{
    struct filenode *node = get_filenode(path);                     //Ѱ�Ҷ�Ӧ�Ľڵ�
    int ret = size;                                                 //ret��ֵΪ��ȡ���ݵĴ�С
    if(offset + size > node->st->st_size)                           //�����ȡ�����ݵĴ�С�������ļ����ܴ�С
        ret = node->st->st_size - offset;                           //��ôʵ�ʶ�ȡ����ֵ������Ϊ�ļ���С��ȥƫ����
    memcpy(buf, node->content + offset, ret);                       //���ļ�����offset��ʼ������copy����������
    return ret;                                                     //���ض�ȡ���ݵĴ�С
}

static int oshfs_unlink(const char *path)               //����ɾ��һ���ڵ�
{
    struct filenode *node1 = get_filenode(path);
    struct filenode *node2 = root;
    if (node1==root)                        //���⴦���ļ�Ϊ����ͷ�����
    {
        root=node1->next;
        node1->next=NULL;
    }
    else if (node1)                         //��node1����
    {
            while(node2->next!=node1&&node2!=NULL)
                    node2 = node2->next;
            node2->next=node1->next;
            node1->next=NULL;
    }
    else return 0;
    //��ʼɾ��node1�ڵ�

}

static const struct fuse_operations op = {              //��ͬ��op����Ӧ�ĺ���
    .init = oshfs_init,
    .getattr = oshfs_getattr,
    .readdir = oshfs_readdir,
    .mknod = oshfs_mknod,
    .open = oshfs_open,
    .write = oshfs_write,
    .truncate = oshfs_truncate,
    .read = oshfs_read,
    .unlink = oshfs_unlink,
};

int main(int argc, char *argv[])
{
    return fuse_main(argc, argv, &op, NULL);            //����fuse����
}
