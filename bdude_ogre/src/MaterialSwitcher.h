#pragma once

#include <Ogre.h>

class MaterialSwitcher : public Ogre::MaterialManager::Listener
{
public:
	MaterialSwitcher(void);
	virtual ~MaterialSwitcher(void);

	virtual Ogre::Technique* handleSchemeNotFound(unsigned short schemeIndex, const Ogre::String &schemeName,
		Ogre::Material *originalMaterial, unsigned short lodIndex, const Ogre::Renderable *rend);

private:
	Ogre::Technique *m_depthTechnique, *m_normalTechnique, *m_combinedTechnique;
};
