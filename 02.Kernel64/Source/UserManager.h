#include "Types.h" 
typedef struct Userstruct
{
    int uID;
    char user_name[30];

    char user_passwd[30]; 

    BOOL flag;
} User;


static User  userlist[100];
static User  cur_user;
static int usercount;
static int uidcount;


void kinitUserList();
void kPrintUserList();
void kADDUser(char * newid);
int kFindUser(char * findid);
void kChangePasswd(int i,char * newpw);
void kDeleteUserStruct(int index);
void kSetCurUser(int i);
char* kGetCurUserName();
char* kGetCurUserPw();

int kFindUID(char *findid);
BOOL kCheckRedunduntUID(int uid);
void kChangeuid(int old,int new);
void kReadAndMapping();
void kWriteAndMapping();
void StingClean(char * str,int max);