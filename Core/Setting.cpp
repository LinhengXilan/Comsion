#include <Core/Setting.h>

void Setting::Load(const std::string& filepath)
{
	m_Filepath = filepath;
	std::ifstream ifs(filepath, std::ios::in | std::ios::binary);
	if (!ifs.is_open()) return;
#ifdef COMSION_DEBUG
	std::cout << __FUNCTION__ << std::endl;
#endif
	std::string content;
	ifs.seekg(0, std::ios::end);
	size_t size = ifs.tellg();
	if (size != -1)
	{
		content.resize(size);
		ifs.seekg(0, std::ios::beg);
		ifs.read(&content[0], content.size());
		ifs.close();
	}
	size_t pos{0};
	while (pos + 1 != std::string::npos)
	{
		std::string token;
		std::string data;
		size_t splitPos{0};
		splitPos = content.find_first_of(':', splitPos);
		token = content.substr(pos, splitPos - pos - 1);
		pos = splitPos;
		splitPos = content.find_first_of('\n', splitPos);
		data = content.substr(pos + 1, splitPos - pos - 2);
		pos = splitPos;
#ifdef COMSION_DEBUG
		std::cout << token << ':' << data << std::endl;
#endif
		m_Data.emplace(token, data);
	}
}

std::string Setting::GetData(const std::string& token)
{
	if (!m_Data.contains(token)) return "";
	return m_Data.at(token);
}