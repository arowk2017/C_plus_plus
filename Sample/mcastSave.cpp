
#include "Mcast.h"
#include <algorithm>
#include <utility>
void mcastSave(std::string mcast_link)
{

    const char *query;
    struct url_parameter_type *up_p;
    char *mcast;
    query = getenv("QUERY_STRING");

    up_p = url_parameter_type_init(query);
    mcast = (char*) url_parameter_get_data(up_p, "mcast", NULL);

    const char *tempP;

    //cookie has username in it
    tempP = getenv("HTTP_COOKIE");

        /* Print mandatory Content-Type */
    printf ("Content-Type: text/html\n\n");     /* Important ENV variables */
    printf("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/strict.dtd\" >\n");


    FILE *file;
    FILE *file2;
    std::string fileName = "../html/wanStr/mps_mcast_cfg/sun_mps_mcast/link" + mcast_link + ".txt";
    std::string fileNameTest = "../html/wanStr/mps_mcast_cfg/standardInputMps.txt";
    const char *contentLength;
    int count;
    char *textFormat;
    
    std::stringstream ss;
    std::string newTextInput;
    std::size_t findChar;
    std::size_t findPlus;
    std::vector <char*> mpsLine;
    std::vector <char*> mpsVal;
    std::vector <char*> mpsName;
    std::vector <char*> mpsOrigLine;
    std::vector <std::string> mpsStringLine;
    char  *saveptr1;
    std::string chompVal;
    std::string fileNewLine = "\n";
    std::string linkNumLine = "link_number=" + mcast_link;

    //contains mps popup window form attributes
    std::map <std::string, std::string> link_config_map;
    std::map <std::string, char*> link_config_val;
    
    contentLength = getenv("CONTENT_LENGTH");
    count = atoi(contentLength);
    count = count + 1;
    char string[count];

    //open the file
    file = fopen(fileName.c_str(), "wb");
    file2 =fopen(fileNameTest.c_str(), "wb");

    //read form field values from mcastForm popup window
    fgets(string, count, stdin);

    //TEST FILE
    fputs(string, file2);

    //parse each key/value pair separated by "&"
    textFormat = strtok_r(string,"&", &saveptr1);
    
    //push each stdin line into vector
    while(textFormat != NULL)
    {
	mpsLine.push_back(textFormat);
	mpsStringLine.push_back(reinterpret_cast<char*>(textFormat));
	textFormat = strtok_r(NULL,"&", &saveptr1);
    }

    //parse each key and value into seperate vectors
    for(size_t x = 0; x < mpsLine.size(); x++)
    {
	mpsName.push_back(strtok(mpsLine[x], "="));
	mpsVal.push_back(strtok(NULL, "="));
    }

    //inserts key/value into map as a check to associate attributes correctly
    for(size_t y = 0; y < mpsName.size(); y++)
    {
	std::string nameString(mpsName[y]);
	link_config_map[nameString] = mpsStringLine[y];
	link_config_val[nameString] = mpsVal[y];
    }

        //checks line for "+" character and replaces with " "
        findPlus = link_config_map["comment"].find("+");
        
	if (findPlus != std::string::npos)
        {
		
                newTextInput = replaceString(link_config_map["comment"], "+", " ");
                fputs (newTextInput.c_str(), file);
		fputs (fileNewLine.c_str(), file);
        }
	else
	{
        	fputs (link_config_map["comment"].c_str(), file);
        	fputs (fileNewLine.c_str(), file);
	}

        //Due to link number being constant and not submit through standard imput, link number is added statically
        fputs (linkNumLine.c_str(), file);

        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["protocol"].c_str(), file);
        fputs (fileNewLine.c_str(), file);

    chompVal = trim_back(link_config_val["protocol"]);

    if ( chompVal.c_str() != NULL && (strcmp(chompVal.c_str(), "SBSI") == 0) )
    {
	
        fputs (link_config_map["sbsi_dl_max_frame"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["sbsi_dl_baud_rate"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["sbsi_dl_rualive"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["sbsi_dl_data_acks"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["sbsi_dl_dump_idles"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["sbsi_dl_idlechar"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["sbsi_dl_TxBitOrder"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["sbsi_dl_RxBitOrder"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["sbsi_dl_modem_out"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["sbsi_dl_modem_in"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["sbsi_dl_sigtimeout"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["sbsi_dl_phyIf"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["sbsi_dl_encoding"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["sbsi_dl_options1"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["sbsi_dl_noclock_to"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
    }
    if ( chompVal.c_str() != NULL && (strcmp(chompVal.c_str(), "HDLC") == 0) )
    {
        fputs (link_config_map["hdlc_framesize"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["hdlc_baud"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["hdlc_encoding"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["hdlc_mode"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["hdlc_modem"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["hdlc_phyIf"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["hdlc_options1"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["hdlc_timeout_baud"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["hdlc_noclock_to"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
    }
    if ( chompVal.c_str() != NULL && (strcmp(chompVal.c_str(), "ASTERIX") == 0) )
    {
        fputs (link_config_map["rdr_max_messages"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["rdr_message_timeout"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["rdr_data_acks"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["rdr_options1"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["as_framesize"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["as_baud"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["as_encoding"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["as_modem"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["as_phyIf"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["as_timeout_baud"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["as_fwd_time"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["as_noclock_to"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
    }
    if ( chompVal.c_str() != NULL && (strcmp(chompVal.c_str(), "ASYNC") == 0) )
    {
        fputs (link_config_map["asy_bits"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["asy_parity"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["asy_stops"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["asy_baud"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["asy_rx_timeout"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["asy_rx_number"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["asy_rx_term"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["asy_rx_tcs"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["asy_use_xon_xoff"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["asy_rx_xon"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["asy_rx_xoff"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["asy_modem"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["asy_noclock_to"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["asy_phyIf"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["asy_sendOnlyGoodData"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["asy_dl_options1"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
    }
    if ( chompVal.c_str() != NULL && (strcmp(chompVal.c_str(), "CD-2") == 0) )
    {
        fputs (link_config_map["rdr_max_messages"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["rdr_message_timeout"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["rdr_data_acks"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["rdr_options1"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["cd_baud"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["cd_timeout_baud"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["cd_maxmesg_size"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["cd_maxmesg_cnt"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["cd_fwd_time"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["cd_dma_blocksize"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["cd_invert"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["cd_idle_pattern"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["cd_modem_in"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["cd_sigtimeout"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["cd_phyIf"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["cd_encoding"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["cd_noclock_to"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
    }
    if ( chompVal.c_str() != NULL && (strcmp(chompVal.c_str(), "TADIL-B") == 0) )
    {
        fputs (link_config_map["tadilb_baud_rate"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tadilb_maxmesg_size"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tadilb_dma_blocksize"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tadilb_idle_char"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tadilb_encoding"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tadilb_modem_in"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tadilb_data_acks"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tadilb_phyIf"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tadilb_timeout_baud"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tadilb_data_group_num"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tadilb_check_group_type"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tadilb_som"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tadilb_options1"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tadilb_wanprot_options1"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tadilb_num_idles_sent"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tadilb_noclock_to"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
    }
    if ( chompVal.c_str() != NULL && (strcmp(chompVal.c_str(), "thom-csf") == 0) )
    {
        fputs (link_config_map["rdr_max_messages"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["rdr_message_timeout"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["rdr_data_acks"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["rdr_options1"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_baud"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_timeout_baud"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_maxmesg_size"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_maxmesg_cnt"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_fwd_time"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_dma_blocksize"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_sync"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_check_crc"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_modem_in"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_sigtimeout"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_phyIf"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_encoding"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_noclock_to"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_hdr1"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_hdr2"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
    }
    if ( chompVal.c_str() != NULL && (strcmp(chompVal.c_str(), "thom-tvt2") == 0) )
    {
        fputs (link_config_map["rdr_max_messages"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["rdr_message_timeout"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["rdr_data_acks"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["rdr_options1"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_baud"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_timeout_baud"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_maxmesg_size"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_maxmesg_cnt"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_fwd_time"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_dma_blocksize"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_sync"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_check_crc"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_modem_in"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_sigtimeout"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_phyIf"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_encoding"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_noclock_to"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
    }
    if ( chompVal.c_str() != NULL && (strcmp(chompVal.c_str(), "toshiba") == 0) )
    {
        fputs (link_config_map["rdr_max_messages"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["rdr_message_timeout"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["rdr_data_acks"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["rdr_options1"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_baud"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_timeout_baud"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_maxmesg_size"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_maxmesg_cnt"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_fwd_time"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_dma_blocksize"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_sync"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_check_crc"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_time_delay"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_modem_in"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_sigtimeout"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_phyIf"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_encoding"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["tc_noclock_to"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
    }
    else
    {
	
	//do nothing
    }

        fputs (link_config_map["serial_throttle"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["stats_freq"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["ser_to_mcast"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["xmt_addr"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["xmt_port"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["xmt_ttl"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["xmt_link_options1"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["mcast_to_ser"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["rcv_addr"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["rcv_port"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["rcv_ttl"].c_str(), file);
        fputs (fileNewLine.c_str(), file);
        fputs (link_config_map["rcv_link_options1"].c_str(), file);


    fclose (file);
    fclose (file2);



}
