#include "MaterialSwitcher.h"

#include <Ogre.h>

MaterialSwitcher::MaterialSwitcher(void)
{
	Ogre::Material *depthMat = (Ogre::Material *) Ogre::MaterialManager::getSingleton().getByName("BasicDepthWrite").getPointer();
	depthMat->load();
	m_depthTechnique = depthMat->getBestTechnique();

	Ogre::Material *normMat = (Ogre::Material *) Ogre::MaterialManager::getSingleton().getByName("BasicWorldNormalWrite").getPointer();
	normMat->load();
	m_normalTechnique = normMat->getBestTechnique();

	Ogre::Material *combinedMat = (Ogre::Material *) Ogre::MaterialManager::getSingleton().getByName("NormalDepthCombinedWrite").getPointer();
	combinedMat->load();
	m_combinedTechnique = combinedMat->getBestTechnique();
}

MaterialSwitcher::~MaterialSwitcher(void)
{
}

Ogre::Technique* MaterialSwitcher::handleSchemeNotFound(unsigned short schemeIndex, const Ogre::String &schemeName,
									   Ogre::Material *originalMaterial, unsigned short lodIndex,
									   const Ogre::Renderable *rend)
{
	if(schemeName == "DepthPass")
		return m_depthTechnique;
	else if(schemeName == "NormalsPass")
		return m_normalTechnique;
	else if(schemeName == "CombinedPass")
		return m_combinedTechnique;
	return NULL;
}