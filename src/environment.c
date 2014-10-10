#include <stdio.h>
#include <time.h>

static int temperature = 25;

#define BOUNDARIES( min, val, max) \
    if( val<min) val=min; else if( val>max) val=max
static unsigned int seed = 0x12345678;
int random( unsigned int minval, unsigned int maxval) {
    seed += seed*seed | 5;
    return seed % (maxval-minval) + minval;
}

double getHumidity() {
    static double humidity = 50;
    humidity += random(-100, +100) / 10;
    BOUNDARIES( 0, humidity, 100);
    return humidity;
}

double getLuminosity() {
    static double luminosity = 80;
    luminosity += random(-100, +100) / 20;
    BOUNDARIES( 0, luminosity, 100);
    return luminosity;
}

int getTemperature()
{
	return temperature;
}

int getTimeStamp()  /* This one isn't a mock-up! */
{
    time_t stime = time(NULL);
    return stime;
}


/******************************************************************************************************************************************/
/*  Object     : This handler is called by command "at+temperature", it allows to simulate a temperature sensor      			          */
/*				 Ex : "at+temperature=?" return the current temperature , "at+temperature=value" will set the current temperature=value   */
/******************************************************************************************************************************************/
/*void temperatureHandler( adl_atCmdPreParser_t * paras ){
	switch (paras->Type)
	{
		case ADL_CMD_TYPE_READ:
		{
			char result[100];
			wm_sprintf(result, "\r\n+temperature: %d\r\n", temperature);
			TRACE(( 1, "+temperature: %d", temperature ));
			adl_atSendResponse(ADL_AT_PORT_TYPE ( paras->Port, ADL_AT_RSP ), result);
			break;
		}
		case ADL_CMD_TYPE_PARA:
		{
			temperature = wm_atoi(ADL_GET_PARAM ( paras, 0 ) );  // 0 get FIRST_PARAMETER
			TRACE(( 1, "+new temperature: %d", temperature ));
			break;
		}
		default:
		{
			TRACE(( 1, "paras->Type  %d", paras->Type ));
			break;
		}
	}
	adl_atSendStdResponsePort(ADL_AT_RSP, paras->Port, ADL_STR_OK);
}*/
