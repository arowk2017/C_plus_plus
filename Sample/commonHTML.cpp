
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/utsname.h>
#include "commonHTML.h"

#include "cgiparse_new.h"
#include "comemctl.h"      /* needed for global shared memory */
#include "comstuff.h"      /* needed for global shared memory */
#include "simcom.h"
#include "header.h"

int compString(std::string firstString, std::string secondString)
{
    int firstNumber = -1;
    int secondNumber = -1;
    unsigned int x;

    for (x=0;x < firstString.length() && x < secondString.length();x++)
    {

        if (isalpha(firstString[x]) )
        {
            if (isalpha(secondString[x]))
            {
                if (secondNumber > -1)
                {
                    if (firstNumber > secondNumber)
                    {
                        return GREATER_THEN;
                    }
                    else if (firstNumber < secondNumber)
                    {
                        return LESS_THEN;
                    }
                    else
                    {
                        firstNumber = -1;
                        secondNumber = -1;
                    }

                }
                if (firstString[x] == secondString[x])
                {
                    //return EQUAL;
                }
                else if (firstString[x] > secondString[x])
                {
                    return GREATER_THEN;
                }
                else
                {
                    return LESS_THEN;
                }
            }
            else
            {
               
                if (isdigit(secondString[x]))
                {
                    if (secondNumber > -1)
                    {
                        return LESS_THEN;
                    }
                    else
                    {
                        return GREATER_THEN;
                    }
                }
                else if (secondNumber > firstNumber)
                {
                    return LESS_THEN;
                }
                else
                {
                    return GREATER_THEN;
                }

            }
        }
        else if (isdigit(firstString[x]))
        {
            if (!isdigit(secondString[x]) && secondNumber == -1)
            {
                return LESS_THEN;
            }
            else if (!isdigit(secondString[x]) && secondNumber > -1)
            {
                return GREATER_THEN;
            }
            else
            {
                if (secondNumber == -1)
                {
                    secondNumber = 0;
                    firstNumber = 0;
                }
                secondNumber = secondNumber * 10 + (secondString[x] - 48);
                firstNumber = firstNumber * 10 + (firstString[x] - 48);
            }
        }
        else
        {
            if (!isdigit(secondString[x]) && secondNumber != -1)
            {
                if (firstNumber > secondNumber)
                {
                    return GREATER_THEN;
                }
                else if (firstNumber < secondNumber)
                {
                    return LESS_THEN;
                }
                else
                {
                    firstNumber = -1;
                    secondNumber = -1;
                }
            }
            if (isalnum(secondString[x]))
            {
                return LESS_THEN;
            }
        }
    }
    if (secondNumber != -1)
    {
        if (x < firstString.size() && isdigit(firstString[x]))
        {
            return GREATER_THEN;
        }
        else if (x < secondString.size() && isdigit(secondString[x]))
        {
            return LESS_THEN;
        }


        if (firstNumber > secondNumber)
        {
            return GREATER_THEN;
        }
        else if (firstNumber < secondNumber)
        {
            return LESS_THEN;
        }
        else
        {
            firstNumber = -1;
            secondNumber = -1;
        }
    }

    return EQUAL;

}


void htmlFooter()
{
    printf("</td><td width='50%%'></td></tr></table> \n");
    printf("</body>\n");
    printf("</html>\n\n");

}


void htmlHeader(){

    printf("<html>\n\n");
    printf("<head>\n");

    printf("<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">\n");
    printf("<title>Configuration</title>\n");
    printf("<LINK REL=StyleSheet HREF=\"../style.css\" TYPE=\"text/css\">");

    printf("</head>\n\n");
    printf("<body>\n");

    printf("<table><tr><td width='50%%'></td>\n");
    printf("<td><center>%s</center></td>\n", printHeader().c_str());
    printf("<td></td>\n");
    printf("</tr>\n");
    printf("<td></td>\n");
    printf("<td>\n");
    printf("<br><br>\n");



}

void htmlHeader2(char * titleP, char * metaDataP)
{
    printf("<html>\n\n");
    printf("<head>\n");

    printf("<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">\n");
    //append additional meta data
    if (metaDataP != NULL)
    {
        printf("%s\n", metaDataP);
    }
    printf("<title>%s</title>\n", titleP);
    printf("<LINK REL=StyleSheet HREF=\"../style.css\" TYPE=\"text/css\">");

    printf("</head>\n\n");
    printf("<body>\n");

    printf("<table><tr><td width='50%%'></td>\n");
    printf("<td><center>%s</center></td>\n", printHeader().c_str());
    printf("<td width='50%%'></td>\n");
    printf("</tr>\n");
    printf("<td></td>\n");
    printf("<td>\n");
    printf("<br><br>\n");

}

int getLoginTimeout()
{
    FILE *iniFile = NULL;
    char buffer[1024];
    char *paramName;
    char *value;
    int timeout = 1200;
    iniFile  = fopen("/home/dcg/web.ini", "r");

    if (iniFile == NULL)
        return timeout;

    while (fgets(buffer, 1024, iniFile))
    {
        paramName = strtok(buffer, "=");
        if (strstr(paramName, "USER_TIMEOUT") != NULL)
        {
            value = strtok(NULL, "=");
            timeout = atoi(value);
        }

    }

    fclose(iniFile);
    return timeout;

}

void htmlHeaderRedirect(char* redirectPage){

    printf("<html>\n\n");
    printf("<head>\n");
    printf("<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">\n");

    printf("<title>Configuration</title>\n");
    printf("<meta http-equiv=\"refresh\" content=\"4; url=%s\">\n",redirectPage);
    printf("<LINK REL=StyleSheet HREF=\"../style.css\" TYPE=\"text/css\">");
    printf("</head>\n\n");
    printf("<body>\n");

    printf("<table><tr><td width='50%%'></td>\n");
    printf("<td><center>%s</center></td>\n", printHeader().c_str());
    printf("<td></td>\n");
    printf("</tr>\n");
    printf("<td></td>\n");
    printf("<td>\n");
    printf("<br><br>\n");
}

void htmlHeaderRedirect2(char* redirectPage, char *titleP, int delayTime, int showImg)
{
    printf("<html>\n\n");
    printf("<head>\n");
    printf("<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">\n");

    printf("<title>%s</title>\n", titleP);
    printf("<meta http-equiv=\"refresh\" content=\"%d; url=%s\">\n",delayTime, redirectPage);
    printf("<LINK REL=StyleSheet HREF=\"../style.css\" TYPE=\"text/css\">");
    printf("</head>\n\n");
    printf("<body>\n");
    if (showImg)
    {

        printf("<table><tr><td width='50%%'></td>\n");
        printf("<td><center>%s</center></td>\n", printHeader().c_str());
        printf("<td></td>\n");
        printf("</tr>\n");
        printf("<td></td>\n");
        printf("<td>\n");
        printf("<br><br>\n");


    }

}

void htmlHeaderRedirectFast(char* redirectPage){

    printf("<html>\n\n");
    printf("<head>\n");
    printf("<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">\n");
    printf("<title>Configuration</title>\n");
    printf("<meta http-equiv=\"refresh\" content=\"1; url=%s\">\n",redirectPage);
    printf("<LINK REL=StyleSheet HREF=\"../style.css\" TYPE=\"text/css\">");
    printf("</head>\n\n");
    printf("<body>\n");
    
}


void redirectToLoginPage(const char *msgP, const char *pageP)
{
    char tempS[128];
    //redirect to login screen
    sprintf(tempS, "./login.cgi?page=%s", pageP);
    htmlHeaderRedirect2(tempS, "Login", 1, 1);
    printf("<center><p><b><big><font color=red>%s</font></big></b></p></center>", msgP);
    htmlFooter();
}


int checkLogin(const char *httpCookieP, const char *pageP, char *html)
{
    struct url_parameter_type *up_p;
    //const char *tempP;
    char *userNameP;
    BoolT foundUserB=FALSE;
    dcgGlobalT *dcgGsmGP;

    if (httpCookieP == NULL)
    {
        //redirect to login screen
        if (html == NULL)
        {
            redirectToLoginPage("Please login first.", pageP);
        }
        else
        {
            printf("%s", html);
        }

        return 1;
    }
    else
    {
        //parse user name
        up_p = url_parameter_type_init(httpCookieP, ';');
        userNameP = (char*) url_parameter_get_data(up_p, "username", NULL);
        //check to make sure username is set
        if (userNameP == NULL)
        {
            //redirect to login screen
            if (html == NULL)
            {
                redirectToLoginPage("Please login first.", pageP);
            }
            else
            {
                printf("%s", html);
            }
            return 1;
        }

        dcgGsmGP = (dcgGlobalT *) ShMemAttach (DCG_SHMEM_NUM);
        if (!dcgGsmGP)
        {
            //exit
            printf ("Software is not running\n");
            url_parameter_type_done(up_p);
            return 1;
        }
        for (int i=0;i<4;i++)
        {
            if (dcgGsmGP->userLoginInfo[i].userName[0] != '\0')
            {
                if (!strcmp(dcgGsmGP->userLoginInfo[i].userName, userNameP))
                {
                    //found this user
                    foundUserB = TRUE;
                    //update timeout counter
                    dcgGsmGP->userLoginInfo[i].timeOutCounter = getLoginTimeout();
                    break;
                }
            }
        }
        url_parameter_type_done(up_p);
    }

    if (!foundUserB)
    {
        if (html != NULL)
        {
            //redirect to login page
            htmlHeader2("Login");
            //delete (expire) existing username cookie
            printf("<META HTTP-EQUIV=\"Set-Cookie\" CONTENT=\"username=;expires=Wednesday, 21-Oct-98 16:14:21 GMT; path=/\">\n");
            //redirect to login screen
            printf("<meta http-equiv=\"refresh\" content=\"1; url=./login.cgi?page=%s\">\n", pageP);
            printf("<center><p><b><big><font color=red>Session is expired, please login again.</font></big></b></p></center>");
            htmlFooter();
        }
        else
        {

            printf("%s", html);
        }
        return 1;
    }
    return 0;
}

void htmlHeader3(char * titleP)
{
    printf("<html>\n\n");
    printf("<head>\n");

    printf("<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">\n");
    printf("<title>%s</title>\n", titleP);
    printf("<LINK REL=StyleSheet HREF=\"../style.css\" TYPE=\"text/css\">");

    printf("</head>\n\n");
    printf("<body>\n");
    printf("<br><br>\n");
}

void printLogin(const char *pageP)
{
    char tempS[128];
    //redirect to login screen
    sprintf(tempS, "./login.cgi?page=%s", pageP);
    htmlHeaderRedirect2(tempS, "Login", 1, 1);
    printf("<center><p><b><big><font color=red> Please login first.</font></big></b></p></center>");
    htmlFooter();
    return ;
}

int getAdsbCards(int *adsbCards)
{
    int numCards = 0;

    dcgGlobalT *sharedMemory = (dcgGlobalT *) ShMemAttach (DCG_SHMEM_NUM);
    if (sharedMemory)
    {
        struct timeval curTime;
        gettimeofday(&curTime, NULL);
        for (int x = 0; x < MAX_NUM_ADSB_CARDS + 1; x++)
        {
            //if the card is in the system it sends status every second
            if (curTime.tv_sec - sharedMemory->adsbCardData[x].lastReceivedStatus.tv_sec < 10)
            {
                if (adsbCards != NULL)
                {
                    adsbCards[numCards] = x;

                }
                numCards++;
            }
        }

    }
    return numCards;
}

void getPlatform(int *platform, int *formFactor)
{
    struct utsname unameBuffer;
    char versionBuffer[100];
    FILE *versionFile;
    uname(&unameBuffer);

    if(platform!=NULL)
    {
	*platform = PLATFORM_UNKNOWN;
	if(strstr(unameBuffer.machine, "86") != NULL)
	{
	    *platform = PLATFORM_X86;
	}
	else if(strstr(unameBuffer.machine, "ppc") != NULL)
	{
	    *platform = PLATFORM_P1012;
	}
	else if(strstr(unameBuffer.machine, "crisv32") != NULL)
	{
	    *platform = PLATFORM_AXIS;
	}
    }

    if(formFactor != NULL)
    {
	*formFactor = FORMFACTOR_UNKNOWN;
     	versionFile = fopen("/home/dcg/version", "r");
	if(versionFile)
	{
	    fgets(versionBuffer, 100, versionFile);
	    if(strstr(versionBuffer, "LONGPORT") != NULL)
	    {
		*formFactor = FORMFACTOR_LONGPORT;
	    }
	    else if(strstr(versionBuffer, "RICI") != NULL)
	    {
		*formFactor = FORMFACTOR_RICI;
	    }
	    else if(strstr(versionBuffer, "SG") != NULL)
	    {
		*formFactor = FORMFACTOR_SGP;
	    }
	}
    }
}
