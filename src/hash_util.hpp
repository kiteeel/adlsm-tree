#ifndef ADL_LSM_TREE_HASH_UTIL_H__
#define ADL_LSM_TREE_HASH_UTIL_H__
#include <openssl/sha.h>
#include <string>

namespace adl {
using namespace std;

string sha256_digit_to_hex(const unsigned char hash[SHA256_DIGEST_LENGTH]);
void hex_to_sha256_digit(string_view hex, unsigned char *hash);
}  // namespace adl
#endif  // ADL_LSM_TREE_HASH_UTIL_H__