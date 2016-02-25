#include "stdafx.h"
#include "Edit.h"

int Edit::idCounter = 5000;

EditVariable::EditVariable(std::string name_, Ogre::ColourValue color, float step_)
{
	name = name_;
	size = 3;
	buffer[0] = color.r;
	buffer[1] = color.g;
	buffer[2] = color.b;

	step = step_;
}

EditVariable::EditVariable(std::string name_, Ogre::Vector3 vec, float step_)
{
	name = name_;
	size = 3;
	buffer[0] = vec.x;
	buffer[1] = vec.y;
	buffer[2] = vec.z;

	step = step_;
}

EditVariable::EditVariable(std::string name_, Ogre::Vector4 vec, float step_)
{
	name = name_;
	size = 4;
	buffer[0] = vec.x;
	buffer[1] = vec.y;
	buffer[2] = vec.z;
	buffer[3] = vec.w;

	step = step_;
}

EditVariable::EditVariable(std::string name_, float x, float y, float z)
{
	name = name_;
	size = 3;
	buffer[0] = x;
	buffer[1] = y;
	buffer[2] = z;
}

EditVariable::EditVariable(std::string name_, float x, float y)
{
	name = name_;
	size = 2;
	buffer[0] = x;
	buffer[1] = y;
}

EditVariable::EditVariable(std::string name_, float x)
{
	name = name_;
	size = 1;
	buffer[0] = x;
}

void Edit::mergeParams(const std::vector<EditVariable>& from, std::vector<EditVariable>& target, bool addNotExisting)
{
	for (auto& var : from)
	{
		if (var.edited)
		{
			bool found = false;

			for (auto& mvar : target)
			{
				if (mvar.name == var.name && mvar.size == var.size)
				{
					memcpy(mvar.buffer, var.buffer, sizeof(float) * var.size);
					found = true;
				}
			}

			if (!found && addNotExisting)
			{
				EditVariable mvar;
				mvar.edited = true;
				mvar.name = var.name;
				mvar.size = var.size;
				memcpy(mvar.buffer, var.buffer, sizeof(float) * var.size);

				target.push_back(mvar);
			}
		}
	}
}