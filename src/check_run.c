#include "check_run.h"
#include <stdio.h>
#include <string.h>
#include "log.h"

void check_run(void)
{
    unsigned int count = 0,pid_count = 0;
    char search_flag = 0;
    char cmd_str[4096];
    char recv_str[4096];
    char pid_str[9];
    FILE *fp;  

    LOG("The process ID is %d\r\n", (int) getpid ()) ; //本进程
    sprintf(cmd_str,"ps | grep ATouchService | grep -v '%d'",(int)getpid());
    LOG("%s\r\n", cmd_str) ;

    fp=popen(cmd_str, "r"); 
    fgets(recv_str,sizeof(recv_str),fp); 
    LOG("%s",recv_str); 

    //为兼容米8Lite
    if(strlen(recv_str) <= 2)
    {

        sprintf(cmd_str,"ps -A | grep ATouchService | grep -v '%d'",(int)getpid());
        LOG("%s\r\n", cmd_str) ;

        fp=popen(cmd_str, "r"); 
        fgets(recv_str,sizeof(recv_str),fp); 
        LOG("%s",recv_str); 
    }

    if(strlen(recv_str) > 2 )
    {
        log_byte(recv_str,10);

        for(;count < strlen(recv_str);count++)
        {
            switch(search_flag)
            {
                case 0:
                if(recv_str[count] == 0x20)
                {
                    search_flag = 1;
                }
                break;
                case 1:
                if(recv_str[count] >= '0' && recv_str[count] <= '9')
                {
                    pid_str[pid_count++] = recv_str[count];
                    search_flag = 2;
                }
                break;
                case 2:
                if(recv_str[count] >= '0' && recv_str[count] <= '9')
                {
                    pid_str[pid_count++] = recv_str[count];
                }else{
                    pid_str[pid_count++] = '\0';
                    search_flag = 3;
                    count = strlen(recv_str);
                }
                break;
                default:
                break;            
            }
        }

        if(search_flag == 3)
        {
            LOG("found pid:%s\r\n", pid_str) ;
            sprintf(cmd_str,"kill -s 9 %s",pid_str);
            LOG(cmd_str);
            LOG("\r\n");
            system(cmd_str);
        }else{
            LOG("not found pid\r\n") ;
        }
    }else{
        LOG("not runing\r\n") ;
    }

    pclose(fp);

}