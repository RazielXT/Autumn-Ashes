#include "stdafx.h"
#include "Player.h"
#include "PostProcessMgr.h"
#include "PlayerPostProcess.h"
#include "PlayerParkour.h"
#include "PlayerSwimming.h"
#include "MUtils.h"
#include "GameStateManager.h"
#include "PlayerAbilities.h"
#include "PlayerSliding.h"
#include "SceneCubeMap.h"
#include "GameUi.h"
#include "..\model\PlayerModel.h"
#include "GUtils.h"

using namespace Ogre;

Player::Player(WorldMaterials* wMaterials) : pAudio(this)
{
	gravity = Ogre::Vector3(0, -9.0f, 0);
	tslf=0;
	bodySpeedAccum=0;
	slowingDown=1;
	startMoveBoost=0;

	groundID=-1;
	movespeed=10;

	mouseX=0;
	bodyVelocityL=0;
	gNormal=Vector3(0,1,0);

	forceDirection=Vector3::ZERO;
	mSceneMgr=Global::mSceneMgr;
	m_World=Global::mWorld;

	right_key=false;
	left_key=false;
	forw_key=false;
	back_key=false;
	moving=false;
	onGround = true;
	sprinting = false;

	hanging=false;
	climbing = BodyType(0);
	grabbedObj = false;
	wallrunning = false;
	sliding = false;
	transformed = false;
	surfaceSliding = false;

	inControl = true;
	inMoveControl = true;

	immortal = true;
	alive = true;

	ui = Global::gameMgr->myMenu->gameUi;
	wmaterials = wMaterials;

	initBody();

	autoTarget = new PlayerAutoTarget();
	pPostProcess = new PlayerPostProcess(this);
	pClimbing = new PlayerClimbing(this);
	pGrabbing = new PlayerGrab(this);
	pParkour = new PlayerParkour(this);
	pSwimming = new PlayerSwimming(this);
	pAbilities = new PlayerAbilities(this);
	pSliding = new PlayerSliding(this);
	pHanging = new PlayerHanging(this);
}

Player::~Player ()
{
	delete pAbilities;
	delete pSwimming;
	delete pPostProcess;
	delete pClimbing;
	delete pGrabbing;
	delete pParkour;
	delete pSliding;
	delete pModel;
	delete pCamera;
	delete autoTarget;
	delete pHanging;
}

void Player::saveState(PlayerStateInfo& info)
{
	info.position = bodyPosition;
	info.velocity = body->getVelocity();

	//info.camOrientation = necknode->getOrientation();
	//info.camPitch = camPitch;
}

void Player::loadState(PlayerStateInfo& info)
{
	body->setPositionOrientation(info.position, Ogre::Quaternion::IDENTITY);
	body->setVelocity(info.velocity);

	//necknode->setOrientation(info.camOrientation);
	//camPitch = info.camPitch;
}

void Player::initBody()
{
	Ogre::Entity* ent = mSceneMgr->createEntity("playerBody", "play2.mesh");
	SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode("CenterNode");
	node->attachObject(ent);
	node->setScale(0.5, 0.5, 0.5);
	//ent->setCastShadows(true);
	ent->setVisible(false);
	//ent->setMaterialName("redConcrete");
	//ent->setVisibilityFlags(1);
	OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(m_World, ent, 10));
	body = new OgreNewt::Body(m_World, col);

	uv = new OgreNewt::UpVector(body, Vector3::UNIT_Y);
	uv2 = new OgreNewt::UpVector(body, Vector3::UNIT_Z);

	Ogre::Vector3 inertia, offset;
	col->calculateInertialMatrix(inertia, offset);
#ifdef OGRENEWT_NO_COLLISION_SHAREDPTR
	//no longer need the collision shape object
	delete col;
#endif
	body->setMassMatrix(0.5, Vector3(20, 20, 20));
	body->setCenterOfMass(offset);
	body->setContinuousCollisionMode(1);
	body->setPositionOrientation(Ogre::Vector3(0, 100, 0), Ogre::Quaternion::IDENTITY);
	body->setLinearDamping(4);
	body->attachNode(node);
	body->setAutoSleep(0);
	//body->setMaterialGroupID(pmat);
	body->setCustomForceAndTorqueCallback<Player>(&Player::move_callback, this);

	pCamera = new PlayerCamera(this, node);
	//pModel = new PlayerModel(this, node);

	ent = mSceneMgr->createEntity("pl_base", "cone_p2.mesh");
	col_p = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(m_World, ent, 10));
}

void Player::updateSpeed()
{
	if (bodyVelocityL < 0.05)
		bodyVelocityL = 0;
	if (bodySpeedAccum + 0.05 < bodyVelocityL)
	{
		bodySpeedAccum += tslf*3.5f;
		if (bodySpeedAccum > 10)
			bodySpeedAccum = 10;
	}
	if (bodySpeedAccum > bodyVelocityL)
	{
		bodySpeedAccum -= tslf*1.5f;
		if (bodySpeedAccum < 0)
			bodySpeedAccum = 0;
	}

	mSceneMgr->setShadowColour(Ogre::ColourValue(bodySpeedAccum, bodyVelocityL / 3.0f));
}

void Player::move_callback_nothing(OgreNewt::Body* me, float timeStep, int threadIndex)
{
	//me->addForce(Ogre::Vector3(0,0,0));
}
void Player::default_callback(OgreNewt::Body* me, float timeStep, int threadIndex )
{
	me->addForce(Ogre::Vector3(0,-6,0));
}

static Ogre::String   mExePath;
static unsigned int   mVertexBufferSize = 0;
static unsigned int   mIndexBufferSize = 0;
static Ogre::Vector3 *mVertexBuffer = 0;
static unsigned long *mIndexBuffer = 0;

void GetMeshData(const Ogre::MeshPtr mesh, size_t &vertex_count, size_t &index_count,
                 const Ogre::Vector3 &position, const Ogre::Quaternion &orient, const Ogre::Vector3 &scale)
{
	bool added_shared = false;
	size_t current_offset = 0;
	size_t shared_offset = 0;
	size_t next_offset = 0;
	size_t index_offset = 0;

	vertex_count = index_count = 0;

	// Calculate how many vertices and indices we're going to need
	for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
	{
		Ogre::SubMesh* submesh = mesh->getSubMesh(i);

		// We only need to add the shared vertices once
		if (submesh->useSharedVertices)
		{
			if (!added_shared)
			{
				vertex_count += mesh->sharedVertexData->vertexCount;
				added_shared = true;
			}
		}
		else
		{
			vertex_count += submesh->vertexData->vertexCount;
		}

		// Add the indices
		index_count += submesh->indexData->indexCount;
	}


	if (vertex_count > mVertexBufferSize)
	{
		OGRE_FREE(mVertexBuffer, Ogre::MEMCATEGORY_GEOMETRY);
		mVertexBuffer = OGRE_ALLOC_T(Ogre::Vector3, vertex_count, Ogre::MEMCATEGORY_GEOMETRY);
		mVertexBufferSize = vertex_count;
	}

	if (index_count > mIndexBufferSize)
	{
		OGRE_FREE(mIndexBuffer, Ogre::MEMCATEGORY_GEOMETRY);
		mIndexBuffer = OGRE_ALLOC_T(unsigned long, index_count, Ogre::MEMCATEGORY_GEOMETRY);
		mIndexBufferSize = index_count;
	}

	added_shared = false;

	// Run through the submeshes again, adding the data into the arrays
	for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
	{
		Ogre::SubMesh* submesh = mesh->getSubMesh(i);

		Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

		if ((!submesh->useSharedVertices) || (submesh->useSharedVertices && !added_shared))
		{
			if (submesh->useSharedVertices)
			{
				added_shared = true;
				shared_offset = current_offset;
			}

			const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

			Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

			unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

			// There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
			//  as second argument. So make it float, to avoid trouble when Ogre::Real will
			//  be compiled/typedef-ed as double:
			//      Ogre::Real* pReal;
			float* pReal;

			for (size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
			{
				posElem->baseVertexPointerToElement(vertex, &pReal);

				Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);

				mVertexBuffer[current_offset + j] = (orient * (pt * scale)) + position;
			}

			vbuf->unlock();
			next_offset += vertex_data->vertexCount;
		}


		Ogre::IndexData* index_data = submesh->indexData;
		size_t numTris = index_data->indexCount / 3;
		Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

		bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

		unsigned long*  pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
		unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);

		size_t offset = (submesh->useSharedVertices) ? shared_offset : current_offset;

		if (use32bitindexes)
		{
			for (size_t k = 0; k < numTris * 3; ++k)
			{
				mIndexBuffer[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
			}
		}
		else
		{
			for (size_t k = 0; k < numTris * 3; ++k)
			{
				mIndexBuffer[index_offset++] = static_cast<unsigned long>(pShort[k]) + static_cast<unsigned long>(offset);
			}
		}

		ibuf->unlock();
		current_offset = next_offset;
	}
	index_count = index_offset;
}

bool PickEntity(Ogre::Ray &ray, Ogre::Entity **result, Ogre::Vector3 &hitpoint, const Ogre::String& excludeobject, Ogre::Real max_distance)
{
	Ogre::RaySceneQuery *mRaySceneQuery = Global::mSceneMgr->createRayQuery(Ogre::Ray());
	mRaySceneQuery->setRay(ray);
	mRaySceneQuery->setQueryMask(-1);
	mRaySceneQuery->setSortByDistance(true);

	unsigned int mVisibilityMask = -1;// OgitorsRoot::getSingletonPtr()->GetSceneManager()->getVisibilityMask();

	if (mRaySceneQuery->execute().size() <= 0) return (false);

	// at this point we have raycast to a series of different objects bounding boxes.
	// we need to test these different objects to see which is the first polygon hit.
	// there are some minor optimizations (distance based) that mean we wont have to
	// check all of the objects most of the time, but the worst case scenario is that
	// we need to test every triangle of every object.
	Ogre::Real closest_distance = max_distance;
	Ogre::Vector3 closest_result;
	Ogre::RaySceneQueryResult &query_result = mRaySceneQuery->getLastResults();

	for (size_t qr_idx = 0; qr_idx < query_result.size(); qr_idx++)
	{
		// stop checking if we have found a raycast hit that is closer
		// than all remaining entities
		if ((closest_distance >= 0.0f) && (closest_distance < query_result[qr_idx].distance))
			break;

		if (query_result[qr_idx].movable != NULL)
			GUtils::DebugPrint(query_result[qr_idx].movable->getName());

		// only check this result if its a hit against an entity
		if ((query_result[qr_idx].movable != NULL) && (query_result[qr_idx].movable->getMovableType().compare("Entity") == 0))
		{
			// get the entity to check
			Ogre::Entity *pentity = static_cast<Ogre::Entity*>(query_result[qr_idx].movable);

			if (!(pentity->getVisibilityFlags() & mVisibilityMask))
				continue;

			if (!pentity->getVisible() || (pentity->getName() == excludeobject))
				continue;

			if (pentity->getName() == "SkyXMeshEnt")
				continue;

			// mesh data to retrieve
			size_t vertex_count;
			size_t index_count;

			// get the mesh information
			GetMeshData(pentity->getMesh(), vertex_count, index_count,
			            pentity->getParentNode()->_getDerivedPosition(),
			            pentity->getParentNode()->_getDerivedOrientation(),
			            pentity->getParentNode()->_getDerivedScale());

			// test for hitting individual triangles on the mesh
			bool new_closest_found = false;
			for (int i = 0; i < static_cast<int>(index_count); i += 3)
			{
				/*assert(mIndexBuffer[i] < vertex_count);
				assert(mIndexBuffer[i + 1] < vertex_count);
				assert(mIndexBuffer[i + 2] < vertex_count);*/
				// check for a hit against this triangle
				std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(ray, mVertexBuffer[mIndexBuffer[i]],
				                                  mVertexBuffer[mIndexBuffer[i + 1]], mVertexBuffer[mIndexBuffer[i + 2]], true, false);

				// if it was a hit check if its the closest
				if (hit.first)
				{
					if ((closest_distance < 0.0f) || (hit.second < closest_distance))
					{
						// this is the closest so far, save it off
						closest_distance = hit.second;
						new_closest_found = true;
					}
				}
			}

			// if we found a new closest raycast for this object, update the
			// closest_result before moving on to the next object.
			if (new_closest_found)
			{
				closest_result = ray.getPoint(closest_distance);
				(*result) = pentity;
			}
		}
	}

	// return the result
	if (closest_distance != max_distance)
	{
		hitpoint = closest_result;
		return (true);
	}
	else
	{
		// raycast failed
		return (false);
	}
}

void Player::pressedKey(const OIS::KeyEvent &arg)
{
	if (hanging && pHanging->pressedKey(arg))
		return;

	if(arg.key == OIS::KC_SPACE && autoTarget->spacePressed())
		return;

	pSliding->pressedKey(arg);

	if (pAbilities->pressedKey(arg))
		return;

	switch (arg.key)
	{
	case OIS::KC_G:
	{
		/*static OgreNewt::BallAndSocket* gJoint = nullptr;
		static OgreNewt::BallAndSocket* gJoint2 = nullptr;
		static OgreNewt::Body* hbody = nullptr;

		if (!hbody)
		{
			OgreNewt::ConvexCollisionPtr col = OgreNewt::ConvexCollisionPtr(new OgreNewt::CollisionPrimitives::Box(Global::mWorld, Ogre::Vector3(1,1,1), 0));
			hbody = new OgreNewt::Body(Global::mWorld, col);

			Ogre::Vector3 inertia, offset;
			col->calculateInertialMatrix(inertia, offset);
			hbody->setMassMatrix(0.2f, inertia);
			hbody->setLinearDamping(1);
			hbody->setCenterOfMass(offset);
			hbody->setPositionOrientation(bodyPosition, Ogre::Quaternion::IDENTITY);
			hbody->setMaterialGroupID(wmaterials->plNoMove_mat);
			//body->setCustomForceAndTorqueCallback<Player>(&Player::move_callback, this);
		}

		if (!gJoint)
		{
			hbody->setPositionOrientation(pCamera->getPosition(), Ogre::Quaternion::IDENTITY);

			gJoint = new OgreNewt::BallAndSocket(body, hbody, pCamera->getPosition(), 0);
			gJoint->setCollisionState(0);

			gJoint2 = new OgreNewt::BallAndSocket(hbody, NULL, pCamera->getPosition() + pCamera->getFacingDirection() * 10, 0);
		}
		else
		{
			delete gJoint;
			gJoint = nullptr;
			delete gJoint2;
			gJoint2 = nullptr;
		}*/

		Ogre::Entity* result = nullptr;
		Ogre::Vector3 hitpoint;

		if (PickEntity(Ogre::Ray(pCamera->getPosition(), pCamera->getFacingDirection()), &result, hitpoint, "", 100000))
			GUtils::DebugPrint(result->getName());

		Global::mWindow->setDeactivateOnFocusChange(false);
		SetActiveWindow(NULL);

	}
	break;
	case OIS::KC_D:
		right_key = true;
		break;
	case OIS::KC_A:
		left_key = true;
		break;
	case OIS::KC_W:
		forw_key = true;
		break;
	case OIS::KC_S:
		back_key = true;
		break;
	}

	if(!inMoveControl)
		return;

	switch (arg.key)
	{
	case OIS::KC_LSHIFT:
		sprinting = true;
		sprintTimer = 0;
		break;

	case OIS::KC_C:
		pClimbing->release();
		break;

	case OIS::KC_SPACE:
		jump();
		break;

	case OIS::KC_Q:
		body->setVelocity(body->getVelocity() + Vector3(0, 9, 0));
		break;

	case OIS::KC_F:
		if(body->getVelocity().normalisedCopy().dotProduct(facingDir) < 0.7f)
			body->setVelocity(facingDir*15);
		else
			body->setVelocity(body->getVelocity() + facingDir * 15);

		break;

	case OIS::KC_G:
	{
		Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Pbody: "+Ogre::StringConverter::toString(body->getPosition()),Ogre::LML_NORMAL);
		Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Headnod: "+Ogre::StringConverter::toString(mSceneMgr->getSceneNode("HeadNod")->getPosition())+","+Ogre::StringConverter::toString(mSceneMgr->getSceneNode("HeadNod")->_getDerivedPosition()),Ogre::LML_NORMAL);
		Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Necknod: "+Ogre::StringConverter::toString(mSceneMgr->getSceneNode("NeckNod")->getPosition())+","+Ogre::StringConverter::toString(mSceneMgr->getSceneNode("NeckNod")->_getDerivedPosition()),Ogre::LML_NORMAL);
		Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Camnod: "+Ogre::StringConverter::toString(mSceneMgr->getSceneNode("CamNod")->getPosition())+","+Ogre::StringConverter::toString(mSceneMgr->getSceneNode("CamNod")->_getDerivedPosition()),Ogre::LML_NORMAL);
		Ogre::LogManager::getSingleton().getLog("RuntimeEvents.log")->logMessage("Camera: "+Ogre::StringConverter::toString(mSceneMgr->getCamera("Camera")->getPosition()),Ogre::LML_NORMAL);

		break;
	}
	}
}

void Player::releasedKey(const OIS::KeyEvent &arg)
{
	pSliding->releasedKey(arg);
	pAbilities->releasedKey(arg);

	if (hanging && pHanging->releasedKey(arg))
		return;

	switch (arg.key)
	{
	case OIS::KC_D:
		right_key=false;
		break;
	case OIS::KC_A:
		left_key=false;
		break;
	case OIS::KC_W:
		forw_key=false;
		break;
	case OIS::KC_S:
		back_key=false;
		break;
	case OIS::KC_LSHIFT:
		sprinting = false;
		break;
	}
}

void Player::pressedMouse(const OIS::MouseEvent &arg,OIS::MouseButtonID id)
{
	if(!pAbilities->pressedMouse(arg, id))
		switch (id)
		{
		case OIS::MB_Right:
			if (!grabbedObj)
				pGrabbing->tryToGrab();

			break;
		}
}
void Player::releasedMouse(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (!pAbilities->releasedMouse(arg, id))
		switch (id)
		{
		case OIS::MB_Right:
			if (grabbedObj)
				pGrabbing->releaseObj();

			break;
		}
}
void Player::movedMouse(const OIS::MouseEvent &e)
{
	if (hanging)
		pHanging->movedMouse(e);

	if (!pParkour->isRolling())
		mouseX = (int)(-1 * e.state.X.rel*Global::timestep);

	int mouseY = (int)(-1 * e.state.Y.rel*Global::timestep);

	if (inControl && pCamera->ownsCamera)
		pCamera->rotateCamera(mouseX / 10.0f, mouseY / 10.0f);
	else
		pSliding->movedMouse(e);
}


void Player::die()
{
	if (!alive)
		return;

	delete uv;
	delete uv2;
	body->setLinearDamping(2);
	body->setAngularDamping(Ogre::Vector3(2));
	body->setMassMatrix(5,Ogre::Vector3(1,1,1));

	Ogre::Vector3 v = body->getVelocity();
	v.y = 0;
	v.normalise();
	if(v!=Ogre::Vector3::ZERO)
		body->setOmega(v*bodyVelocityL);
	else
		body->setOmega(Ogre::Vector3(5,0,0));

	enableControl(false);

	alive=false;
}


void Player::hideBody()
{
	body->setPositionOrientation(Vector3(0, 1000, 0), Quaternion::IDENTITY);
	body->freeze();
}

void Player::update(Real time)
{
	tslf = time*Global::timestep;
	pCamera->update();
	//pModel->update(time);

	pPostProcess->update(tslf);

	pAudio.update(tslf);

	if (!alive)
		return;

	updateStats();

	updateDirectionForce();

	pClimbing->update(tslf);

	pCamera->updateHead();

	updateSpeed();
}

void Player::updateStats()
{
	moving = (right_key || forw_key || back_key || left_key) && inMoveControl;

	bodyPosition = body->getPosition();

	sprintTimer += tslf;

	updateGroundStats();

	bodyVelocityL = body->getVelocity().length();
	/*if (bodyVelocityL > 20)
	{
		bodyVelocityL = 20;
		body->setVelocity(20 * body->getVelocity() / bodyVelocityL);
	}*/

	pSwimming->update(tslf);
	pAbilities->update(tslf);

	if (hanging)
		pHanging->update(tslf);

	bool readyToSlide = (inControl && !pParkour->isRolling() && !wallrunning && climbing==0 && !hanging);
	pSliding->update(tslf, readyToSlide);

	autoTarget->updateAutoTarget(camPosition, facingDir, tslf, 35.0f);

	if (!inControl)
		return;

	if (!onGround && !wallrunning && !hanging && !climbing && !pParkour->isRolling())
	{
		pClimbing->updateClimbingPossibility();

		if (!climbing && !hanging)
			pParkour->updateParkourPossibility();
	}
	else if(climbing)
	{
		pClimbing->updateClimbingStats();
	}
	else if (wallrunning)
	{
		pParkour->updateWallrunning();
	}
}

void Player::updateUseGui()
{
	const float distanceFacingProbe = 20;

	auto camPos = pCamera->getPosition();
	OgreNewt::BasicRaycast ray(m_World, camPos, camPos + pCamera->getFacingDirection() * distanceFacingProbe, false);
	OgreNewt::BasicRaycast::BasicRaycastInfo info = ray.getFirstHit();

	facingWallDistance = info.mBody ? distanceFacingProbe*info.getDistance() : distanceFacingProbe;

	if (inControl)
		if (!grabbedObj && !climbing && !hanging)
			if (!wallrunning && info.mBody && facingWallDistance <= 2.0f)
			{
				//grabbable
				if (info.mBody->getType() == Grabbable)
				{
					ui->showUseGui(Ui_Pickup);
				}
				//climbable
				else if (info.mBody->getType() == Pullup_old)
				{
					ui->showUseGui(Ui_Climb);
				}
				//trigger
				else if (info.mBody->getType() == Trigger)
				{
					Ogre::Any any = info.mBody->getUserData();

					if (!any.isEmpty())
					{
						bodyUserData* a0 = Ogre::any_cast<bodyUserData*>(any);
						if (a0->enabledTrigger)
						{
							ui->showUseGui(Ui_Use);
						}
					}
				}
			}
}
