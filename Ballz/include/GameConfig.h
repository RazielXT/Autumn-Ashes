#pragma once

struct GameConfig
{
    int width;
    int height;
    int shadow;
    bool ssao;
    bool fs;
    /*
    struct LevelInfo
    {
        std::string path;
        int id;
        std::string img;
    };

    std::vector<LevelInfo> levels;

    std::string strtok_str(std::string& txt, char delim)
    {
        auto dPos = txt.find_first_of(delim);
        std::string ret = txt;

        if (dPos != std::string::npos)
        {
            ret.erase(dPos, std::string::npos);
            txt.erase(0, dPos + 1);
        }
        else
            txt.clear();

        return ret;
    }
    */
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
        /*
        auto it = cfgFile.getSettingsIterator("levels");

        for (auto l = it.begin(); l != it.end(); l++)
        {
            auto str = l->second;
            LevelInfo info;

            info.id = Ogre::StringConverter::parseInt(l->first);
            info.path = strtok_str(str, '\t');
            info.img = strtok_str(str, '\n');

            levels.push_back(info);
        }*/
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
        /*
        cfgFile << "\n[levels]\n";

        for (auto l : levels)
        {
            cfgFile << std::string(Ogre::StringConverter::toString(l.id)) << "\t" << l.path << "\t" << l.img << "\n";
        }*/
    }
};