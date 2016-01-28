#include "Shader.h"

CU_NS_BEGIN

Shader::Shader()
{

}

Shader::~Shader()
{

}

Technique* Shader::find(const String& name) const
{
	for (TechVec::const_iterator itor = m_techniques.begin(); itor != m_techniques.end(); ++itor)
	{
		Technique* tech = *itor;
		if (tech->getName() == name)
		{
			return tech->isSupported() ? tech : NULL;
		}
	}
	return NULL;
}

Technique* Shader::findBest(Scheme scheme /* = SCHEME_AUTO */) const
{
	Technique* supported = NULL;
	for (TechVec::const_iterator itor = m_techniques.begin(); itor != m_techniques.end(); ++itor)
	{
		Technique* tech = *itor;
		if (!tech->isSupported())
			continue;
		// 直接返回第一个,否则找到相匹配的第一个
		if (scheme == SCHEME_AUTO)
			return tech;
		if (!supported)
			supported = tech;
		if (tech->getScheme() == scheme)
			return tech;
	}

	return NULL;
}

void Shader::addTechnique(Technique* tech)
{
	m_techniques.push_back(tech);
}

bool Shader::hasProperty(const String& key) const
{
	return m_properties.find(key) != m_properties.end();
}

String Shader::getProperty(const String& key) const
{
	PropMap::const_iterator itor = m_properties.find(key);
	if (itor != m_properties.cend())
		return itor->second;
	return String();
}

void Shader::setProperty(const String& key, String& val)
{
	m_properties[key] = val;
}

CU_NS_END
