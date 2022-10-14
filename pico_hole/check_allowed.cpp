#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

// Nice lists: https://raw.githubusercontent.com/JO2EY/Rules/master/Surge/RULE-SET/Streaming.list

const char *allowed_patterns[] = {
    "netflix.com.", // Netflix domains below
    "netflix.net.",
    "nflxext.com.",
    "nflximg.com.",
    "nflximg.net.",
    "nflxso.net.",
    "nflxvideo.net.",
    "disneyplus.com.", // Disney+ domains below
    "disney-plus.net.",
    "disneystreaming.com.",
    "dssott.com.",
    "bamgrid.com.",
    "cdn.registerdisney.go.com.",
    "bam.nr-data.net.",
    "disney-portal.my.onetrust.com.",
    "disneyplus.bn5x.net.",
    "starott.com.",
    "adobedtm.com.",
    "conviva.com.",
    "x.ss2.us.",
    "js-agent.newrelic.com.",
    "d9.flashtalking.com.",
    "disney.co.jp.",
    "disney.my.sentry.io.",
    "ntp.org.",
    "worldtimeserver.com.",
    "amazontrust.com.",
    "comodoca.com.",
    "usertrust.com.",
    "mediaservices.cdn-apple.com.",
    "digicert.com.",
    "mzstatic.com.",
    "googleusercontent.com.",
    "pki.goog.",
    "microsoft.com.",
    "images.ctfassets.net.",
    "time.samsungcloudsolution.com.",
    "amazon.com.", //  Amazon prime domains below
    "amazonvideo.com.",
    "amazon.co.uk.",
    "d184dfn36gombl.cloudfront.net.",
    "media-amazon.com.",
    "ssl-images-amazon.com.",
    "aiv-cdn.net.",
    "aiv-delivery.net.",
    "images-amazon.com.",
    "pv-cdn.net.",
};

// wildcard to match blacklist specific to samsung, don't even print reject. (can switch later to a blacklist style if list below covers everything, to gather logs first)
const char *ignore_print[] = {
    "samsung",
    "d7luqmstmj331.cloudfront.net.",
    ".adgrx.com.",
    ".sectigo.com.",
    ".tivuon.tv.",
    ".demdex.net.",
};

/*
For more domains on samsung, so far lots of subdomains for:
.samsungads.com.
.samsungcloudsolution.com.
.samsungcloudsolution.net.
.samsungelectronics.com.
.samsungotn.net.
.samsungotn.net.
.samsungqbe.com.
*/

const int allowed_patterns_size = sizeof(allowed_patterns)/sizeof(const char*);
const int ignore_print_size = sizeof(ignore_print)/sizeof(const char*);

bool ends_with(const char *str, const char *suffix)
{
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    
    if (lensuffix > lenstr)
    {
        return false;
    }
    
    if (strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0)
    {
        // don't want someone registering anetflix.com and having it allowed
        return ((lensuffix == lenstr) || (str[lenstr - lensuffix - 1] == '.'));
    }

    return false;
}

extern "C" bool check_dns_name(int id, const char *name)
{
    for (int i=0;i<allowed_patterns_size;++i)
    {
        if (ends_with(name, allowed_patterns[i]))
        {
            printf("[%d] Allowing: %s\r\n", id, name);
            return true;
        }
    }

    for (int i=0;i<ignore_print_size;++i)
    {
        if (strstr(name, ignore_print[i])!=NULL)
        {
            return false;
        }
    }

    printf("[%d] Rejecting: %s\r\n", id, name);
    return false;
}
