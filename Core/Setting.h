#ifndef CORE_SETTING_H
#define CORE_SETTING_H

#include <Core/pch.h>

class Setting
{
public:
	Setting() = default;
	~Setting() = default;

public:
	friend class Dialog;

	int32_t GetData(const std::string& token) const;
	void SetData(const std::string& token, const std::string& data);

private:
	void Load(const std::string& filepath);
	void Save(const std::string& filepath);

private:
	std::string m_Filepath;
	std::unordered_map<std::string, std::string> m_Data;
};

#endif