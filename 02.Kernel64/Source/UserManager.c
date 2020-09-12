#include "UserManager.h"
#include "Console.h"
#include "Utility.h"
#include "FileSystem.h"
void kinitUserList()
{

    
    kReadAndMapping();

    uidcount = userlist[usercount - 1].uID;
   
    kSetCurUser(0);
}
void kWriteAndMapping()
{

    FILE *fp;
    int iEnterCount;
    BYTE bKey;

    char str[100];
    char foruid[10];
    char forflag[2];

    fp = fopen("userinfo.txt", "w");
    if (fp == NULL)
    {
        kPrintf("userinfo.txt File Open Fail\n");
        return;
    }
    for (int i = 0; i < 100; i++)
    {
        if (userlist[i].flag == TRUE)
        {
            StringClean(str, 100);
            kIToA(userlist[i].uID, foruid, 10);
            kIToA(userlist[i].flag, forflag, 10);
            kStrCat(str, foruid);
            kStrCat(str, ";");
            kStrCat(str, userlist[i].user_name);
            kStrCat(str, ";");
            kStrCat(str, userlist[i].user_passwd);
            kStrCat(str, ";");
            kStrCat(str, forflag);
            kStrCat(str, "\n");

            fwrite(str, sizeof(str), 1, fp);
        }
    }
    kPrintf("File Create Success\n");
    char temp[10] = "]\n";
    fwrite(temp, sizeof(temp), 1, fp);
    fclose(fp);
}
void kReadAndMapping()
{
    int iLength;
    FILE *fp;
    int i = 0;
    int index = 0;
    int iEnterCount = 0;
    char bKey;
    int tokenindex = 0;
    char string[1000];
    char string2[100][30];

    fp = fopen("userinfo.txt", "r");

    if (fp == NULL)
    {
        kPrintf("userinfo.txt File Open Fail\n");
        return;
    }

    while (1)
    {
        fread(&string[i], sizeof(string[i]), 1, fp);
       
        if (string[i] == ']')
        {
            
            break;
        }
        
        if (string[i] == '\n')
        {

         
            kStrCpy(string2[iEnterCount], string);

          
            iEnterCount++;
            StringClean(string, 100);
            i = 0;
            continue;
        }
        
        i++;
    }
  
    fclose(fp);
   
    int k = 0;
    char temp[30];
    for (int j = 0; j < iEnterCount; j++)
    {
        k = 0;
       
        for (int i = 0; i < 30; i++)
        {
            if (string2[j][i] != ';' && (string2[j][i] != '\n'))
            {
                temp[k] = string2[j][i];
            }
            if (string2[j][i] == ';')
            {
              
                if (tokenindex == 0)
                {
                    userlist[j].uID = kAToI(temp, 10);
                    StringClean(temp, 30);
                    tokenindex++;
                }
                else if (tokenindex == 1)
                {
                    kStrCpy(userlist[j].user_name, temp);
                    StringClean(temp, 30);
                    tokenindex++;
                }
                else if (tokenindex == 2)
                {
                    kStrCpy(userlist[j].user_passwd, temp);
                    StringClean(temp, 30);
                    tokenindex++;
                }
                k = 0;
                continue;
            }
            else if (string2[j][i] == '\n')
            {
                if (tokenindex == 3)
                {
                    userlist[j].flag = (BOOL)kAToI(temp, 10);
                    tokenindex = 0;
                    usercount++;
                }
                StringClean(temp, 30);
                break;
            }
            k++;
        }
    }

    kPrintf("userinfo.txt File Read Success\n");
}
void StringClean(char *str, int max)
{
    for (int k = 0; k < max; k++)
    {
        str[k] = '\0';
    }
}

char *kGetCurUserName()
{
    return cur_user.user_name;
}
char *kGetCurUserPw()
{
    return cur_user.user_passwd;
}
void kSetCurUser(int i)
{
    cur_user.uID = userlist[i].uID;
    kStrCpy(cur_user.user_name, userlist[i].user_name);
    kStrCpy(cur_user.user_passwd, userlist[i].user_passwd);
    cur_user.flag = userlist[i].flag;
}
BOOL kCmpPasswd(int index, char *tmp)
{
    if (kStrCmp(userlist[index].user_passwd, tmp) == 0)
    {
        return TRUE;
    }
    return FALSE;
}
void kPrintUserList()
{
    kPrintf("usercount : %d\n", usercount);
    for (int i = 0; i < 100; i++)
    {
        if (userlist[i].flag == TRUE)
        {
            kPrintf("uid:%d | User_name:%s | ", userlist[i].uID, userlist[i].user_name);
            kPrintf("pw:%s \n",userlist[i].user_passwd);            
         
        }
    }
}

void kADDUser(char *newid)
{
    for (int i = 0; i < 100; i++)
    {
        if (userlist[i].flag == FALSE)
        {

            userlist[i].uID = ++uidcount;
            kStrCpy(userlist[i].user_name, newid);
            kStrCpy(userlist[i].user_passwd, "00000");
            userlist[i].flag = TRUE;
            usercount++;
            break;
        }
    }
}
void kChangePasswd(int i, char *newpw)
{

    kStrCpy(userlist[i].user_passwd, newpw);
}

void kDeleteUserStruct(int i)
{
    userlist[i].flag = FALSE;
    userlist[i].uID = -1;
    kStrCpy(userlist[i].user_name, "");
    kStrCpy(userlist[i].user_passwd, "");
    usercount--;
}
int kFindUser(char *findid)
{
    for (int i = 0; i < 100; i++)
    {
        if (kStrCmp(userlist[i].user_name, findid) == 0)
        {
            return i;
        }
    }
    return -1;
}

int kFindUID(char *findid)
{
    for (int i = 0; i < 100; i++)
    {
        if (kStrCmp(userlist[i].user_name, findid) == 0)
        {
            return userlist[i].uID;
        }
    }
    return -1;
}

BOOL kCheckRedunduntUID(int uid)
{
    for (int i = 0; i < 100; i++)
    {
        if (userlist[i].uID == uid)
        {
            return TRUE;
        }
    }
    return FALSE;
}

void kChangeuid(int old, int new)
{
    for (int i = 0; i < 100; i++)
    {
        if (userlist[i].uID == old)
        {
            userlist[i].uID = new;
            return;
        }
    }
    return;
}