#pragma once

#include "MaterialEdit.h"

struct OptimizedGroup;

struct OptimizedGroupEdit : public MaterialEdit
{
public:

    OptimizedGroupEdit() {}
    OptimizedGroupEdit(OptimizedGroup* group);
    virtual ~OptimizedGroupEdit() {}

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & originName;
        ar & psVariables;
        ar & vsVariables;
    }

    virtual void customAction(std::string name) override;

    static OptimizedGroupEdit* query();
    static void applyChanges(const std::map < std::string, OptimizedGroupEdit >& changes);

protected:

    bool varsChanged = false;
	std::string groupId;
};