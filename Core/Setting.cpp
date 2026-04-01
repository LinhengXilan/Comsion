#include <Core/Setting.h>

int32_t Setting::GetData(const std::string& token) const
{
	return std::stoi(m_Data.at(token));
}

void Setting::SetData(const std::string& token, const std::string& data)
{
	if (data.empty()) return;
	m_Data[token] = data;
}

void Setting::Load(const std::string& filepath)
{
	m_Filepath = filepath;
	std::ifstream ifs(filepath, std::ios::in | std::ios::binary);
	if (!ifs.is_open()) return;
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
	size_t splitPos{0};
	while (splitPos != std::string::npos)
	{
		std::string token;
		std::string data;

		splitPos = content.find_first_of(':', pos);
		token = content.substr(pos, splitPos - pos);
		pos = splitPos + 2;

		splitPos = content.find_first_of('\n', pos);
		data = content.substr(pos, splitPos - pos);
		pos = splitPos + 1;
		m_Data.emplace(token, data);
	}
}

void Setting::Save(const std::string& filepath)
{
	std::ofstream ofs(filepath, std::ios::out | std::ios::binary);
	if (!ofs.is_open()) return;
	for (auto& it : m_Data)
	{
		if (it.second == "")
		{
			continue;
		}
		ofs << it.first << ": " << it.second << '\n';
	}
	ofs.close();
}