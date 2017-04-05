
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <openssl/md5.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <limits.h>
#include "dsctypes.h"
#include "cgiparse_new.h"
#include "commonHTML.h"
#include "dcgsys.h"
#include "simcom.h"
#include "cmnweblogin.h"


int verifyUserLogin(char *userNameP, char *passChksumP, char *challengeP)
{
    userDataBaseInfoT userDataBaseInfo;
    char expPassChksum[WEB_BUFF_SIZE], tempS[WEB_BUFF_SIZE];
    FILE *fileP;
    int retVal = ERR_IVLD_USRPASS;
    long timeVal, dayVal, dayDiff;

    //Look for user name in the user file
    fileP = fopen(USERS_INFO, "r");

    if (fileP)
    {
        while (fgets(tempS, WEB_BUFF_SIZE, fileP) != NULL)
        {
            //initialize user info structure
            resetUserDataBaseFields(&userDataBaseInfo);
            sscanf(tempS, "%[^':\n']", userDataBaseInfo.userName);

            if (!strcmp(userDataBaseInfo.userName, userNameP))
            {
                //initialize user info structure
                resetUserDataBaseFields(&userDataBaseInfo, "null");

                //get the data fields from the file
                sscanf(tempS, "%[^':\n']:%[^':\n']:%[^':\n']:%[^':\n']:%[^':\n']:%[^':\n']:%[^':\n']",
                       userDataBaseInfo.userName,
                       userDataBaseInfo.userPerm,
                       userDataBaseInfo.userPasswd,
                       userDataBaseInfo.userPasswdEpochTimeSECS,
                       userDataBaseInfo.userPasswdExpirationDAYS,
                       userDataBaseInfo.userPasswdHintQuestion,
                       userDataBaseInfo.userPasswdHintAnswer);

                sprintf(tempS, "%s%s", userDataBaseInfo.userPasswd, challengeP);
                //calculate MD5 checksum
                md5(tempS, strlen(tempS), &expPassChksum[0]);

                if (strcmp(passChksumP, expPassChksum) != 0)
                {
                    //password is invalid - check to see if security question is set
                    if (!strcmp(userDataBaseInfo.userPasswdHintQuestion, "null") ||
                            !strcmp(userDataBaseInfo.userPasswdHintAnswer,   "null"))
                    {
                        //user password hint info is not set - so we cannot help user by allowing
                        //to reset password by answering password hint question
                        retVal = ERR_IVLD_USRPASS;
                    }
                    else
                    {
                        //user password hint info is set - so we can help user by allowing
                        //to reset password by answering password hint question
                        retVal = USR_PASSWD_HINT_SET;
                    }
                }
                else
                {
                    //if we reach here that means password is correct. Now, check to see
                    //if password has expired

                    //get the password creation/reset time and number of days
                    if ((strToLong(userDataBaseInfo.userPasswdEpochTimeSECS, &timeVal, 10) == TRUE) &&
                            (strToLong(userDataBaseInfo.userPasswdExpirationDAYS, &dayVal, 10) == TRUE))
                    {
                        if (dayVal == 0)
                        {
                            //password NEVER expires
                            retVal = TRUE;
                        }
                        else
                        {
                            dayDiff =  dayVal - ((time(NULL) - timeVal)/NUM_SECS_DAY);
                            if (dayDiff > 0)
                            {
                                //password has NOT expired
                                retVal = TRUE;
                            }
                            else
                            {
                                //password has expired
                                retVal = USR_PASSWD_EXPIRED;
                            }
                        }
                    }
                    else
                    {
                        //cannot obtain info on the required fields - just accept this request
                        retVal = TRUE;
                    }
                }

                break;
            }
        }

        fclose(fileP);
    }

    return (retVal);
}


int totalUsersDB()
{
    FILE *fileP;
    int total=0;
    char tempS[WEB_BUFF_SIZE];

    //Look for user name in the user file
    fileP = fopen(USERS_INFO, "r");

    if (fileP)
    {
        while (fgets(tempS, WEB_BUFF_SIZE, fileP) != NULL)
        {
            total++;
        }

        fclose(fileP);
    }

    return (total);
}

int addUser(userDataBaseInfoT* userDataBaseInfoP)
{
    FILE *lckFileP, *fileP;
    int retVal = ERR_OPR_FAILED;

    lckFileP = fopen(USERS_LCKFILE, "w");
    if (lckFileP)
    {
        //exclusively lock this file while updating users info file
        flock(fileno(lckFileP),  LOCK_EX);
        //Add this user to our database file
        fileP = fopen(USERS_INFO, "a");
        if (fileP)
        {
            fprintf(fileP, "%s:%s:%s:%s:%s:%s:%s\n",
                    userDataBaseInfoP->userName,
                    userDataBaseInfoP->userPerm,
                    userDataBaseInfoP->userPasswd,
                    userDataBaseInfoP->userPasswdEpochTimeSECS,
                    userDataBaseInfoP->userPasswdExpirationDAYS,
                    userDataBaseInfoP->userPasswdHintQuestion,
                    userDataBaseInfoP->userPasswdHintAnswer);
            fclose(fileP);
            retVal = TRUE;
        }
        //unlock the file
        flock(fileno(lckFileP),  LOCK_UN);
        fclose(lckFileP);
    }
    return (retVal);
}


void resetUserDataBaseFields(userDataBaseInfoT * userDataBaseInfoP, char *initCharP)
{
    //no need to use expensive memset function since the following
    //will do just fine for our purposes
    if (initCharP != NULL)
    {
        strcpy(userDataBaseInfoP->userName, "null");
        strcpy(userDataBaseInfoP->userPerm, "null");
        strcpy(userDataBaseInfoP->userPasswd, "null");
        strcpy(userDataBaseInfoP->userPasswdEpochTimeSECS, "null");
        strcpy(userDataBaseInfoP->userPasswdExpirationDAYS, "null");
        strcpy(userDataBaseInfoP->userPasswdHintQuestion, "null");
        strcpy(userDataBaseInfoP->userPasswdHintAnswer, "null");
    }
    else
    {
        userDataBaseInfoP->userName[0]                 = '\0';
        userDataBaseInfoP->userPerm[0]                 = '\0';
        userDataBaseInfoP->userPasswd[0]               = '\0';
        userDataBaseInfoP->userPasswdEpochTimeSECS[0]  = '\0';
        userDataBaseInfoP->userPasswdExpirationDAYS[0] = '\0';
        userDataBaseInfoP->userPasswdHintQuestion[0]   = '\0';
        userDataBaseInfoP->userPasswdHintAnswer[0]     = '\0';
    }
}


int resetUserParms(userDataBaseInfoT* userDataBaseInfoP)
{
    FILE *lckFileP, *tmpFileP, *fileP;
    char tempS[WEB_BUFF_SIZE];
    int retVal = ERR_OPR_FAILED;
    userDataBaseInfoT userDataBaseInfo;

    lckFileP = fopen(USERS_LCKFILE, "w");
    if (lckFileP)
    {
        //exclusively lock this file while updating users info file
        flock(fileno(lckFileP),  LOCK_EX);
        //open users database file
        fileP = fopen(USERS_INFO, "r");
        //open tmp file for copying
        tmpFileP = fopen(USERS_TMPFILE, "w");
        if (fileP && tmpFileP)
        {
            retVal = ERR_USR_NFOUND;
            while (fgets(tempS, WEB_BUFF_SIZE, fileP) != NULL)
            {
                //get user name
                sscanf(tempS, "%[^':\n']", userDataBaseInfo.userName);

                //is this the user we're looking for?
                if (!strcmp(userDataBaseInfoP->userName, userDataBaseInfo.userName))
                {
                    //initialize user info structure
                    resetUserDataBaseFields(&userDataBaseInfo, "null");

                    //get the data fields from the file
                    sscanf(tempS, "%[^':\n']:%[^':\n']:%[^':\n']:%[^':\n']:%[^':\n']:%[^':\n']:%[^':\n']",
                           userDataBaseInfo.userName,
                           userDataBaseInfo.userPerm,
                           userDataBaseInfo.userPasswd,
                           userDataBaseInfo.userPasswdEpochTimeSECS,
                           userDataBaseInfo.userPasswdExpirationDAYS,
                           userDataBaseInfo.userPasswdHintQuestion,
                           userDataBaseInfo.userPasswdHintAnswer);

                    //check for empty fields in the input data
                    //fill it with what's found in the database
                    if (userDataBaseInfoP->userPerm[0] != '\0')
                    {
                        strcpy(userDataBaseInfo.userPerm, userDataBaseInfoP->userPerm);
                    }
                    if (userDataBaseInfoP->userPasswd[0] != '\0')
                    {
                        strcpy(userDataBaseInfo.userPasswd, userDataBaseInfoP->userPasswd);
                    }
                    if (userDataBaseInfoP->userPasswdEpochTimeSECS[0] != '\0')
                    {
                        strcpy(userDataBaseInfo.userPasswdEpochTimeSECS, userDataBaseInfoP->userPasswdEpochTimeSECS);
                    }
                    if (userDataBaseInfoP->userPasswdExpirationDAYS[0] != '\0')
                    {
                        strcpy(userDataBaseInfo.userPasswdExpirationDAYS, userDataBaseInfoP->userPasswdExpirationDAYS);
                    }
                    if (userDataBaseInfoP->userPasswdHintQuestion[0] != '\0')
                    {
                        strcpy(userDataBaseInfo.userPasswdHintQuestion, userDataBaseInfoP->userPasswdHintQuestion);
                    }
                    if (userDataBaseInfoP->userPasswdHintAnswer[0] != '\0')
                    {
                        strcpy(userDataBaseInfo.userPasswdHintAnswer, userDataBaseInfoP->userPasswdHintAnswer);
                    }

                    //write user information to the file
                    fprintf(tmpFileP, "%s:%s:%s:%s:%s:%s:%s\n",
                            userDataBaseInfo.userName,
                            userDataBaseInfo.userPerm,
                            userDataBaseInfo.userPasswd,
                            userDataBaseInfo.userPasswdEpochTimeSECS,
                            userDataBaseInfo.userPasswdExpirationDAYS,
                            userDataBaseInfo.userPasswdHintQuestion,
                            userDataBaseInfo.userPasswdHintAnswer);

                    //found user
                    retVal = TRUE;

                }
                else
                {
                    fprintf(tmpFileP, "%s", tempS);
                }
            }

            fclose(tmpFileP);
            fclose(fileP);
            if (rename(USERS_TMPFILE, USERS_INFO) < 0)
            {
                retVal = ERR_OPR_FAILED;
            }
        }
        //unlock the file
        flock(fileno(lckFileP),  LOCK_UN);
        fclose(lckFileP);
    }

    return (retVal);
}


int rmUser(char *userNameP)
{
    FILE *lckFileP, *tmpFileP, *fileP;
    char tempS[WEB_BUFF_SIZE];
    int retVal = ERR_OPR_FAILED;
    userDataBaseInfoT userDataBaseInfo;

    lckFileP = fopen(USERS_LCKFILE, "w");
    if (lckFileP)
    {
        //exclusively lock this file while updating users info file
        flock(fileno(lckFileP),  LOCK_EX);
        //open users database file
        fileP = fopen(USERS_INFO, "r");
        //open tmp file for copying
        tmpFileP = fopen(USERS_TMPFILE, "w");
        if (fileP && tmpFileP)
        {
            retVal = ERR_USR_NFOUND;
            while (fgets(tempS, WEB_BUFF_SIZE, fileP) != NULL)
            {
                //get user name
                sscanf(tempS, "%[^':\n']", userDataBaseInfo.userName);

                if (!strcmp(userDataBaseInfo.userName, userNameP))
                {
                    //found user
                    retVal = TRUE;
                }
                else
                {
                    fprintf(tmpFileP, "%s", tempS);
                }
            }

            fclose(tmpFileP);
            fclose(fileP);
            if (rename(USERS_TMPFILE, USERS_INFO) < 0)
            {
                retVal = ERR_OPR_FAILED;
            }
        }
        //unlock the file
        flock(fileno(lckFileP),  LOCK_UN);
        fclose(lckFileP);

        //end this user's active sessions (if any)
        if (retVal)
        {
            endUsrSession(userNameP);
        }
    }

    return (retVal);
}

int isUsrFoundDB(char *userNameP)
{
    FILE *fileP;
    char tempS[WEB_BUFF_SIZE];
    userDataBaseInfoT userDataBaseInfo;

    //Look for user name in the user file
    fileP = fopen(USERS_INFO, "r");

    if (fileP)
    {
        while (fgets(tempS, WEB_BUFF_SIZE, fileP) != NULL)
        {
            userDataBaseInfo.userName[0] = '\0';

            sscanf(tempS, "%[^':\n']", userDataBaseInfo.userName);

            if (!strcmp(userDataBaseInfo.userName, userNameP))
            {
                //Found that user in our search
                fclose(fileP);
                return (TRUE);
            }
        }

        fclose(fileP);
    }

    return (ERR_USR_NFOUND);
}

int endUsrSession(char *userNameP)
{
    dcgGlobalT *dcgGsmGP=0;

    dcgGsmGP = (dcgGlobalT *) ShMemAttach (DCG_SHMEM_NUM);
    if (!dcgGsmGP)
    {
        return (ERR_SHM_UINIT);
    }

    for (int i=0;i<MAX_LOGIN_USERS;i++)
    {
        if (dcgGsmGP->userLoginInfo[i].userName[0] != '\0')
        {
            if (!strcmp(dcgGsmGP->userLoginInfo[i].userName, userNameP))
            {
                //reset shared memory data
                dcgGsmGP->userLoginInfo[i].userName[0]    = 0;
                dcgGsmGP->userLoginInfo[i].timeOutCounter = 0;
                dcgGsmGP->userLoginInfo[i].sessionCount   = 0;
                return (TRUE);
            }
        }
    }

    return (ERR_USR_NLGDIN);
}


int isUsrLoggedIn(char *userNameP, BoolT resetIdleTime)
{
    dcgGlobalT *dcgGsmGP=0;

    dcgGsmGP = (dcgGlobalT *) ShMemAttach (DCG_SHMEM_NUM);
    if (!dcgGsmGP)
    {
        return (ERR_SHM_UINIT);
    }
    for (int i=0;i<MAX_LOGIN_USERS;i++)
    {
        if (dcgGsmGP->userLoginInfo[i].userName[0] != '\0')
        {
            if (!strcmp(dcgGsmGP->userLoginInfo[i].userName, userNameP))
            {
                if (resetIdleTime)
                {
                    //update timeout counter
                    dcgGsmGP->userLoginInfo[i].timeOutCounter = getLoginTimeout();
                }
                return (TRUE);
            }
        }
    }

    return (ERR_USR_NLGDIN);
}


int getUserSessionCount(char *userNameP)
{
    dcgGlobalT *dcgGsmGP=0;

    dcgGsmGP = (dcgGlobalT *) ShMemAttach (DCG_SHMEM_NUM);
    if (!dcgGsmGP)
    {
        return (ERR_SHM_UINIT);
    }
    for (int i=0;i<MAX_LOGIN_USERS;i++)
    {
        if (dcgGsmGP->userLoginInfo[i].userName[0] != '\0')
        {
            if (!strcmp(dcgGsmGP->userLoginInfo[i].userName, userNameP))
            {
                return (dcgGsmGP->userLoginInfo[i].sessionCount);
            }
        }
    }

    return (ERR_USR_NLGDIN);
}


int getChallengeKey(char * challengeKey, int len)
{
    int randN;
    char tempS[MD5_DIGEST_LEN + 1];

    if (len < (CHLNGE_KEY_LEN + 1))
    {
        return (ERR_IVLD_ARRLEN);
    }

    //Initialize
    memset(challengeKey, 0, sizeof(char) * (CHLNGE_KEY_LEN + 1));
    srand(time(NULL));
    randN = 1+(int) (100.0*rand()/(RAND_MAX+1.0));

    sprintf(tempS, "%d", randN);

    //generate random challenge key
    md5(&tempS[0], strlen(tempS), challengeKey);

    return (CHLNGE_KEY_LEN);
}


void md5(char *inputStr, int strLen, char *outputStr)
{
    MD5_CTX state;
    unsigned char digest[MD5_DIGEST_LEN];
    int di;

    MD5_Init(&state);
    MD5_Update(&state, inputStr, strLen);
    MD5_Final(digest, &state);
    for (di = 0; di < MD5_DIGEST_LEN; ++di)
    {
        sprintf(outputStr + di * 2, "%02x", digest[di]);
    }
}


int rsaPrivateDecrypt(char *inDataP, int len, char *outDataP)
{
    BIO *key=NULL;
    EVP_PKEY *pkey=NULL;
    RSA *rsa = NULL;
    char tmpStr[4];
    unsigned char *rsa_in = NULL, *rsa_out = NULL, encData[RSA_DATA_LEN];
    int rsa_inlen(0), rsa_outlen(0), keysize(0);
    int i(0), tmpCount(0), count(0);
    //secret pass code to unencrypt the private key
    const char passPhrase[] = {'O', 'N', 'E', 'S', 'U', 'N', 'H', 'I', 'L', 'L', 'O', '\0'};

    //RSA related initialization
    OpenSSL_add_all_algorithms();
    key = BIO_new_mem_buf((void *)PRIVATE_KEY, strlen(PRIVATE_KEY));
    pkey= PEM_read_bio_PrivateKey(key,NULL,NULL, (void *)passPhrase);

    if (pkey == NULL)
    {
        return (ERR_IVLD_RSAERR);
    }
    rsa = EVP_PKEY_get1_RSA(pkey);
    EVP_PKEY_free(pkey);
    keysize = RSA_size(rsa);
    rsa_in = (unsigned char*)OPENSSL_malloc(keysize * 2);
    rsa_out = (unsigned char*)OPENSSL_malloc(keysize);

    //BCD conversion
    count = 0;
    for (tmpCount=0; tmpCount < (RSA_DATA_LEN * 2); tmpCount += 2)
    {
        tmpStr[0] = inDataP[tmpCount];
        tmpStr[1] = inDataP[tmpCount + 1];
        tmpStr[2] = '\0';
        //RSA public encrypted data byte
        encData[count++] = strtoul (tmpStr, NULL, 0x10);
    }

    rsa_inlen = count;
    //RSA private decrypt
    rsa_outlen  = RSA_private_decrypt(rsa_inlen, encData, rsa_out, rsa, RSA_NO_PADDING);

    if (rsa_outlen <= 0)
    {//error
        return (ERR_IVLD_RSAERR);
    }

    //rsa_out contains null chars and the MD5 password checksum but in
    //reverse order, so will reverse it back and get rid of all null chars
    count--;

    //remove all the null chars
    while (count >= 0)
    {
        if (rsa_out[count] == '\0')
        {
            break;
        }
        else
        {
            outDataP[i++] = rsa_out[count];
        }

        count--;
    }

    //null terminate
    outDataP[i] = '\0';

    //free up memory
    RSA_free(rsa);
    if (rsa_in) OPENSSL_free(rsa_in);
    if (rsa_out) OPENSSL_free(rsa_out);

    return (TRUE);
}



int verifyUserSecurityInfo(char *userNameP, char *secrChksumP)
{
    userDataBaseInfoT userDataBaseInfo;
    char tempS[WEB_BUFF_SIZE];
    FILE *fileP;
    int retVal = ERR_USR_NFOUND;

    //Look for user name in the user file
    fileP = fopen(USERS_INFO, "r");

    if (fileP)
    {
        while (fgets(tempS, WEB_BUFF_SIZE, fileP) != NULL)
        {
            //initialize user info structure
            resetUserDataBaseFields(&userDataBaseInfo);
            sscanf(tempS, "%[^':\n']", userDataBaseInfo.userName);

            if (!strcmp(userDataBaseInfo.userName, userNameP))
            {
                //initialize user info structure
                resetUserDataBaseFields(&userDataBaseInfo, "null");

                //get the data fields from the file
                sscanf(tempS, "%[^':\n']:%[^':\n']:%[^':\n']:%[^':\n']:%[^':\n']:%[^':\n']:%[^':\n']",
                       userDataBaseInfo.userName,
                       userDataBaseInfo.userPerm,
                       userDataBaseInfo.userPasswd,
                       userDataBaseInfo.userPasswdEpochTimeSECS,
                       userDataBaseInfo.userPasswdExpirationDAYS,
                       userDataBaseInfo.userPasswdHintQuestion,
                       userDataBaseInfo.userPasswdHintAnswer);

                //password is invalid - check to see if security question is set
                if (!strcmp(userDataBaseInfo.userPasswdHintQuestion, "null") ||
                        !strcmp(userDataBaseInfo.userPasswdHintAnswer,   "null"))
                {
                    //user password hint info is not set - so we cannot help user by allowing
                    //to reset password by answering password hint question
                    retVal = USR_PASSWD_HINT_NOTSET;
                }
                else
                {
                    //compare security answer
                    if (!strcmp(userDataBaseInfo.userPasswdHintAnswer, secrChksumP))
                    {
                        retVal = TRUE;
                    }
                    else
                    {
                        retVal = FALSE;
                    }
                }

                break;
            }
        }

        fclose(fileP);
    }

    return (retVal);
}


int isUserPasswdHintInfoSet(char *userNameP)
{
    userDataBaseInfoT userDataBaseInfo;
    char tempS[WEB_BUFF_SIZE];
    FILE *fileP;
    int retVal = ERR_USR_NFOUND;
    long lValue;

    //Look for user name in the user file
    fileP = fopen(USERS_INFO, "r");

    if (fileP)
    {
        while (fgets(tempS, WEB_BUFF_SIZE, fileP) != NULL)
        {
            //initialize user info structure
            resetUserDataBaseFields(&userDataBaseInfo);
            sscanf(tempS, "%[^':\n']", userDataBaseInfo.userName);

            if (!strcmp(userDataBaseInfo.userName, userNameP))
            {
                //initialize user info structure
                resetUserDataBaseFields(&userDataBaseInfo, "null");

                //get the data fields from the file
                sscanf(tempS, "%[^':\n']:%[^':\n']:%[^':\n']:%[^':\n']:%[^':\n']:%[^':\n']:%[^':\n']",
                       userDataBaseInfo.userName,
                       userDataBaseInfo.userPerm,
                       userDataBaseInfo.userPasswd,
                       userDataBaseInfo.userPasswdEpochTimeSECS,
                       userDataBaseInfo.userPasswdExpirationDAYS,
                       userDataBaseInfo.userPasswdHintQuestion,
                       userDataBaseInfo.userPasswdHintAnswer);

                //password is invalid - check to see if security question is set
                if (!strcmp(userDataBaseInfo.userPasswdHintQuestion, "null") ||
                        !strcmp(userDataBaseInfo.userPasswdHintAnswer,   "null"))
                {
                    //user password hint info is not set - so we cannot help user by allowing
                    //to reset password by answering password hint question
                    retVal = USR_PASSWD_HINT_NOTSET;
                }
                else
                {
                    //user password hint info is set - so we can help user by allowing
                    //to reset password by answering password hint question
                    if (strToLong(userDataBaseInfo.userPasswdHintQuestion, &lValue, 10) == TRUE)
                    {
                        //return the question number
                        retVal = (int)lValue;
                    }
                    else
                    {
                        retVal = USR_PASSWD_HINT_NOTSET;
                    }
                }

                break;
            }
        }

        fclose(fileP);
    }

    return (retVal);
}


BoolT strToLong(char *strVal, long *longVal, int baseVal)
{
    int retVal = TRUE;
    long val;
    char *endptr;

    //check to make sure base value is correct
    if (baseVal >= BASE_MIN_RANGE && baseVal <= BASE_MAX_RANGE && baseVal != 1)
    {
        errno = 0;    /* To distinguish success/failure after call */
        val = strtol(strVal, &endptr, baseVal);

        /* Check for various possible errors */
        if ((errno == ERANGE && (val  == LONG_MAX || val == LONG_MIN))
                || (errno != 0 && val == 0))
        {
            retVal = FALSE;
        }
        else if (endptr == strVal)
        {
            //no digits found
            retVal = FALSE;
        }
        else
        {
            //success
            *longVal = val;
        }
    }

    return retVal;
}


void printSecurityJavaScriptHTML()
{
    printf("<script language=\"javascript\" src=\"../md5.js\"></script>\n");
    printf("<script language=\"JavaScript\" src=\"../BigInt.js\"></script>\n");
    printf("<script language=\"JavaScript\" src=\"../Barrett.js\"></script>\n");
    printf("<script language=\"JavaScript\" src=\"../RSA.js\"></script>\n");
    printf("<script language=\"JavaScript\" src=\"../webusers.js\"></script>\n");
}


void printForgotPasswordHTML(char *msgP)
{
    //show "Forgot Password" button
    printf("<form name=\"forgotpass\" method=\"POST\" action=\"./usrPasswd.cgi\">"
           "<input type=hidden name=\"passpg_action\" value=\"Forgot Password?\">"
           "%s<br><input type=submit name=\"submitbtn\" value=\"Forgot Password?\">"
           "</form>\n", msgP);
}


void printResetPasswordHTML(char *userNameP)
{
    //show "Reset Password" button
    printf("<form name=\"expirepass\" method=\"POST\" action=\"./usrPasswd.cgi\">"
           "<input type=hidden name=\"passpg_action\" value=\"Expired Password - Show Reset Password Screen\">"
           "<input type=hidden name=\"userid\" value=\"%s\">"
           "Your password has expired!  Please reset password<br><br>"
           "<input type=submit name=\"submitbtn\" value=\"Reset Password\">&nbsp;&nbsp;"
           "<input type=\"button\" name=\"mainbtn\" value=\"Cancel\" onClick=\"window.location.href='./login.cgi'\">"
           "</form>\n", userNameP);
}


void printLoginHTML(cmnLoginPageFieldsT * cmnLoginPageFieldsP)
{
    char challengeKey[CHLNGE_KEY_LEN + 1];
    char metaData[256];

    sprintf(metaData, "<meta http-equiv=\"Pragma\" CONTENT=\"no-cache\">\n"
            "<meta http-equiv=\"Expires\" CONTENT=\"-1\">\n");

    //set page title
    if (cmnLoginPageFieldsP->pageTitle[0] != '\0')
    {
        //htmlHeader2(cmnLoginPageFieldsP->pageTitle, metaData);
    }
    else
    {
        //htmlHeader2("Sign In", metaData);
    }

    //include required security javascript files
    printSecurityJavaScriptHTML();

    //if password has expired, no need to show login form
    if (cmnLoginPageFieldsP->eventValue != USR_PASSWD_EXPIRED)
    {
        //javascript code
        printf("<SCRIPT TYPE=\"text/javascript\">\n \
               <!--\n \
               \n \
               /*\n \
               * execute 'login()' function when page is loaded\n \
               */\n \
               window.onload=function()\n \
           {\n \
               var W3CDOM=document.getElementById&&document.\n \
               getElementsByTagName&&document.createElement;\n \
               // check if browser is W3CDOM compatible\n \
               if(W3CDOM)\n \
           {\n \
               document.loginform.userid.value=\"%s\"; \n \
               document.loginform.userid.style.color=\"black\"; \n \
               document.loginform.userid.focus();\n \
               document.loginform.passwd.disabled=false;\n \
               document.loginform.challenge.disabled=false;\n \
               document.loginform.salt.disabled=false;\n \
               document.loginform.page.disabled=false;\n \
               document.loginform.loginbtn.disabled=false;\n \
               document.loginform.loginbtn.value=\"%s\"; \n \
               if(document.loginform.userid.value != \"\")\n \
           {\n \
               document.loginform.userid.disabled=\"disabled\";\n \
               document.loginform.passwd.focus();\n \
           }\n \
               document.getElementById('loginform').onsubmit=function()\n \
           {\n \
               document.loginform.method=\"POST\"; \n \
               document.loginform.action=\"%s\"; \n \
               document.loginform.userid.disabled=false;\n \
               return verifyLogin();\n \
           }\n \
           }\n \
           }\n \
               //-->\n \
               </SCRIPT>\n", cmnLoginPageFieldsP->userName, cmnLoginPageFieldsP->buttonText, cmnLoginPageFieldsP->formAction);
    }

    printf("<center>");
    printf("<b>%s</b>", cmnLoginPageFieldsP->pageTitle);

    //print any event messages
    printEventMsgInfo(cmnLoginPageFieldsP->userName, cmnLoginPageFieldsP->failure, cmnLoginPageFieldsP->eventValue);

    //if password has expired, no need to show login form
    if (cmnLoginPageFieldsP->eventValue != USR_PASSWD_EXPIRED)
    {
        printf("<table border=\"0\" style=\" border: 1px solid grey;\">\n");
        printf("<tr><td><form name=\"loginform\" id=\"loginform\" method=\"GET\" action=\"../JavaScriptNotEnabled.html\"></td></tr>\n");
        //Display login and password text input and login button
        printf("<tr><td>&nbsp;</td></tr>\n");
        printf("<tr><td>User ID<br><br></td><td><input type=\"text\" name=\"userid\" style=\"color: red;\" id=\"userid\" value=\"Please Enable Javascript\" maxlength=\"32\"><br><br></td></tr>\n");
        printf("<tr><td>Password<br><br></td><td><input type=\"password\" name=\"passwd\" disabled=\"disabled\" id=\"passwd\" maxlength=\"32\"><br><br></td></tr>\n");

        //Now get a challenge key
        getChallengeKey(&challengeKey[0], CHLNGE_KEY_LEN + 1);

        printf("<tr><td>&nbsp;</td>\n");

        printf("<td><input type=submit name=\"loginbtn\" value=\"Help\">\n");
        printf("<input type=hidden name=\"challenge\" disabled=\"disabled\" id=\"challenge\" value=\"%s\">\n", &challengeKey[0]);
        printf("<input type=hidden name=\"salt\" disabled=\"disabled\" id=\"salt\" value=\"%s\">\n", SYSTEM_SALT);
        if (cmnLoginPageFieldsP->redirectPage)
        {
            printf("<input type=hidden name=\"page\" disabled=\"disabled\" id=\"page\" value=\"%s\">\n", cmnLoginPageFieldsP->redirectPage);
        }
        else
        {
            printf("<input type=hidden name=\"page\" disabled=\"disabled\" id=\"page\" value=\"\">\n");
        }
        printf("</td></tr>\n");

        printf("<tr><td>&nbsp;</td>\n");

        printf("</form></table><br>\n");
    }

    printf("</center>");
    htmlFooter();
}


void printEventMsgInfo(char *userNameP, BoolT loginFail, int eventVal)
{
    if (loginFail == TRUE)
    {
        printf("<font color=\"red\">\n");
    }
    else
    {
        printf("<font color=\"blue\">\n");
    }

    //show msg ?
    if (eventVal < 0)
    {
        printf("<br><br>\n");

        switch (eventVal)
        {
        case ERR_LOGIN_FAIL_SHDMEM:
            printf ("Application software is not running!");
            break;
        case ERR_LOGIN_FAIL_USREXST:
            printf("User already logged in!");
            break;
        case ERR_LOGIN_FAIL_USEROVF:
            printf("Maximum user login count reached!");
            break;
        case ERR_LOGIN_FAIL_USRPASS:
        case USR_PASSWD_HINT_SET:
            //show "Forgot Password" button
            printForgotPasswordHTML("Incorrect User ID or Password entered!");
            break;
        case USR_PASSWD_EXPIRED:
            //show "Reset Password" button
            printResetPasswordHTML(userNameP);
            break;
        case ERR_USR_ADMIN_ADDEL:
            printf("The operation is NOT permitted for this user!");
            break;
        case ERR_USR_FOUND:
            printf("User already exists!");
            break;
        case ERR_USR_NFOUND:
            printf("User does not exist!");
            break;
        case ERR_OPR_FAILED:
            printf("Operation Failed!");
            break;
        case ERR_USR_MAXCNT:
            printf("Maximum allowed user count reached!");
            break;
        case USR_ADD_SUCCESS:
            printf("New user account successfully created!");
            break;
        case USR_DEL_SUCCESS:
            printf("User account successfully deleted!");
            break;
        case USR_RPS_SUCCESS:
            printf("User password successfully reset!");
            break;
        case USR_CHNSET_SUCCESS:
            printf("User account setting(s) successfully changed!");
            break;
        case ERR_IVLD_RSAERR:
            printf("Operation failed! Please make sure JavaScript is <a href=../JavaScriptNotEnabled.html>enabled</a>");
            break;
        case ERR_IVLD_USRPASSEXP:
            printf("Invalid password expiration information!");
            break;
        case ERR_IVLD_USRPASS:
            printForgotPasswordHTML("Incorrect password entered!");
            break;
        case USR_PASSWD_HINT_NOTSET:
            printf("Incorrect User ID or no security information set for this user!");
            break;
        case ERR_IVLD_USRSECRANS:
            printf("Incorrect user security answer! Please try again.");
            break;
        case ERR_IVLD_USRSECRQA:
            printf("Invalid user security question/answer information!");
            break;
        default:
            printf("Login failed (CODE: %d)!", eventVal);
            break;
        }
    }

    printf("</font>");
}
