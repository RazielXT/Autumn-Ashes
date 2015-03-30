#pragma once

struct GameConfig
{
    int width;
    int height;
    int shadow;
    bool ssao;
    bool fs;

    void loadCfg()
    {
        Ogre::ConfigFile cfgFile;
        cfgFile.loadDirect("config.ini");
        std::string wString = cfgFile.getSetting("width", "renderer");
        std::string hString = cfgFile.getSetting("height", "renderer");
        std::string shdString = cfgFile.getSetting("shadow", "renderer");
        std::string ssaoString = cfgFile.getSetting("ssao", "renderer");
        std::string fsString = cfgFile.getSetting("fullscreen", "renderer");
        width = Ogre::StringConverter::parseInt(wString);
        height = Ogre::StringConverter::parseInt(hString);
        shadow = Ogre::StringConverter::parseInt(shdString);
        ssao = Ogre::StringConverter::parseBool(ssaoString);
        fs = Ogre::StringConverter::parseBool(fsString);
    }

    void saveCfg()
    {
        std::ofstream cfgFile("config.ini");
        cfgFile << "[renderer]\n";
        cfgFile << "width = " << std::string(Ogre::StringConverter::toString(width) + "\n");
        cfgFile << "height = " << std::string(Ogre::StringConverter::toString(height) + "\n");
        cfgFile << "shadow = " << std::string(Ogre::StringConverter::toString(shadow) + "\n");
        cfgFile << "ssao = " << std::string(Ogre::StringConverter::toString(ssao) + "\n");
        cfgFile << "fullscreen = " << std::string(Ogre::StringConverter::toString(fs) + "\n");
    }
};