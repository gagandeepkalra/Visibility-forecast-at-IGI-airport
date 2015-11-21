
#include <string>
#include <assert.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include "stationData.h"
#include <sstream>
#include <time.h>
#include <getopt.h>

#ifdef SQL
#include "../sql/sqlSession.h"
#include "../sql/pushDB.h"
std::string sqldbname, sqlusername, sqlpassword;
int toDB = false;
#endif

std::vector<std::string> fieldList;
std::vector<int> stationList;
char *explicitDateStrYYYYMMDD = NULL;

static int toDigit(char c)
{
    if(c >= '0' || c <='9')
        return c - '0';

    return -99;
}

/*
 * Function naming convention followed
 * the number after last underscore tells to which section the function belongs to
 */

//stores all verbose data required to be printed
static std::unordered_map<std::string, std::string> hash;

/* while decoding if some error occurs, decodeError is set true.
 * if decodeError = true, none of the decoding gets printed
 *
 */
static bool decodeError = false;

static bool iihVV_1(std::string &w)
{
    int ir = toDigit(w[0]);
    int ix = toDigit(w[1]);

    if( ! (ir>=0 && ir<=4 && ix>=1 && ix<=7) ) {
        decodeError = true;
        return false;
    }

    std::string precipitationIndicator;

    switch (ir) {
    case 0:
        precipitationIndicator = "\"Precipitation in groups 1 and 3\"";
        break;

    case 1:
        precipitationIndicator = "\"Precipitation reported in group 1 only\"";
        break;

    case 2:
        precipitationIndicator = "\"Precipitation reported in group 3 only\"";
        break;

    case 3:
        precipitationIndicator = "\"Precipitation omitted, no precipitation\"";
        break; //if a csv file data field contains comma, the whole data should be enclosed in double quotes

    case 4:
        precipitationIndicator = "\"Precipitation omitted, no observation\"";
        break;

    default:
        return false;
    }

    std::string stationType;

    switch (ix) {
    case 1:
        stationType = "\"manned station -- weather group included\"";
        break;

    case 2:
        stationType = "\"manned station -- omitted, no significant weather\"";
        break;

    case 3:
        stationType = "\"manned station -- omitted, no weather observation\"";
        break;

    case 4:
        stationType = "\"automated station -- weather group included (see automated weather codes 4677 and 4561)\"";
        break;

    case 5:
        stationType = "\"automated station -- omitted, no significant weather\"";
        break;

    case 6:
        stationType = "\"automated station -- omitted, no weather observation\"";
        break;

    case 7:
        stationType = "\"automated station -- weather group included (see automated weather codes 4680 and 4531)\"";
        break;

    default:
        return false;
    }

    int h = toDigit(w[2]); //  can be missing
    std::string cloudBase = "\"nunknown\"";

    if(h != -1)
        switch(h) {
        case 0:
            cloudBase = "\"0 to 50 m\"";
            break;

        case 1:
            cloudBase = "\"50 to 100 m\"";
            break;

        case 2:
            cloudBase = "\"100 to 200 m\"";
            break;

        case 3:
            cloudBase = "\"200 to 300 m\"";
            break;

        case 4:
            cloudBase = "\"300 to 600 m\"";
            break;

        case 5:
            cloudBase = "\"600 to 1000 m\"";
            break;

        case 6:
            cloudBase = "\"1000 to 1500 m\"";
            break;

        case 7:
            cloudBase = "\"1500 to 2000 m\"";
            break;

        case 8:
            cloudBase = "\"2000 to 2500 m\"";
            break;

        case 9:
            cloudBase = "\"above 2500 m or higher or no clouds\"";
            break;
        }

    int VV = toDigit(w[3]) * 10 + toDigit(w[4]);  //can be missing
    std::string visibility = "\"unknown\"";

    if(VV > 0) {
        if(VV >= 0 && VV <= 50)
            visibility = VV/10;
        else if(VV >=56 && VV <= 80)
            visibility = std::to_string(VV - 50);
        else if(VV >= 81 && VV <= 88)
            visibility = std::to_string((VV -80)*5 + 30);
        else if(VV == 89)
            visibility = "\"greater that 70\"";
        else if(VV == 90)
            visibility = "\"less than 0.05\"";
        else
            switch(VV) {
            case 91:
                visibility = "\"0.05\"";
                break;

            case 92:
                visibility = "\"0.2\"";
                break;

            case 93:
                visibility = "\"0.5\"";
                break;

            case 94:
                visibility = "\"1\"";
                break;

            case 95:
                visibility= "\"2\"";
                break;

            case 96:
                visibility= "\"4\"";
                break;

            case 97:
                visibility= "\"10\"";
                break;

            case 98:
                visibility = "\"20\"";
                break;

            case 99:
                visibility = "\"greater than 50\"";
                break;
            }
    }

    hash["precipitation indicator"] = precipitationIndicator;
    hash["station type"] = stationType;
    hash["cloud base of lowest cloud seen (m)"] = cloudBase;
    hash["Visibility"] = visibility;

    return true;
}

static bool Nddff_1(std::string &w)
{
    int N = toDigit(w[0]);
    std::string cloudCover = "\"No measurement made\"";

    //oktas
    if(N != -1)
        switch(N) {
        case 0:
            cloudCover = "\"0  (clear)\"";
            break;

        case 1:
            cloudCover = "\"1/8th or less \"";
            break;

        case 2:
            cloudCover = "\"2/8ths\"";
            break;

        case 3:
            cloudCover = "\"3/8ths\"";
            break;

        case 4:
            cloudCover = "\"4/8ths\"";
            break;

        case 5:
            cloudCover = "\"5/8ths\"";
            break;

        case 6:
            cloudCover = "\"6/8ths\"";
            break;

        case 7:
            cloudCover = "\"7/8ths or more, but not 8/8\"";
            break;

        case 8:
            cloudCover = "\"8/8ths (overcast)\"";
            break;

        case 9:
            cloudCover = "\"sky obscured or cannot be estimated\"";
            break;
        }

    std::string windDirection;
    int dd= toDigit(w[1]) *10 + toDigit(w[2]);

    if(dd == 99)
        windDirection = "\" Variable, or all directions,  or unknown, or waves confused, direction indeterminate.\"";
    else {
        dd *=  10; //todo: short method applied now, use correct decoding
        windDirection = std::to_string(dd);
    }

    int surfaceWindSpeed;
    int ff = toDigit(w[3])*10 + toDigit(w[4]);

    if(ff == 0)
        surfaceWindSpeed = 0;
    else if(ff >= 4 && ff <= 6)
        surfaceWindSpeed = 2;
    else if(ff >= 7 && ff <= 10)
        surfaceWindSpeed = 9;
    else if(ff >= 11 && ff <= 16)
        surfaceWindSpeed = 13;
    else if(ff >= 17 && ff <= 21)
        surfaceWindSpeed = 18;
    else if(ff >= 22 && ff <= 27)
        surfaceWindSpeed = 24;
    else if(ff >= 28 && ff <= 33)
        surfaceWindSpeed = 30;
    else if(ff >= 34 && ff <= 40)
        surfaceWindSpeed = 37;
    else if(ff >= 41 && ff <= 47)
        surfaceWindSpeed = 44;
    else if(ff >= 48 && ff <= 55)
        surfaceWindSpeed = 52;
    else if(ff >= 56  && ff <= 63)
        surfaceWindSpeed = 60;
    else
        surfaceWindSpeed = -99; //speed in 00fff group

    hash["cloud cover"] = cloudCover;
    hash["wind direction"] = windDirection; //speed units given by speed indicator field
    hash["surface wind speed"] = std::to_string(surfaceWindSpeed);

    return true;
}

static bool _00fff_1(std::string &w)
{
    int d1 = toDigit(w[0]);
    int d2 = toDigit(w[2]);

    if( ! (d1 == 0 && d2 == 0))
        return false;

    int fff = toDigit(w[2])*100 + toDigit(w[3])*10 + toDigit(w[4]);

    hash["optional wind speed"] = fff;

    return true;
}

static void _1sTTT_1(std::string &w)
{
    int sign = toDigit(w[1]);

    float TTT = toDigit(w[2])*10 + toDigit(w[3]) + toDigit(w[4])/10.0;

    if(sign == 1)
        TTT = -TTT;

    hash["temperature (C)"] = std::to_string(TTT);
}

static void _2sTTT_29UUU_1(std::string &w)
{
    int TTT = toDigit(w[2])*10 + toDigit(w[3]) + toDigit(w[4])/10.0;

    if(w[1] == '1' || w[1] == '0') {
        hash["dewpoint (C)"] =  std::to_string(-TTT);
    } else if(w[1] == '9')
        hash["relative humidity (%)"] =  std::to_string(TTT * 10);
}

static void _3PPPP_1(std::string &w)
{
    float PPPP = toDigit(w[1])*100 + toDigit(w[2])*10 + toDigit(w[3]) + toDigit(w[4])/10.0;

    //todo: unit is not clear, and decoding is not correct
    if(w[1] == '1')
        PPPP /= 1000;

    hash["station pressure (hPa)"] = std::to_string(PPPP);

}

static void _4PPPP_1(std::string &w)
{
    float PPPP = toDigit(w[1])*100 + toDigit(w[2])*10 + toDigit(w[3]) + toDigit(w[4])/10;

    //todo: unit is not clear, and decoding is not correct
    if(w[1] == '1')
        PPPP /= 1000;

    //todo: check the unit
    hash["sea level pressure (hPa)"] = std::to_string(PPPP);
}

static void _5appp_1(std::string &w)
{
    std::string pressureTendency;
    int a = toDigit(w[1]);

    switch(a) {
    case 0:
        pressureTendency = "\"Increasing, then decreasing -- resultant pressure same or higher\"";
        break;

    case 1:
        pressureTendency = "\"Increasing, then steady -- resultant pressure higher\"";
        break;

    case 2:
        pressureTendency = "\"Increasing (steadily or unsteadily) -- resultant pressure higher\"";
        break;

    case 3:
        pressureTendency = "\"Decreasing or steady, then increasing; or increasing, then increasing more rapidly -- resultant pressure higher\"";
        break;

    case 4:
        pressureTendency = "\"Steady -- resultant pressure same\"";
        break;

    case 5:
        pressureTendency = "\"Decreasing, then increasing; atmospheric pressure the same or lower than 3 hours ago\"";
        break;

    case 6:
        pressureTendency = "\"Decreasing, then steady; or decreasing, then decreasing more slowly -- resultant pressure lower\"";
        break;

    case 7:
        pressureTendency = "\"Decreasing (steadily or unsteadily) -- resultant pressure lower\"";
        break;

    case 8:
        pressureTendency = "\" Steady or increasing, then decreasing; or decreasing, then decreasing more rapidly. -- resultant pressure lower\"";
        break;
    }


    float ppp = toDigit(w[2])*10 + toDigit(w[3]) + toDigit(w[4])/10.0;

    hash["pressure tendency"] = pressureTendency;
    hash["pressure change in last 3hrs (hPa)"] = std::to_string(ppp);
}

static void _6RRRt_1(std::string &w)
{
    int RRR = toDigit(w[1])*100 + toDigit(w[2])*10 + toDigit(w[3]);
    std::string precipitation;

    if(RRR < 989)
        precipitation = std::to_string(RRR);
    else if(RRR == 989)
        precipitation = "\"989 or more\"";
    else if(RRR == 990)
        precipitation = "\"Trace\"";
    else
        precipitation = std::to_string((RRR%10)/10.0) ;


    int t = toDigit(w[4]);

    std::string duration;

    if(t == -1)
        duration = "\"24\"";
    else
        switch(t) {
        case 1:
            duration = "\"6\"";
            break;

        case 2:
            duration = "\"12\"";
            break;

        case 3:
            duration = "\"18\"";
            break;

        case 4:
            duration = "\"24\"";
            break;

        case 5:
            duration = "\"1\"";
            break;

        case 6:
            duration = "\"2\"";
            break;

        case 7:
            duration = "\"3\"";
            break;

        case 8:
            duration = "\"9\"";
            break;

        case 9:
            duration = "\"15\"";
            break;
        }

    hash["liquid precipitation amount Sec 1 (mm)"] = std::to_string(RRR);
    hash["liquid precipitation duration Sec 1 (hrs)"] = duration;
}

static void _7wwWW_1(std::string &w)
{
    int ww = toDigit(w[1])*10 + toDigit(w[2]);
    std::string presentWeather;

    switch (ww) {
    case 0:
        presentWeather = "\"Cloud development not observed or not observable\"";
        break;

    case 1:
        presentWeather = "\"Clouds  generally dissolving or becoming less developed\"";
        break;

    case 2:
        presentWeather = "\"State of sky on the whole unchanged\"";
        break;

    case 3:
        presentWeather = "\" Clouds generally forming or developing\"";
        break;

    case 4:
        presentWeather = "\"Visibility reduced by smoke, e.g. veldt or forest fires, industrial smoke or volcanic ashes\"";
        break;

    case 5:
        presentWeather = "\"Haze\"";
        break;

    case 6:
        presentWeather = "\"Widespread dust in suspension in the air, not raised by wind at or near the station at the time of observation\"";
        break;

    case 7:
        presentWeather = "\"Dust or sand raised by wind at or near the station at the time of observation, but no well-developed dust whirl(s) or sand whirl(s), and no duststorm or sandstorm seen; or, in the case of ships, blowing spray at the station\"";
        break;

    case 8:
        presentWeather = "\"Well-developed dust whirl(s) or sand whirl(s) seen at or near the station during the preceding hour or at the time of observation, but no duststorm or sandstorm\"";
        break;

    case 9:
        presentWeather = "\"Duststorm or sandstorm within sight at the time of observation, or at the station during the preceding hour\"";
        break;

    case 10:
        presentWeather = "\"Mist\"";
        break;

    case 11:
        presentWeather = "\"patches of shallow fog\"";
        break;

    case 12:
        presentWeather = "\"continuous shallow fog\""; //todo: seems incorrect
        break;

    case 13:
        presentWeather = "\"lightning visible, no thunder heard\"";
        break;

    case 14:
        presentWeather = "\"precipitation within sight but not hitting ground\"";
        break;

    case 15:
        presentWeather = "\"Precipitation within sight, reaching the ground or the surface of the sea, but distant, i.e. estimated to be more than 5 km from the station\"";
        break;

    case 16:
        presentWeather = "\"Precipitation within sight, reaching the ground or the surface of the sea, near to, but not at the station\"";
        break;

    case 17:
        presentWeather = "\"Thunderstorm, but no precipitation at the time of observation\"";
        break;

    case 18:
        presentWeather = "\"squalls within sight but no precipitation falling at station\"";
        break;

    case 19:
        presentWeather = "\"funnel clouds within sight\"";
        break;

    case 20:
        presentWeather = "\"drizzle\"";
        break;

    case 21:
        presentWeather = "\"rain\"";
        break;

    case 22:
        presentWeather = "\"snow\"";
        break;

    case 23:
        presentWeather = "\"rain and snow\"";
        break;

    case 24:
        presentWeather = "\"freezing rain\"";
        break;

    case 25:
        presentWeather = "\"rain showers\"";
        break;

    case 26:
        presentWeather = "\"snow showers\"";
        break;

    case 27:
        presentWeather = "\"hail showers\"";
        break;

    case 28:
        presentWeather = "\"fog\"";
        break;

    case 29:
        presentWeather = "\"thunderstorms\"";
        break;

    case 30:
        presentWeather = "\"slight to moderate duststorm, decreasing in intensity\"";
        break;

    case 31:
        presentWeather = "\"slight to moderate duststorm, no change\"";
        break;

    case 32:
        presentWeather = "\"slight to moderate duststorm, increasing in intensity\"";
        break;

    case 33:
        presentWeather = "\"severe duststorm, decreasing in intensity\"";
        break;

    case 34:
        presentWeather = "\"severe duststorm, no change\"";
        break;

    case 35:
        presentWeather = "\"severe duststorm, increasing in intensity\"";
        break;

    case 36:
        presentWeather = "\"slight to moderate drifting snow, below eye level\"";
        break;

    case 37:
        presentWeather = "\"heavy drifting snow, below eye level\"";
        break;

    case 38:
        presentWeather = "\"slight to moderate drifting snow, above eye level\"";
        break;

    case 39:
        presentWeather = "\"heavy drifting snow, above eye level\"";
        break;

    case 40:
        presentWeather = "\"Fog at a distance\"";
        break;

    case 41:
        presentWeather = "\"patches of fog\"";
        break;

    case 42:
        presentWeather = "\"fog, sky visible, thinning\"";
        break;

    case 43:
        presentWeather = "\"fog, sky not visible, thinning\"";
        break;

    case 44:
        presentWeather = "\"fog, sky visible, no change\"";
        break;

    case 45:
        presentWeather = "\"fog, sky not visible, no change\"";
        break;

    case 46:
        presentWeather = "\"fog, sky visible, becoming thicker\"";
        break;

    case 47:
        presentWeather = "\"fog, sky not visible, becoming thicker\"";
        break;

    case 48:
        presentWeather = "\"fog, depositing rime, sky visible\"";
        break;

    case 49:
        presentWeather = "\"fog, depositing rime, sky not visible\"";
        break;

    case 50:
        presentWeather = "\"intermittent light drizzle\"";
        break;

    case 51:
        presentWeather = "\"continuous light drizzle\"";
        break;

    case 52:
        presentWeather = "\"intermittent moderate drizzle\"";
        break;

    case 53:
        presentWeather = "\"continuous moderate drizzle\"";
        break;

    case 54:
        presentWeather = "\"intermittent heavy drizzle\"";
        break;

    case 55:
        presentWeather = "\"continuous heavy drizzle\"";
        break;

    case 56:
        presentWeather = "\"light freezing drizzle\"";
        break;

    case 57:
        presentWeather = "\"moderate to heavy freezing drizzle\"";
        break;

    case 58:
        presentWeather = "\"light drizzle and rain\"";
        break;

    case 59:
        presentWeather = "\"moderate to heavy drizzle and rain\"";
        break;

    case 60:
        presentWeather = "\"intermittent light rain\"";
        break;

    case 61:
        presentWeather = "\"continuous light rain\"";
        break;

    case 62:
        presentWeather = "\"intermittent moderate rain\"";
        break;

    case 63:
        presentWeather = "\"continuous moderate rain\"";
        break;

    case 64:
        presentWeather = "\"intermittent heavy rain\"";
        break;

    case 65:
        presentWeather = "\"continuous heavy rain\"";
        break;

    case 66:
        presentWeather = "\"light freezing rain\"";
        break;

    case 67:
        presentWeather = "\"moderate to heavy freezing rain\"";
        break;

    case 68:
        presentWeather = "\"light rain and snow\"";
        break;

    case 69:
        presentWeather = "\"moderate to heavy rain and snow\"";
        break;

    case 70:
        presentWeather = "\"intermittent light snow\"";
        break;

    case 71:
        presentWeather = "\"continuous light snow\"";
        break;

    case 72:
        presentWeather = "\"intermittent moderate snow\"";
        break;

    case 73:
        presentWeather = "\"continuous moderate snow\"";
        break;

    case 74:
        presentWeather = "\"intermittent heavy snow\"";
        break;

    case 75:
        presentWeather = "\"continuous heavy snow\"";
        break;

    case 76:
        presentWeather = "\"diamond dust\"";
        break;

    case 77:
        presentWeather = "\"snow grains\"";
        break;

    case 78:
        presentWeather = "\"snow crystals\"";
        break;

    case 79:
        presentWeather = "\"ice pellets\"";
        break;

    case 80:
        presentWeather = "\"light rain showers\"";
        break;

    case 81:
        presentWeather = "\"moderate to heavy rain showers\"";
        break;

    case 82:
        presentWeather = "\"violent rain showers\"";
        break;

    case 83:
        presentWeather = "\"light rain and snow showers\"";
        break;

    case 84:
        presentWeather = "\"moderate to heavy rain and snow showers\"";
        break;

    case 85:
        presentWeather = "\"light snow showers\"";
        break;

    case 86:
        presentWeather = "\"moderate to heavy snow showers\"";
        break;

    case 87:
        presentWeather = "\"light snow/ice pellet showers\"";
        break;

    case 88:
        presentWeather = "\"moderate to heavy snow/ice pellet showers\"";
        break;

    case 89:
        presentWeather = "\"light hail showers\"";
        break;

    case 90:
        presentWeather = "\"moderate to heavy hail showers\"";
        break;

    case 91:
        presentWeather = "\"thunderstorm in past hour, currently only light rain\"";
        break;

    case 92:
        presentWeather = "\"thunderstorm in past hour, currently only moderate to heavy rain\"";
        break;

    case 93:
        presentWeather = "\"thunderstorm in past hour, currently only light snow or rain/snow mix\"";
        break;

    case 94:
        presentWeather = "\"thunderstorm in past hour, currently only moderate to heavy snow or rain/snow mix\"";
        break;

    case 95:
        presentWeather = "\"light to moderate thunderstorm\"";
        break;

    case 96:
        presentWeather = "\"light to moderate thunderstorm with hail\"";
        break;

    case 97:
        presentWeather = "\"heavy thunderstorm\"";
        break;

    case 98:
        presentWeather = "\"heavy thunderstorm with duststorm\"";
        break;

    case 99:
        presentWeather = "\"heavy thunderstorm with hail\"";
        break;
    }

    int W1 = toDigit(w[3]);
    std::string type1;

    switch (W1) {
    case 0:
        type1 = "\"cloud covering less than half of sky\"";
        break;

    case 1:
        type1 = "\"cloud covering more than half of sky during part of period and more than half during part of period\"";
        break;

    case 2:
        type1 = "\"cloud covering more than half of sky\"";
        break;

    case 3:
        type1 = "\"sandstorm, duststorm or blowing snow\"";
        break;

    case 4:
        type1 = "\"fog or ice fog orthick haze\"";
        break;

    case 5:
        type1 = "\"drizzle\"";
        break;

    case 6:
        type1 = "\"rain\"";
        break;

    case 7:
        type1 = "\"snow or mixed rain and snow\"";
        break;

    case 8:
        type1 = "\"showers\"";
        break;

    case 9:
        type1 = "\"Thunderstorm(s) with or without precipitation\"";
        break;
    }

    int W2 = toDigit(w[4]);
    std::string type2;

    switch (W2) {
    case 0:
        type2 = "\"cloud covering less than half of sky\"";
        break;

    case 1:
        type2 = "\"cloud covering more than half of sky during part of period and more than half during part of period\"";
        break;

    case 2:
        type2 = "\"cloud covering more than half of sky\"";
        break;

    case 3:
        type2 = "\"sandstorm, duststorm or blowing snow\"";
        break;

    case 4:
        type2 = "\"fog or ice fog orthick haze\"";
        break;

    case 5:
        type2 = "\"drizzle\"";
        break;

    case 6:
        type2 = "\"rain\"";
        break;

    case 7:
        type2 = "\"snow or mixed rain and snow\"";
        break;

    case 8:
        type2 = "\"showers\"";
        break;

    case 9:
        type2 = "\"Thunderstorm(s) with or without precipitation\"";
        break;
    }

    hash["present weather"] = presentWeather;
    hash["past weather (most significant)"] = type1;
    hash["past weather (second most significant)"] = type2;
}

static void _8NCCC_1(std::string &w)
{
    int N = toDigit(w[1]);
    std::string cloudCover;

    //oktas
    if(N != -1)
        switch(N) {
        case 0:
            cloudCover = "\"0  (clear)\"";
            break;

        case 1:
            cloudCover = "\"1/8th or less \"";
            break;

        case 2:
            cloudCover = "\"2/8ths\"";
            break;

        case 3:
            cloudCover = "\"3/8ths\"";
            break;

        case 4:
            cloudCover = "\"4/8ths\"";
            break;

        case 5:
            cloudCover = "\"5/8ths\"";
            break;

        case 6:
            cloudCover = "\"6/8ths\"";
            break;

        case 7:
            cloudCover = "\"7/8ths or more, but not 8/8\"";
            break;

        case 8:
            cloudCover = "\"8/8ths (overcast)\"";
            break;

        case 9:
            cloudCover = "\"sky obscured or cannot be estimated\"";
            break;
        }

    //lowCloudType
    std::string lowCloudType;
    int Cl = toDigit(w[2]);

    if(Cl == -1)
        lowCloudType = "\"low clouds unobserved due to darkness or obscuration\"";
    else
        switch(Cl) {
        case 0:
            lowCloudType = "\"no low clouds\"";
            break;

        case 1:
            lowCloudType = "\"cumulus humulis or fractus (no vertical development)\"";
            break;

        case 2:
            lowCloudType = "\"cumulus mediocris or congestus (moderate vertical development)\"";
            break;

        case 3:
            lowCloudType = "\"cumulonimbus calvus (no outlines nor anvil)\"";
            break;

        case 4:
            lowCloudType = "\"stratocumulus cumulogenitus (formed by spreading of cumulus)\"";
            break;

        case 5:
            lowCloudType = "\"stratocumulus\"";
            break;

        case 6:
            lowCloudType = "\"stratus nebulosus (continuous sheet)\"";
            break;

        case 7:
            lowCloudType = "\"stratus or cumulus fractus (bad weather)\"";
            break;

        case 8:
            lowCloudType = "\"cumulus and stratocumulus (multilevel)\"";
            break;

        case 9:
            lowCloudType = "\"cumulonimbus with anvil\"";
            break;
        }

    //middleCloudType
    int Cm = toDigit(w[3]);
    std::string middleCloudType;

    if(Cm == -1)
        middleCloudType = "\"middle clouds unobserved due to darkness or obscuration\"";
    else
        switch(Cm) {
        case 0:
            middleCloudType = "\"no middle clouds\"";
            break;

        case 1:
            middleCloudType = "\"altostratus translucidous (mostly transparent)\"";
            break;

        case 2:
            middleCloudType = "\"altostratus opacus or nimbostratus\"";
            break;

        case 3:
            middleCloudType = "\"altocumulus translucidous (mostly transparent)\"";
            break;

        case 4:
            middleCloudType = "\"patches of altocumulus (irregular, lenticular)\"";
            break;

        case 5:
            middleCloudType = "\"bands of altocumulus\"";
            break;

        case 6:
            middleCloudType = "\"altocumulus cumulogenitus (formed by spreading of cumulus)\"";
            break;

        case 7:
            middleCloudType = "\"altocumulus (multilayers)\"";
            break;

        case 8:
            middleCloudType = "\"altocumulus castellanus (having cumuliform tufts)\"";
            break;

        case 9:
            middleCloudType = "\"altocumulus of a chaotic sky\"";
            break;
        }

    //highCloudType
    std::string highCloudType;
    int Ch = toDigit(w[4]);

    if(Ch == -1)
        highCloudType = "\"middle clouds unobserved due to darkness or obscuration\"";
    else
        switch(Ch) {
        case 0:
            highCloudType = "\"no high clouds\"";
            break;

        case 1:
            highCloudType = "\"cirrus fibratus (wispy)\"";
            break;

        case 2:
            highCloudType = "\"cirrus spissatus (dense in patches)\"";
            break;

        case 3:
            highCloudType = "\"cirrus spissatus cumulogenitus (formed out of anvil)\"";
            break;

        case 4:
            highCloudType = "\"cirrus unicus or fibratus (progressively invading sky)\"";
            break;

        case 5:
            highCloudType = "\"bands of cirrus or cirrostratus invading sky (less than 45 degree above horizon)\"";
            break;

        case 6:
            highCloudType = "\"bands of cirrus or cirrostratus invading sky (more than 45 degree above horizon)\"";
            break;

        case 7:
            highCloudType = "\"cirrostratus covering whole sky\"";
            break;

        case 8:
            highCloudType = "\"cirrostratus not covering sky but not invading\"";
            break;

        case 9:
            highCloudType = "\"cirrocumulus\"";
            break;
        }

    hash["amount of lower(low/middle) cloud cover"] = cloudCover;
    hash["low cloud type"] = lowCloudType;
    hash["middle cloud type"] = middleCloudType;
    hash["high cloud type"] = highCloudType;
}

static void _9GGgg_1(std::string &w)
{
    int GG = toDigit(w[1])*10 + toDigit(w[2]);
    int gg = toDigit(w[3])*10 + toDigit(w[4]);

    hash["time of oberservation (hh:mm)"] = std::to_string(GG) + ":" + std::to_string(gg);
}

static std::vector<stationData> stationDataVector;

//loads stationDataVector with weather station data
void initialize()
{
    std::ifstream stationCodeHash;
    //make sure the file is sorted by station code, as getStationData function is dependent on this fact
    stationCodeHash.open ("./stationData.csvToStationDataHash/stationDataHash", std::ifstream::in);

    if(!stationCodeHash.is_open()) {
        std::cerr<<"File not found\nFile: ./stationData.csvToStationDataHash/stationDataHash\n";
        exit(EXIT_FAILURE);
    }

    //discard first line (csv heading)
    std::string buffer;
    std::getline(stationCodeHash, buffer);

    stationCodeHash>>stationDataVector;

    if(stationDataVector.size() == 0) {
        std::cerr << "stationDataVector size is zero. Please check that stationDataHash is in correct format\n";
        abort();
    }
}

/*returnValue: weather station data from stationDataVector
 * parameter: station code of the weather station whose data is required
 */
static stationData getSationData(int stationCode)
{
    if(stationDataVector.size() == 0) {
        std::cerr << "stationDataVector size is zero. Check if initialize() function got executed properly\n";
        abort();
    }

    int low = 0, high = stationDataVector.size() - 1, mid;

    while(low <= high) {
        mid = low + ((high - low) / 2);

        std::string &sCodeStr = stationDataVector[mid].station_code;
        int currCode = 0, len = sCodeStr.length();

        for(int i = 0; i<len; ++i)
            currCode = currCode*10 + (sCodeStr[i] - '0');

        if(currCode == stationCode)
            return stationDataVector[mid];

        if(currCode < stationCode)
            low = mid + 1;
        else
            high = mid - 1;
    }

    std::cerr << "error: unknown station code :"<<stationCode<<": data requested\n";
    decodeError = true;
    return stationData();
}

void section0(char *l)
{
    std::string line(l);
    //(now->tm_year + 1900) << '-' << (now->tm_mon + 1) << '-' <<  now->tm_mday;

    std::vector<std::string> list = split(line, ' ');
    std::string &gmtTime = list[1];

    time_t t = time(0);   // get time now
    struct tm *now = localtime( & t );
    int day, month, year;

    if(explicitDateStrYYYYMMDD == NULL) {
        year = now->tm_year + 1900;
        month = now->tm_mon + 1;
        day = toDigit(gmtTime[0])*10 + toDigit(gmtTime[1]);
    } else {
        char *d = explicitDateStrYYYYMMDD;
        //assumption: date validation has been already performed
        year = (d[0] - '0')*1000 + (d[1] - '0')*100 + (d[2] - '0')*10 + (d[3] - '0');
        month = (d[4] - '0')*10 + (d[5] - '0');
        day = (d[6] - '0')*10 + (d[7] - '0');
    }

    int nearestHour = toDigit(gmtTime[2])*10 + toDigit(gmtTime[3]);

    int windIndicator = toDigit(gmtTime[4]);
    std::string windIndicatorStr;

    switch (windIndicator) {
    case 0:
        windIndicatorStr = "\"wind speed estimated (metres/sec)\"";
        break;

    case 1:
        windIndicatorStr = "\"wind speed form anemometer (metres/sec)\"";
        break;

    case 3:
        windIndicatorStr = "\"wind speed estimated (knots/sec)\"";
        break;

    case 4:
        windIndicatorStr = "\"wind speed form anemometer (knots/sec)\"";
        break;

    default:
        windIndicatorStr = "\"unknown\"";
        break;
    }

    hash["year"] = std::to_string(year);
    hash["month"] = std::to_string(month);
    hash["day"] = std::to_string(day);
    hash["nearest hour"] = std::to_string(nearestHour);
    hash["wind indicator"] = windIndicatorStr;
    hash["wind indicator dig"] = std::to_string(windIndicator);

}

void section1(char *l)
{
    std::string line(l);
    std::vector<std::string> list = split(line, ' ');
    std::string &stationC = list[0];
    int stationCode = toDigit(stationC[0])*10000 +  toDigit(stationC[1])*1000 + toDigit(stationC[2])*100
                      + toDigit(stationC[3])*10 + toDigit(stationC[4]);
    stationData data = getSationData(stationCode);

    hash["station code"] = std::to_string(stationCode);
    hash["station place name"] = data.place_name;
    hash["station latitude"] = data.lat;
    hash["station longitude"] = data.lon;

    iihVV_1(list[1]);
    Nddff_1(list[2]);

    if(hash["surface wind speed"] == std::to_string(-99))
        if(toDigit(list[3][0]) == 0 && toDigit(list[3][1]) == 0)
            _00fff_1(list[3]);

    //for (std::string list) {
    for(int i = 3; i<list.size(); ++i) {
        std::string &w = list[i];

        int msd = toDigit(w[0]);

        switch (msd) {
        case 1:
            _1sTTT_1(w);
            break;

        case 2:
            _2sTTT_29UUU_1(w);
            break;

        case 3:
            _3PPPP_1(w);
            break;

        case 4:
            _4PPPP_1(w);
            break;

        case 5:
            _5appp_1(w);
            break;

        case 6:
            _6RRRt_1(w);
            break;

        case 7:
            _7wwWW_1(w);
            break;

        case 8:
            _8NCCC_1(w);
            break;

        case 9:
            _9GGgg_1(w);
            break;

        default:
            break;
        }
    }
}

void section2(char *l)
{

}

static void _1sTTT_3(std::string &w)
{
    int sign = toDigit(w[1]);

    float TTT = toDigit(w[2])*10 + toDigit(w[3]) + toDigit(w[4])/10.0;

    if(sign == 1)
        TTT = -TTT;

    //todo: time group for india
    hash["max temp (C)"] = std::to_string(TTT);
}

static void _2sTTT_3(std::string &w)
{
    int sign = toDigit(w[1]);

    float TTT = toDigit(w[2])*10 + toDigit(w[3]) + toDigit(w[4])/10;

    if(sign == 1)
        TTT = -TTT;

    hash["min temp last 24hr (C)"] = std::to_string(TTT);
}

static void _3Ejjj_3(std::string &w)
{
    float Ejjj = toDigit(w[1]);
    // not sure about this ...
    hash["State of the ground without snow or measurable ice cover"] = std::to_string(Ejjj);
}

static void _4Esss_3(std::string &w)
{
    int E4 = toDigit(w[1]);
    std::string StateOfGround;

    switch(E4) {
    case 0:
        StateOfGround = "\"predominantly covered with ice\"";
        break;

    case 1:
        StateOfGround = "\"compact or wet snow covering less than half of ground\"";
        break;

    case 2:
        StateOfGround = "\"compact or wet snow covering more than half of ground but not completely covered\"";
        break;

    case 3:
        StateOfGround = "\"even layer of compact or wet snow covering entire ground\"";
        break;

    case 4:
        StateOfGround = "\"uneven layer of compact or wet snow covering entire ground\"";
        break;

    case 5:
        StateOfGround= "\"loose dry snow covering less than half of ground\"";
        break;

    case 6:
        StateOfGround= "\"loose dry snow covering more than half of ground but not completely covered\"";
        break;

    case 7:
        StateOfGround= "\"even layer of loose dry snow covering entire ground\"";
        break;

    case 8:
        StateOfGround= "\"uneven layer of loose dry snow covering entire ground\"";
        break;

    case 9:
        StateOfGround= "\"snow covering ground completely with deep drifts\"";
        break;
    }

    float sss = toDigit(w[2])*100 + toDigit(w[3])*10 + toDigit(w[4]);

    std::string depth;

    if(sss == 0)
        depth= "\"Not used\"";
    else if(sss > 0 && sss < 997)
        depth = std::to_string(sss);
    else if(sss == 997)
        depth= "\"Less that 0.5\"";
    else if(sss == 998)
        depth= "\"Snow cover not continuous\"";
    else
        depth= "\"Measurement impossible or inaccurate\"";


    hash["snow depth (cm)"] = depth;
    hash["state of ground"] = StateOfGround;
}

static void _5jjjj_3(std::string &w)
{
    // skip it ... dont know what to do
    hash["additional information"] = w;
}

static void _6RRRt_3(std::string &w)
{
    //todo: incorrect decoding
    // same as section 1 : 6RRRt
    // do check again
    int RRR = toDigit(w[2])*100 + toDigit(w[2])*10 + toDigit(w[3]);
    std::string precipitation;

    if(RRR < 989)
        precipitation = std::to_string(RRR);
    else if(RRR == 989)
        precipitation= "\"989 or more\"";
    else if(RRR == 990)
        precipitation= "\"Trace\"";
    else
        precipitation = std::to_string((RRR%10)/10);


    int t = toDigit(w[4]);

    std::string duration;

    if(t == -1)
        duration= "24";
    else
        switch(t) {
        case 1:
            duration= "6";
            break;

        case 2:
            duration= "12";
            break;

        case 3:
            duration= "18";
            break;

        case 4:
            duration= "24";
            break;

        case 5:
            duration= "1";
            break;

        case 6:
            duration= "2";
            break;

        case 7:
            duration= "3";
            break;

        case 8:
            duration= "9";
            break;

        case 9:
            duration= "15";
            break;
        }

    hash["liquid precipitation amount Sec 2(mm)"] = std::to_string(RRR);
    hash["precipitation duration Sec 2 (hr)"] = duration;
}

static void _7RRRR_3(std::string &w)
{
    float RRRR = toDigit(w[1])*100 + toDigit(w[2])*10 + toDigit(w[3]) + toDigit(w[4])/10.0;

    if(RRRR == 999.9)
        hash["24 hr precipitation (mm)"]= "\"Trace\"";
    else
        hash["24 hr precipitation (mm)"] = std::to_string(RRRR);
}

static void _8NChh_3(std::string &w)
{
    int N = toDigit(w[1]);

    std::string genusOfCloud;
    int C = toDigit(w[2]);

    if(C == -1)
        genusOfCloud= "\"cloud not visible\"";
    else
        switch(C) {
        case 0:
            genusOfCloud= "\"cirrus(Ci)\"";
            break;

        case 1:
            genusOfCloud= "\"cirrocumulus(Cc)\"";
            break;

        case 2:
            genusOfCloud= "\"cirrostatus(Cs)\"";
            break;

        case 3:
            genusOfCloud= "\"altocumulus(Ac)\"";
            break;

        case 4:
            genusOfCloud= "\"altostatus(As)\"";
            break;

        case 5:
            genusOfCloud= "\"nimbostratus(Ns)\"";
            break;

        case 6:
            genusOfCloud= "\"stratocumulus(Sc)\"";
            break;

        case 7:
            genusOfCloud= "\"stratus(St)\"";
            break;

        case 8:
            genusOfCloud= "\"cumulus(Cu)\"";
            break;

        case 9:
            genusOfCloud= "\"cumulonimbus(Cb)\"";
            break;
        }

    int hh = toDigit(w[3])*10 + toDigit(w[4]);
    int heightOfCloudBase ;

    if(hh <= 50 && hh >= 0)
        heightOfCloudBase = hh * 30 ; // in meters(m)
    else if(hh >= 89 && hh <= 99) {
        ;// dont know what to do with this
    }

    hash["cloud coverage of layer"] = std::to_string(N);
    hash["genus of cloud"] = genusOfCloud;
    hash["height of cloud base"] = std::to_string(heightOfCloudBase);

    // hash["8NCC"] = GenusOfCloud + " middleCloudType: " + middleCloudType + " highCloudType: " + highCloudType;
}

static void _9SSss_3(std::string &w)
{
    int SS = toDigit(w[1])*1000 + toDigit(w[2])*100 + toDigit(w[3])*10 + toDigit(w[4]);

    hash["Special Phenomena Group"] = std::to_string(SS);
}

void section3(char *l)
{
    std::string line(l);
    std::vector<std::string> list = split(line, ' ');

    for (std::string w: list) {

        int msd = toDigit(w[0]);

        switch (msd) {
        //case 0: _0something_3(w) break;
        // dont know about the above one
        case 1:
            _1sTTT_3(w);
            break;

        case 2:
            _2sTTT_3(w);
            break;

        case 3:
            _3Ejjj_3(w);
            break;

        case 4:
            _4Esss_3(w);
            break;

        case 5:
            _5jjjj_3(w);
            break;

        case 6:
            _6RRRt_3(w);
            break;

        case 7:
            _7RRRR_3(w);
            break;

        case 8:
            _8NChh_3(w);
            break;

        case 9:
            _9SSss_3(w);
            break;

        default:
            break;
        }
    }
}

void section4(char *l)
{

}

void section5(char *l)
{

}

static std::string getVal(const std::unordered_map<std::string, std::string> &hash, const std::string key, const int defaultValue = -99)
{
    auto it = hash.find(key);

    if(it == hash.end())
        return std::to_string(defaultValue);

    return it->second;
}

static bool firstTime = true;
int readingNo = -1;
void decode()
{
    if(decodeError) {
        decodeError = false;
        return;
    }

    int currCode = stoi(getVal(hash, "station code"));

    for(int j = 0; j<stationList.size(); ++j)
        if(currCode == stationList[j]) { //this station's detials needs to be printed

            if(firstTime) {//print csv header
                firstTime = false;

                for(int i = 0; i<fieldList.size()-1; ++i)
                    std::cout<<"\""<<fieldList[i]<<"\",";

                std::cout<<'"'<<fieldList[fieldList.size()-1]<<"\"\n";
            }

            for(int i = 0; i<fieldList.size()-1; ++i)
                for(int j = 0; j<stationList.size(); ++j)
                    if(currCode == stationList[j])
                        std::cout<<'"'<<getVal(hash, fieldList[i])<<"\",";

            std::cout<<'"'<<getVal(hash, fieldList[fieldList.size()-1])<<"\"\n";
        } //if(currCode == stationList[j])

#ifdef SQL

    if(toDB) {
        soci::session *sql= SQLSession::getSession(&sqldbname, &sqlusername, &sqlpassword);
        pushDB(sql, hash, readingNo);
    }

#endif
    hash.clear();

}

#include "node.h"
extern FILE *sectionFile;
void printBlock(char *heading, char *sec0, SecList *list)
{


    while( ! list->q.empty()) {
        section0(sec0);

        if(sectionFile) {
            fprintf(sectionFile, "h\n%s\n", heading);
            fprintf(sectionFile, "0\n%s\n", sec0);
        }

        Sec *n = list->q.front();

        for(int i = 0; i<6; ++i) {
            if(n->s[i])
                switch(i) {
                case 1:
                    section1(n->s[i]);
                    break;

                case 2:
                    section2(n->s[i]);
                    break;

                case 3:
                    section3(n->s[i]);
                    break;

                case 4:
                    section4(n->s[i]);
                    break;

                case 5:
                    section5(n->s[i]);
                    break;
                }

            if(sectionFile)
                if(n->s[i])
                    switch(i) {
                    case 1:
                        fprintf(sectionFile, "1\n%s\n", n->s[i]);
                        break;

                    case 2:
                        fprintf(sectionFile, "2\n%s\n", n->s[i]);
                        break;

                    case 3:
                        fprintf(sectionFile, "3\n%s\n", n->s[i]);
                        break;

                    case 4:
                        fprintf(sectionFile, "4\n%s\n", n->s[i]);
                        break;

                    case 5:
                        fprintf(sectionFile, "5\n%s\n", n->s[i]);
                        break;
                    }
        }

        decode();
        list->q.pop();

        if(sectionFile) {
            fprintf(sectionFile, "\n");
        }
    }
}