#include <Core/Setting.h>

int32_t Setting::GetData(const std::string& token) const
{
	if (!m_Data.contains(token))
	{
		return 0;
	}
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
	std::ifstream ifs(filepath, std::ios::in);
	if (!ifs.is_open())
	{
		std::cerr << "无法加载配置文件：" << filepath << std::endl;
		return;
	}
	m_Data.clear();
	std::string content;
	while (std::getline(ifs, content))
	{
		if (content.empty()) continue;

		// 查找分隔符
		auto pos = content.find(':');
		if (pos == std::string::npos)
		{
			std::cerr << "配置文件格式错误：" << content << std::endl;
			continue;
		}

		// 提取 key 和 value
		std::string token = content.substr(0, pos);
		std::string data = content.substr(pos + 1);

#ifdef COMSION_DEBUG
		std::cout << '[' << __FUNCTION__ << ']' << token << ':' << data << std::endl;
#endif
		m_Data[token] = data;
	}
	ifs.close();
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