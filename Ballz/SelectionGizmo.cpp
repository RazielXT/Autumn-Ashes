#include "stdafx.h"
#include "SelectionGizmo.h"
#include "PlayerCamera.h"
#include "GUtils.h"
#include "SceneInteraction.h"
#include "ObjectSelection.h"

void SelectionGizmo::init(ObjectSelection* p)
{
	parent = p;
	axesPlanes[0] = GUtils::MakeEntity("lPlane.mesh");
	axesPlanes[1] = GUtils::MakeEntity("rPlane.mesh");
	axesPlanes[2] = GUtils::MakeEntity("bPlane.mesh");

	axesPlanes[0]->setVisible(false);
	axesPlanes[1]->setVisible(false);
	axesPlanes[2]->setVisible(false);

	currentMode = SelectionMode::Select;
	item = nullptr;
	activeWidget = nullptr;
	copyMode = false;
}

void SelectionGizmo::setRoot(EditorItem* node)
{
	if(activeWidget && !node)
		deactivate();

	item = node;
}

void SelectionGizmo::setMode(SelectionMode mode)
{
	if (mode == SelectionMode::Select)
	{
		deactivate();
	}
	else
	{
		if (currentMode == SelectionMode::Select)
		{
			copyMode = false;
			registerInputListening();
		}

		if (mode == SelectionMode::Rotate)
			activeWidget = &rotateWidget;
		else if (mode == SelectionMode::Scale)
			activeWidget = &scaleWidget;
		else
			activeWidget = &moveWidget;

		activeWidget->init(this);
	}

	currentMode = mode;
}

void SelectionGizmo::deactivate()
{
	if (activeWidget)
		activeWidget->deinit();
	activeWidget = nullptr;
	unregisterInputListening();
}

void SelectionGizmo::pressedKey(const OIS::KeyEvent &arg)
{
	if (arg.key == OIS::KC_LSHIFT)
		copyMode = true;
}

void SelectionGizmo::releasedKey(const OIS::KeyEvent &arg)
{
	if (arg.key == OIS::KC_LSHIFT)
		copyMode = false;
}

void SelectionGizmo::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (activeWidget && id == OIS::MB_Left)
	{
		activeWidget->mousePressed(id, item);

		if (activeWidget->aboveAxis == -1 && activeWidget->activeAxis == -1)
			parent->pickMouseRayItem();
	}
}

void SelectionGizmo::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (activeWidget)
		activeWidget->mouseReleased(id, item);
}

void SelectionGizmo::movedMouse(const OIS::MouseEvent &e)
{
	if (activeWidget)
	{
		activeWidget->movedMouse(e.state.X.rel, e.state.Y.rel, item, this);
		parent->updateUiSelectedInfo();
	}
}

bool SelectionGizmo::update()
{
	if (currentMode != SelectionMode::Select && item)
	{
		auto itempos = item->getPosition();
		auto cam = Global::camera->camera;
		auto dist = cam->getDerivedPosition().distance(itempos);

		Ogre::Vector3 scale(dist / 5.0f, dist / 5.0f, dist / 5.0f);

		Ogre::Vector3 rotScale[3];
		auto dir = (itempos - cam->getDerivedPosition());
		dir.normalise();
		auto z = (dir.dotProduct(Ogre::Vector3::UNIT_Z) < 0);
		auto x = (dir.dotProduct(Ogre::Vector3::UNIT_X) < 0);
		auto y = (dir.dotProduct(Ogre::Vector3::UNIT_Y) > 0);

		rotScale[0] = Ogre::Vector3(1, y ? -1 : 1, z ? -1 : 1);
		rotScale[1] = Ogre::Vector3(x ? -1 : 1, y ? -1 : 1, 1);
		rotScale[2] = Ogre::Vector3(x ? -1 : 1, 1, z ? -1 : 1);

		//GUtils::DebugPrint(std::to_string(dir.dotProduct(Ogre::Vector3::UNIT_Z)) + " " + std::to_string(dir.dotProduct(Ogre::Vector3::UNIT_X)) + " " + std::to_string(dir.dotProduct(Ogre::Vector3::UNIT_Y)));
		axesPlanes[0]->getParentSceneNode()->setScale(scale*rotScale[0]);
		axesPlanes[1]->getParentSceneNode()->setScale(scale*rotScale[1]);
		axesPlanes[2]->getParentSceneNode()->setScale(scale*rotScale[2]);

		axesPlanes[0]->getParentSceneNode()->setPosition(itempos);
		axesPlanes[1]->getParentSceneNode()->setPosition(itempos);
		axesPlanes[2]->getParentSceneNode()->setPosition(itempos);
	}

	return currentMode != SelectionMode::Select;
}

void SelectionGizmo::GizmoWidget::deinit()
{
	for (int i = 0; i < MAX_AXES && axes[i]; i++)
		axes[i]->setVisible(false);
}

void SelectionGizmo::GizmoWidget::mousePressed(OIS::MouseButtonID id, EditorItem* root)
{
	if (id != OIS::MB_Left)
		return;

	activeAxis = aboveAxis;
}

void SelectionGizmo::GizmoWidget::mouseReleased(OIS::MouseButtonID id, EditorItem* root)
{
	if (id != OIS::MB_Left)
		return;

	activeAxis = -1;
	setDefaultMats();
}

void SelectionGizmo::GizmoWidget::movedMouse(int x, int y, EditorItem* root, SelectionGizmo* parent)
{
	if (activeAxis < 0)
	{
		int currentAbove = -1;
		float closest = 100000;
		auto mouseRay = SceneInteraction::getMouseRay();
		Ogre::Vector3 hit[MAX_AXES];

		for (int i = 0; i < MAX_AXES && axes[i]; i++)
		{
			if (SceneInteraction::TestEntityRay(mouseRay, axes[i], hit[i], &closest, true))
				currentAbove = i;
		}

		if (currentAbove != aboveAxis)
		{
			setDefaultMats();

			if(currentAbove > -1)
				axes[currentAbove]->setMaterialName(selectedMaterial);

			aboveAxis = currentAbove;
		}
	}
	else
	{
		if (parent->copyMode)
		{
			root->duplicate();
			parent->copyMode = false;
		}

		movedActiveMouse(x, y, root);
	}
}

void SelectionGizmo::RotateGizmoWidget::init(SelectionGizmo* parent)
{
	axes[0] = parent->axesPlanes[0];
	axes[1] = parent->axesPlanes[1];
	axes[2] = parent->axesPlanes[2];
	axes[3] = nullptr;

	axes[0]->setVisible(true);
	axes[1]->setVisible(true);
	axes[2]->setVisible(true);

	selectedMaterial = "scaleYellow";
	setDefaultMats();
}

void SelectionGizmo::RotateGizmoWidget::movedActiveMouse(int x, int y, EditorItem* root)
{
	if (activeAxis == 0)
	{
		root->rotate(Ogre::Vector3(1, 0, 0), Ogre::Radian(y*0.02f));
	}
	if (activeAxis == 1)
	{
		root->rotate(Ogre::Vector3(0, 0, 1), Ogre::Radian(y*0.02f));
	}
	if (activeAxis == 2)
	{
		root->rotate(Ogre::Vector3(0, 1, 0), Ogre::Radian(x*0.02f));
	}
}

void SelectionGizmo::RotateGizmoWidget::setDefaultMats()
{
	axes[0]->setMaterialName("scaleRed");
	axes[1]->setMaterialName("scaleBlue");
	axes[2]->setMaterialName("scaleGreen");
}

void SelectionGizmo::MoveGizmoWidget::init(SelectionGizmo* parent)
{
	axes[0] = parent->axesPlanes[0];
	axes[1] = parent->axesPlanes[1];
	axes[2] = parent->axesPlanes[2];
	axes[3] = nullptr;

	axes[0]->setVisible(true);
	axes[1]->setVisible(true);
	axes[2]->setVisible(true);

	selectedMaterial = "scaleYellow";
	setDefaultMats();
}

void SelectionGizmo::MoveGizmoWidget::movedActiveMouse(int x, int y, EditorItem* root)
{
	if (activeAxis == 0)
	{
		//auto sign = axes[0]->getParentSceneNode()->getScale().z > 0 ? 1 : -1;
		root->move(Ogre::Vector3(0, 0, -0.25f*x));
	}
	if (activeAxis == 1)
	{
		root->move(Ogre::Vector3(-0.25f*x, 0, 0));
	}
	if (activeAxis == 2)
	{
		root->move(Ogre::Vector3(0, -0.25f*y, 0));
	}
}

void SelectionGizmo::MoveGizmoWidget::setDefaultMats()
{
	axes[0]->setMaterialName("scaleRed");
	axes[1]->setMaterialName("scaleBlue");
	axes[2]->setMaterialName("scaleGreen");
}

void SelectionGizmo::ScaleGizmoWidget::init(SelectionGizmo* parent)
{
	axes[0] = parent->axesPlanes[0];
	axes[1] = parent->axesPlanes[1];
	axes[2] = parent->axesPlanes[2];
	axes[3] = nullptr;

	axes[0]->setVisible(true);
	axes[1]->setVisible(true);
	axes[2]->setVisible(true);

	selectedMaterial = "scaleYellow";
	setDefaultMats();
}

void SelectionGizmo::ScaleGizmoWidget::movedActiveMouse(int x, int y, EditorItem* root)
{
	Ogre::Vector3 scale;

	if (activeAxis == 0)
	{
		scale.z += -0.25f*x;
		root->addScale(scale);
	}
	if (activeAxis == 1)
	{
		scale.x += -0.25f*x;
		root->addScale(scale);
	}
	if (activeAxis == 2)
	{
		scale.y += -0.25f*y;
		root->addScale(scale);
	}
}

void SelectionGizmo::ScaleGizmoWidget::setDefaultMats()
{
	axes[0]->setMaterialName("scaleRed");
	axes[1]->setMaterialName("scaleBlue");
	axes[2]->setMaterialName("scaleGreen");
}