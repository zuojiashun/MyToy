#ifndef _CONFIG_H_
#define _CONFIG_H_

#define _Config CConfig::GetInstance()

#include<map>
#include<sstream>
#include<fstream>
#include"ExternInclude\rapidxml\rapidxml.hpp"
#include"SpCommon\Singleton.h"
#include"SpCommon\Converter.h"
#include"SpCommon\ApiDefine.h"

using namespace std;
using namespace rapidxml;
using namespace spgameengine;

class ENGINE_API CConfig : public Singleton<CConfig>
{
private:
	friend class Singleton<CConfig>;
	map<string,map<string, string>> m_configs;

	template<typename TKey, typename TValue>
	void SetPair(map<TKey, TValue>& mp, TKey& key, TValue& value)
	{
		auto it = mp.begin();
		if (it == mp.end())
			mp.insert(make_pair(key, value));
		else
			mp[key] = value;
	}

	virtual void OnInitialize() override;
public:

	template<typename T>
	T GetValue(const string group,const string key)
	{
		auto it_configs = m_configs.find(group);
		if (it_configs == m_configs.end())
			return T(0);
		auto it = it_configs->second.find(key);
		if (it == it_configs->second.end())
			return T(0);
		return CConverter::ToValue<T>(it->second);
	}

	string GetString(const string group, const string key);
};

#endif