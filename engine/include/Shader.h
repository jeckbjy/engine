#pragma once
#include "Graphics.h"
#include "Asset.h"

CU_NS_BEGIN

enum Scheme
{
	SCHEME_AUTO,		// 自动查找可支持的
	SCHEME_HIGH,
	SCHEME_MIDDLE,
	SCHEME_LOW,
};

typedef Pipeline Pass;

class CU_API Technique
{
public:
	Technique(const String& name, Scheme scheme = SCHEME_AUTO) 
		: m_name(name), m_scheme(scheme){}
	~Technique(){}

	const String& getName() const { return m_name; }
	Scheme getScheme() const { return m_scheme; }
	Pass*  getPass(size_t index) const { return m_passes[index]; }
	void   addPass(Pass* pass) { m_passes.push_back(pass); }
	size_t getPassCount() const { return m_passes.size(); }

	// todo:impl
	bool isSupported() const { return true; }

private:
	typedef Vector<Pass*> PassVec;
	String	m_name;
	Scheme	m_scheme;
	PassVec	m_passes;
};

class CU_API Shader : public Asset
{
public:
	typedef Vector<Technique*>		 TechVec;
	typedef std::map<String, String> PropMap;
	typedef PropMap::const_iterator	 prop_iterator;

	Shader();
	~Shader();

	bool load(Stream* stream);
	void save(Stream* stream);

	Technique* find(const String& name) const;
	Technique* findBest(Scheme scheme = SCHEME_AUTO) const;
	void addTechnique(Technique* tech);

	bool hasProperty(const String& key) const;
	String getProperty(const String& key) const;
	void setProperty(const String& key, String& val);

	// 强制类型转换
	template<typename T>
	T getProperty(const String& name) const
	{
		const String& val = getProperty(name);
		return *(T*)val.c_str();
	}
	// 强制转换
	template<typename T>
	void setProperty(const String& key, T& val)
	{
		String tmp((char*)&val, sizeof(T));
		setProperty(key, tmp);
	}

	prop_iterator begin() const { return m_properties.cbegin(); }
	prop_iterator end() const { return m_properties.cend(); }

protected:
	TechVec m_techniques;
	PropMap m_properties;
};

CU_NS_END
