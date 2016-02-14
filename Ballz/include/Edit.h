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
};

using EditVariables = std::vector<EditVariable>;

struct EditRow
{
    std::string name;
    enum {Caption, Params, Static, Custom} type;
};
using EditBaseRows = std::vector<EditRow>;

struct Edit
{
    virtual ~Edit() {}

    static int idCounter;

    std::string originName;

    EditBaseRows getBaseRows()
    {
        return rows;
    };
    virtual EditVariables* getParams(int row) = 0;
    virtual void editChanged(EditVariable& var, int row) = 0;
    virtual void customAction(std::string name) = 0;

protected:

    EditBaseRows rows;
    void mergeParams(std::vector<EditVariable>& from, std::vector<EditVariable>& target, bool addNotExisting);
};