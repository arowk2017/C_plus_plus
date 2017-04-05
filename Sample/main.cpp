
#include "Mcast.h"
#include <cstdlib>

int main (int argc, char **argv, char** envp)
{   
    const char *query;
    char *page;
    struct url_parameter_type *up_p;
    htmlTemplate *currentPage;
    char *mcast;
    char *proto;
    char *mcastButton;

    query = getenv("QUERY_STRING");

    if(query == NULL)
    {
        query = "";
        if(argc > 1)
        {
            query = argv[1];
        }
    }
    
    up_p = url_parameter_type_init(query);
    
    mcast = (char*) url_parameter_get_data(up_p, "mcast", NULL);
    page = (char*) url_parameter_get_data(up_p, "page", NULL);
    proto = (char*) url_parameter_get_data(up_p, "proto", NULL);
    mcastButton = (char*) url_parameter_get_data(up_p, "mcastButton", NULL);
    std::string mcast_link(reinterpret_cast<char*>(mcast));

    if(page == NULL)
    {
        printf("<h1>Page '' Not found<h1><br>\n" );
        exit(0);
    }

    for(unsigned int x = 0; x < strlen(page); x++)
    {
        if(page[x] == '/')
        {
            printf("<h1>Page '%s' Not found<h1><br>\n",page );
            exit(0);
        }
    }


    currentPage = new htmlTemplate(page);

    const char *tempP;
    int rc;
    
    //cookie has username in it
    tempP = getenv("HTTP_COOKIE");

	/* Print mandatory Content-Type */
    printf ("Content-Type: text/html\n\n");	/* Important ENV variables */
    printf("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/strict.dtd\" >\n");


    rc = checkLogin(tempP, "Mcast.cgi");

    if (rc)
        return rc;

    if ( page != NULL && (strcmp(page ,"McastScreen") == 0) )
    {


/*
HTML SECTION
*/

 printf("<html>\n\n");
    printf("<head>\n");

    printf("<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">\n");
    printf("<title>MPS Mcast</title>\n");
    printf("<LINK REL=StyleSheet HREF=\"../mps.css\" TYPE=\"text/css\">\n");
    printf("<script type='text/javascript' src='../mpsMcast.js'> </script>\n");
    printf("</head>\n\n");
    printf("<body>\n");


printf("<form id='mcastForm' method='post' name='mcastForm'  >\n");

    	if ( proto != NULL && (strcmp(proto ,"HDLC") == 0) )
    	{
    		printf("<table id='mcast_protocol_HDLC' cellpadding='0' cellspacing='0' width='100%%'> \n");
		mcastHDLC();
		printf("</table> \n");
		printf("<table id='mcast_protocol_ASYNC' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
		mcastASYNCdefault();
		printf("</table> \n");
    		printf("<table id='mcast_protocol_ASTERIX' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
		mcastASTERIXdefault();
		printf("</table> \n");
    		printf("<table id='mcast_protocol_CD2' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
		mcastCD2default();
		printf("</table> \n");
		printf("<table id='mcast_protocol_TADILB' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
		mcastTADILBdefault();
		printf("</table> \n");
		printf("<table id='mcast_protocol_SBSI' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
		mcastSBSIdefault();
		printf("</table> \n");
                printf("<table id='mcast_protocol_THOM_CSF' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastTHOM_CSFdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_THOM_TVT2' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastTHOM_TVT2default();
                printf("</table> \n");
                printf("<table id='mcast_protocol_TOSH' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastTOSHdefault();
                printf("</table> \n");

   		printf("<table id='mcast_protocol_standard' cellpadding='0' cellspacing='0' width='100%%' > \n");
    		mcastStandardAttrForm(0);
    	}
	else if ( proto != NULL && (strcmp(proto ,"ASTERIX") == 0) )
	{

                printf("<table id='mcast_protocol_HDLC' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastHDLCdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_ASYNC' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastASYNCdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_ASTERIX' cellpadding='0' cellspacing='0' width='100%%'> \n");
                mcastASTERIX();
                printf("</table> \n");
                printf("<table id='mcast_protocol_CD2' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastCD2default();
                printf("</table> \n");
                printf("<table id='mcast_protocol_TADILB' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastTADILBdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_SBSI' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastSBSIdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_THOM_CSF' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastTHOM_CSFdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_THOM_TVT2' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastTHOM_TVT2default();
                printf("</table> \n");
                printf("<table id='mcast_protocol_TOSH' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastTOSHdefault();
                printf("</table> \n");

                printf("<table id='mcast_protocol_standard' cellpadding='0' cellspacing='0' width='100%%' > \n");
                mcastStandardAttrForm(2);
	}
        else if ( proto != NULL && (strcmp(proto ,"ASYNC") == 0) )
        {
                printf("<table id='mcast_protocol_HDLC' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastHDLCdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_ASYNC' cellpadding='0' cellspacing='0' width='100%%'> \n");
                mcastASYNC();
                printf("</table> \n");
                printf("<table id='mcast_protocol_ASTERIX' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastASTERIXdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_CD2' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastCD2default();
                printf("</table> \n");
                printf("<table id='mcast_protocol_TADILB' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastTADILBdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_SBSI' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastSBSIdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_THOM_CSF' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastTHOM_CSFdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_THOM_TVT2' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastTHOM_TVT2default();
                printf("</table> \n");
                printf("<table id='mcast_protocol_TOSH' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastTOSHdefault();
                printf("</table> \n");

                printf("<table id='mcast_protocol_standard' cellpadding='0' cellspacing='0' width='100%%' > \n");
                mcastStandardAttrForm(1);

        }
        else if ( proto != NULL && (strcmp(proto ,"CD-2") == 0) )
        {
                printf("<table id='mcast_protocol_HDLC' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastHDLCdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_ASYNC' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastASYNCdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_ASTERIX' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastASTERIXdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_CD2' cellpadding='0' cellspacing='0' width='100%%'> \n");
                mcastCD2();
                printf("</table> \n");
                printf("<table id='mcast_protocol_TADILB' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastTADILBdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_SBSI' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastSBSIdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_THOM_CSF' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastTHOM_CSFdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_THOM_TVT2' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastTHOM_TVT2default();
                printf("</table> \n");
                printf("<table id='mcast_protocol_TOSH' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastTOSHdefault();
                printf("</table> \n");

                printf("<table id='mcast_protocol_standard' cellpadding='0' cellspacing='0' width='100%%' > \n");
                mcastStandardAttrForm(3);
        }
        else if ( proto != NULL && (strcmp(proto ,"TADIL-B") == 0) )
        {
                printf("<table id='mcast_protocol_HDLC' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastHDLCdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_ASYNC' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastASYNCdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_ASTERIX' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastASTERIXdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_CD2' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastCD2default();
                printf("</table> \n");
                printf("<table id='mcast_protocol_TADILB' cellpadding='0' cellspacing='0' width='100%%'> \n");
                mcastTADILB();
                printf("</table> \n");
                printf("<table id='mcast_protocol_SBSI' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastSBSIdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_THOM_CSF' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastTHOM_CSFdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_THOM_TVT2' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastTHOM_TVT2default();
                printf("</table> \n");
                printf("<table id='mcast_protocol_TOSH' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastTOSHdefault();
                printf("</table> \n");

                printf("<table id='mcast_protocol_standard' cellpadding='0' cellspacing='0' width='100%%' > \n");
                mcastStandardAttrForm(4);
        }
        else if ( proto != NULL && (strcmp(proto ,"SBSI") == 0) )
        {
                printf("<table id='mcast_protocol_HDLC' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastHDLCdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_ASYNC' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastASYNCdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_ASTERIX' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastASTERIXdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_CD2' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastCD2default();
                printf("</table> \n");
                printf("<table id='mcast_protocol_TADILB' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastTADILBdefault();
                printf("</table> \n"); 
                printf("<table id='mcast_protocol_SBSI' cellpadding='0' cellspacing='0' width='100%%'> \n");
                mcastSBSI();
                printf("</table> \n");
                printf("<table id='mcast_protocol_THOM_CSF' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastTHOM_CSFdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_THOM_TVT2' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastTHOM_TVT2default();
                printf("</table> \n");
                printf("<table id='mcast_protocol_TOSH' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastTOSHdefault();
                printf("</table> \n");

                printf("<table id='mcast_protocol_standard' cellpadding='0' cellspacing='0' width='100%%' > \n");
                mcastStandardAttrForm(5);

        }
        else if ( proto != NULL && (strcmp(proto ,"thom-csf") == 0) )
        {
                printf("<table id='mcast_protocol_HDLC' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastHDLCdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_ASYNC' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastASYNCdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_ASTERIX' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastASTERIXdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_CD2' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastCD2default();
                printf("</table> \n");
                printf("<table id='mcast_protocol_TADILB' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastTADILBdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_SBSI' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastSBSIdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_THOM_CSF' cellpadding='0' cellspacing='0' width='100%%'> \n");
                mcastTHOM_CSF();
                printf("</table> \n");
                printf("<table id='mcast_protocol_THOM_TVT2' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastTHOM_TVT2default();
                printf("</table> \n");
                printf("<table id='mcast_protocol_TOSH' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastTOSHdefault();
                printf("</table> \n");

                printf("<table id='mcast_protocol_standard' cellpadding='0' cellspacing='0' width='100%%' > \n");
                mcastStandardAttrForm(6);
        }
        else if ( proto != NULL && (strcmp(proto ,"thom-tvt2") == 0) )
        {
                printf("<table id='mcast_protocol_HDLC' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastHDLCdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_ASYNC' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastASYNCdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_ASTERIX' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastASTERIXdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_CD2' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastCD2default();
                printf("</table> \n");
                printf("<table id='mcast_protocol_TADILB' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastTADILBdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_SBSI' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastSBSIdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_THOM_CSF' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastTHOM_CSFdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_THOM_TVT2' cellpadding='0' cellspacing='0' width='100%%'> \n");
                mcastTHOM_TVT2();
                printf("</table> \n");
                printf("<table id='mcast_protocol_TOSH' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastTOSHdefault();
                printf("</table> \n");

                printf("<table id='mcast_protocol_standard' cellpadding='0' cellspacing='0' width='100%%' > \n");
                mcastStandardAttrForm(7);
        }
        else if ( proto != NULL && (strcmp(proto ,"toshiba") == 0) )
        {
                printf("<table id='mcast_protocol_HDLC' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastHDLCdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_ASYNC' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastASYNCdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_ASTERIX' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastASTERIXdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_CD2' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastCD2default();
                printf("</table> \n");
                printf("<table id='mcast_protocol_TADILB' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastTADILBdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_SBSI' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastSBSIdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_THOM_CSF' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastTHOM_CSFdefault();
                printf("</table> \n");
                printf("<table id='mcast_protocol_THOM_TVT2' cellpadding='0' cellspacing='0' width='100%%' style='display:none;'> \n");
                mcastTHOM_TVT2default();
                printf("</table> \n");
                printf("<table id='mcast_protocol_TOSH' cellpadding='0' cellspacing='0' width='100%%'> \n");
                mcastTOSH();
                printf("</table> \n");

                printf("<table id='mcast_protocol_standard' cellpadding='0' cellspacing='0' width='100%%' > \n");
                mcastStandardAttrForm(8);
        }

    printf("</table> \n");
    printf("</form> \n");
	htmlFooter();  

	return 0;
    }

    else if ( page != NULL && (strcmp(page,"McastSave") == 0) )
    {
	if ( mcastButton != NULL && (strcmp(mcastButton,"single") == 0) )
	{
    		mcastSave(mcast_link);
	}
	else if ( mcastButton != NULL && (strcmp(mcastButton,"all") == 0) )
	{
		std::string link0 = "0";
           
		mcastSave(link0);
                std::system("rm ../html/wanStr/mps_mcast_cfg/sun_mps_mcast/link1.txt");
                std::system("rm ../html/wanStr/mps_mcast_cfg/sun_mps_mcast/link2.txt");
                std::system("rm ../html/wanStr/mps_mcast_cfg/sun_mps_mcast/link3.txt");
		std::system("cp ../html/wanStr/mps_mcast_cfg/sun_mps_mcast/link0.txt ../html/wanStr/mps_mcast_cfg/sun_mps_mcast/link1.txt");
                std::system("cp ../html/wanStr/mps_mcast_cfg/sun_mps_mcast/link0.txt ../html/wanStr/mps_mcast_cfg/sun_mps_mcast/link2.txt");
                std::system("cp ../html/wanStr/mps_mcast_cfg/sun_mps_mcast/link0.txt ../html/wanStr/mps_mcast_cfg/sun_mps_mcast/link3.txt");
                std::system("chmod 777  ../html/wanStr/mps_mcast_cfg/sun_mps_mcast/link0.txt");
                std::system("chmod 777  ../html/wanStr/mps_mcast_cfg/sun_mps_mcast/link1.txt");
                std::system("chmod 777  ../html/wanStr/mps_mcast_cfg/sun_mps_mcast/link2.txt");
                std::system("chmod 777  ../html/wanStr/mps_mcast_cfg/sun_mps_mcast/link3.txt");
		std::system("cat ../html/wanStr/mps_mcast_cfg/sun_mps_mcast/link1.txt  | grep \"link_number\" | sed -i 's/link_number=0/link_number=1/' ../html/wanStr/mps_mcast_cfg/sun_mps_mcast/link1.txt");
                std::system("cat ../html/wanStr/mps_mcast_cfg/sun_mps_mcast/link2.txt  | grep \"link_number\" | sed -i 's/link_number=0/link_number=2/' ../html/wanStr/mps_mcast_cfg/sun_mps_mcast/link2.txt");
                std::system("cat ../html/wanStr/mps_mcast_cfg/sun_mps_mcast/link3.txt  | grep \"link_number\" | sed -i 's/link_number=0/link_number=3/' ../html/wanStr/mps_mcast_cfg/sun_mps_mcast/link3.txt");
	}
	else
	{
		return 0;
	}
    }
    else
    {
	return 0;
    }
}
