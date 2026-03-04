#include "accessibility.h"

#include <algorithm>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

bool ResourceTest::check(const std::string& path, std::vector<std::string>&  filenames) 
{
	std::unordered_map<std::string, size_t> appearances; 

	for (const auto & entry : std::filesystem::directory_iterator(path))
	{
        std::string full_path = entry.path();
		std::istringstream token_stream(full_path); 
		
		std::string token;
		std::vector<std::string> tokens;
		
		while(std::getline(token_stream, token, '/'))
		{
			tokens.push_back(token);
		}

		appearances[tokens[tokens.size()-1]]++;
	}
	
	auto has_all_files = [appearances](const std::string& f){
		return appearances.contains(f);
	};

	return std::ranges::all_of(
		filenames.cbegin(),
		filenames.cend(),
		has_all_files
	);
}