#include "http_request.h"
#include "encrypt.h"

int main()
{
    const char* key = "test123";
    const char* key2 = "CF81C4C0EFED4C6C39E300BC33A4EE735AA9BF85547CBD5D8B2A05E53480B83A78323DE90F09E07D75A3147B67287715690CAF9250E73F6CA046A2A0D04D72545F531E150195B509AC23DC86EC7AE88C60E0294044AF60340B66570D03ED7A5C354C684F0F0CA47C6BA054C98CCC6EA9794E96B41F2E2F5797C2606AA336D9086031886065F773FFFD21A9746EB7F2933F1722E8EAEB687E6F67F0D88D70A7894B1FFB41B5AFB71CD6927269FE41796EC7B1B93381C9B36BC1319D94837259548D7A969756BC473E7984A7EEB601EF4CAC904721D8D5D7BFBFAFC0C222279F5DE78322DA34A5D919ABD5B15A298E7DE75A38CC7312148755A3A204646D654D82AA83D7C9576C201E6E9D213EC3EB300C7607F97470B46D8AE8B39D41EEDB5ADA558CED3ACE7D0A697865213A5DF969775539D087CDDFBF1420F4DF8FD496A72BE7D6D30A7E5E57117B0C9E9E47D8E9B4870B74D8C650D8B4A44F09E0EE5061ABE2B75F9AF283F8AB1406500B7633351196BD53FCD801F069A02F6BDBEF5FE029";
    // printf("\n%d\n\n", request_public_key_check(&key));
    // printf("\n%d\n\n", request_public_key_check(&key2));
    // printf("\n%d\n\n", request_public_key_check(&key2));
    // printf("\n%d\n\n", request_public_key_check(&key2));
    // printf("\n%d\n\n", request_public_key_check(&key2));
    // printf("\n%d\n\n", request_public_key_check(&key2));
    // printf("\n%d\n\n", request_public_key_check(&key2));
    // printf("\n%d\n\n", request_public_key_check(&key2));
    // printf("\n%d\n\n", request_public_key_check(&key2));
    // printf("\n%d\n\n", request_public_key_check(&key2));
    // printf("\n%d\n\n", request_public_key_check(&key2));

    send_updated_key(&key, &key2, &key2);
}
