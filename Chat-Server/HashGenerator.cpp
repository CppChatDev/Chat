#include <stdexcept>

#include "HashGenerator.h"
#include "bcrypt.h"

std::string HashGenerator::generate_hash(std::string& input, int work_factor)
{
	char salt[BCRYPT_HASHSIZE];
	char hash[BCRYPT_HASHSIZE];

	int ret = bcrypt_gensalt(work_factor, salt);
	if (ret != 0)
		throw std::runtime_error("generating salt error");
	ret = bcrypt_hashpw(input.c_str(), salt, hash);
	if (ret != 0)
		throw std::runtime_error("hashing error");

	return std::string(hash);
}

bool HashGenerator::check_hash(std::string& input, std::string& hash)
{
	int ret = bcrypt_checkpw(input.c_str(), hash.c_str());
	if (ret == -1)
		throw std::runtime_error("hash checking error");

	return ret == 0;
}
