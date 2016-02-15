#pragma once

#include "Gorilla.h"

struct EditVariable
{
    std::string name;
    int size = 0;
    float buffer[4];

    bool edited = false;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & size;
        ar & boost::serialization::make_array(buffer, 4);
    }

    EditVariable() {}
    EditVariable(std::string name, Ogre::Vector3 vec);
    EditVariable(std::string name, Ogre::Vector4 vec);
    EditVariable(std::string name, Ogre::ColourValue);
    EditVariable(std::string name, float x, float y, float z);
    EditVariable(std::string name, float x, float y);
    EditVariable(std::string name, float x);
};

using EditVariables = std::vector<EditVariable>;

struct EditRow
{
    std::string name;
    enum {Caption, Params, Static, Action} type;
};
using EditBaseRows = std::vector<EditRow>;

struct Edit
{
    virtual ~Edit() {}

    static int idCounter;

    bool persistent = false;
    std::string originName;

    EditBaseRows getBaseRows()
    {
        return rows;
    };
    virtual EditVariables* getParams(const std::string& row) = 0;
    virtual void editChanged(EditVariable& var, const std::string& row) = 0;
    virtual void customAction(std::string name) = 0;

protected:

    EditBaseRows rows;
    void mergeParams(const std::vector<EditVariable>& from, std::vector<EditVariable>& target, bool addNotExisting);
};