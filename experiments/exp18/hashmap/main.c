#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#define MAC_ADDR_SIZE 6
#define SSID_SIZE (32+1)
#define SCAN_RESULT_LIST_SIZE 32

typedef uint8_t u8;
typedef int32_t i32;
typedef uint32_t u32;
typedef uint64_t u64;
typedef struct { u8 addr[MAC_ADDR_SIZE]; } mac_t;
typedef i32 dbm_t;
typedef struct {
    u8 ssid_len;
    u8 ssid[SSID_SIZE];
} ssid_t;
typedef struct {
    mac_t mac;
    dbm_t rssi;
    ssid_t ssid;
} scan_result_t;

static u32 PRNG_SEED = 8;

void set_random_seed(u32 seed) {
    assert(seed != 0);
    PRNG_SEED = seed;
}

// PRNG using 32-bit xorshift from Marsaglia 2003.
// Do not use seed=0 because it yields output stream of all zeros.
u32 random() {
    assert(PRNG_SEED != 0);
    u32 y = PRNG_SEED;
    y ^= y<<13;
    y ^= y>>17;
    y ^= y<<5;
    PRNG_SEED = y;
    return y;
}

// Fill a mac address struct with random bytes
void random_mac(mac_t *mac) {
    assert(mac != NULL);
    for(u32 i=0; i<MAC_ADDR_SIZE; ++i) {
        mac->addr[i] = random() >> 24;
    }
}

void print_mac(mac_t *mac) {
    assert(mac != NULL);
    for(u32 i=0; i<MAC_ADDR_SIZE; ++i) {
        printf("%02X", mac->addr[i]);
        if(i+1 < MAC_ADDR_SIZE) {
            printf(":");
        }
    }
}

// Fill ssid struct with length and characters for a random ssid name
// P(hidden ssid with len==0): 1/8
// P(hidden ssid with len>0 && ssid[0]=0): 1/8
// P(normal long ssid): 2/8
// P(normal short ssid): 4/8
void random_ssid(ssid_t *ssid) {
    assert(ssid != NULL);
    // Clear the string field
    for(u32 i=0; i<SSID_SIZE; ++i) {
        ssid->ssid[i] = 0;
    }
    // Probability distribution over flavors of hidden and normal SSIDs
    switch(random() % 8) {
    case 0:
        ssid->ssid_len = 0;                         // Hidden SSID (len=0)
        return;
    case 1:
        ssid->ssid_len = random() % (SSID_SIZE-1);  // Hidden SSID (null str)
        return;
    case 2:
    case 3:
        ssid->ssid_len = random() % (SSID_SIZE-1);  // Normal SSID (long)
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    default:
        ssid->ssid_len = random() % (SSID_SIZE/3);  // Normal SSID (short)
        break;
    }
    // Generate a normal SSID string
    for(u32 i=0; i<ssid->ssid_len; ++i) {
        u32 n = random() % (2*26 + 10 + 3);
        if     (n<26)        { ssid->ssid[i] = (u8)('a')+n; }
        else if(n<(2*26))    { ssid->ssid[i] = (u8)('A')+(n-26); }
        else if(n<(2*26+10)) { ssid->ssid[i] = (u8)('0')+(n-2*26); }
        else if(n<(2*26+11)) { ssid->ssid[i] = '_'; }
        else                 { ssid->ssid[i] = ' '; }
    }
}

// Print SSID, allowing for two possible types of hidden SSIDs
void print_ssid(ssid_t *ssid) {
    assert(ssid != NULL);
    assert(ssid->ssid != NULL);
    const u8 len = ssid->ssid_len;
    const bool hidden_len_0 = len == 0;
    const bool hidden_null = ssid->ssid[0] == 0;
    if(hidden_len_0)     { printf("'' 0");  }                     // Hidden
    else if(hidden_null) { printf("-- %u", len); }                // Hidden
    else                 { printf("'%s' %u", ssid->ssid, len); }  // Normal
}

void random_scan_result(scan_result_t *sr) {
    assert(sr != NULL);
    random_mac(&(sr->mac));
    random_ssid(&(sr->ssid));
    if(sr->ssid.ssid_len==0 || sr->ssid.ssid[0]==0) {
        // Bias hidden SSIDs to have weaker dBm
        sr->rssi = -40 - (i32)(random() % 50);
    } else {
        sr->rssi = -33 - (i32)(random() % 60);
    }
}

int main() {
    scan_result_t scan_results[SCAN_RESULT_LIST_SIZE];
    set_random_seed(2);
    u32 partition = SCAN_RESULT_LIST_SIZE / 3 * 2;
    u32 limit = SCAN_RESULT_LIST_SIZE;
    // Generate random scan results (high probability they are unique)
    for(u32 i=0; i<partition; ++i) {
        random_scan_result(&(scan_results[i]));
    }
    // Add additional beacons for some of the previous results
    for(u32 i=partition; i<limit; ++i) {
        // Copy a random result from before the partition
        scan_results[i] = scan_results[random() % partition];
        // Maybe modify the MAC address (different BSSID, same SSID)
        if(random() % 10 < 2) {
            random_mac(&(scan_results[i].mac));
        }
        // Perturb the RSSI within +/- 3dBm of previous value
        scan_results[i].rssi += (random() % 7) - 3;
    }
    // Print the whole scan result list
    for(u32 i=0; i<limit; ++i) {
        scan_result_t sr = scan_results[i];
        print_mac(&(sr.mac));
        printf(" %d ", sr.rssi);
        print_ssid(&(sr.ssid));
        printf("\n");
    }
    return 0;
}
