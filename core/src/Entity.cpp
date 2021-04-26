#include "Entity.h"
#include "Game.h"
#include "JsonParser.h"
#include <glad/glad.h>
#ifdef __TEST_CORE
#include <TestCore.h>
#endif // __TEST_CORE

namespace Core
{
	static const float SPEED = 200.0f;

	int Entity::getEntityId() { return _entityId; }
	void Entity::setEntityId(int entityId) { _entityId = entityId; }
	// Constructor of the game class
	Entity::Entity(std::string s, glm::vec2 location, glm::vec2 scale,
				   double rotation, int spriteID, bool invisible)
		: mEntityName(s), mLocation(location), mScale(scale), mRotation(rotation),
		  mSpriteID(spriteID), mIsInvisible(invisible) {}

	Entity::Entity(const Entity &other)
		: mEntityName(other.mEntityName), mLocation(other.mLocation),
		  mScale(other.mScale), mRotation(other.mRotation),
		  mSpriteID(other.mSpriteID), mParentPage(other.mParentPage),
		  _entityId(other._entityId), mInScriptId(other.mInScriptId)
	{
	}

	// CONSTRUCTOR

	// =========================
	// ATTRIBUTES OPERATION
	std::string Entity::getName() { return this->mEntityName; }

	// Sets name based on a constant string, for example setName("Entity1")
	void Entity::setName(const std::string name) { this->mEntityName = name; }
	// =========================
	// PROPERTY OPERATION
	void Entity::setProperty(glm::vec2 l, glm::vec2 s, double r, int sid)
	{
		this->mLocation = l;
		this->mScale = s;
		this->mRotation = r;
		this->mSpriteID = sid;
	}
	// =========================
	// MEMBER OPERATION

	// =========================
	// UTILITY OPERATION

	double Entity::getRotation() { return this->mRotation; }
	void Entity::setRotation(double d) { this->mRotation = d; }
	void Entity::setSpriteID(int i)
	{
		this->mSpriteID = i;
		if (mGameSprites->atID(mSpriteID))
		{
			float *texcoords =
				mGameSprites->atID(mSpriteID)->getTextureCoordinates();
			// P1 texture coords
			mCoords[2] = texcoords[0];
			mCoords[3] = texcoords[1];

			// P2 texture coords
			mCoords[6] = texcoords[2];
			mCoords[7] = texcoords[3];

			// P3 texture coords
			mCoords[10] = texcoords[4];
			mCoords[11] = texcoords[5];

			// P4 texture coords
			mCoords[14] = texcoords[6];
			mCoords[15] = texcoords[7];
		}
	}
	int Entity::getSpriteID() { return this->mSpriteID; }

	Entity::Entity(std::string s) : mEntityName(s) { mScale = glm::vec2(0.0); }

	void Entity::calculateCoords(glm::vec2 location, glm::vec2 scale)
	{
		// Get the distances to the left/right and top/bottom of the entity from the
		// center
		float halfScaleWidth = scale.x / 2;
		float halfScaleHeight = scale.y / 2;

		// Calculate the smallest and greatest x and y (combinations of these make
		// the 4 corners of the entity)
		float lowX = location.x - halfScaleWidth;
		float highX = location.x + halfScaleWidth;
		float lowY = location.y - halfScaleHeight;
		float highY = location.y + halfScaleHeight;

		if (mGameSprites->atID(mSpriteID))
		{
			float *texcoords =
				mGameSprites->atID(mSpriteID)->getTextureCoordinates();
			// P1
			mCoords[0] = lowX;	// Top left x
			mCoords[1] = highY; // Top left y
			// P1 texture coords
			mCoords[2] = texcoords[0];
			mCoords[3] = texcoords[1];

			// P2
			mCoords[4] = lowX; // Bottom left x
			mCoords[5] = lowY; // Bottom left y
			// P2 texture coords
			mCoords[6] = texcoords[2];
			mCoords[7] = texcoords[3];

			// P3
			mCoords[8] = highX; // Top right x
			mCoords[9] = highY; // Top right y
			// P3 texture coords
			mCoords[10] = texcoords[4];
			mCoords[11] = texcoords[5];

			// P4
			mCoords[12] = highX; // Bottom right x
			mCoords[13] = lowY;	 // Bottom right y
			// P4 texture coords
			mCoords[14] = texcoords[6];
			mCoords[15] = texcoords[7];
		}
		else
		{
			// P1
			mCoords[0] = lowX;	// Top left x
			mCoords[1] = highY; // Top left y
			// P1 texture coords
			mCoords[2] = 0;
			mCoords[3] = 1;

			// P2
			mCoords[4] = lowX; // Bottom left x
			mCoords[5] = lowY; // Bottom left y
			// P2 texture coords
			mCoords[6] = 0;
			mCoords[7] = 0;

			// P3
			mCoords[8] = highX; // Top right x
			mCoords[9] = highY; // Top right y
			// P3 texture coords
			mCoords[10] = 1;
			mCoords[11] = 1;

			// P4
			mCoords[12] = highX; // Bottom right x
			mCoords[13] = lowY;	 // Bottom right y
			// P4 texture coords
			mCoords[14] = 1;
			mCoords[15] = 0;
		}
	}

	void Entity::setLocation(glm::vec2 location)
	{
		mLocation = location;
		calculateCoords(mLocation, mScale);
	}

	glm::vec2 Entity::getLocation() { return mLocation; }

	void Entity::setScale(glm::vec2 scale)
	{
		mScale = scale;
		calculateCoords(mLocation, mScale);
	}

	glm::vec2 Entity::getScale() { return mScale; }

	void Entity::render()
	{
		if (!mIsInvisible)
		{
			if (mGameSprites->atID(mSpriteID) != nullptr)
			{
				calculateCoords(mLocation, mScale);
				glActiveTexture(GL_TEXTURE0);
				Sprite *sprite = mGameSprites->atID(mSpriteID);
				glBindTexture(GL_TEXTURE_2D,
							  sprite->getOpenGLTextureID()); // Bind correct sprite
				if (sprite->getType() != SPRITE_TYPES::FULL)
				{
					// If the sprite is a looping sprite, the current sprite's
					// texture coordinates may need to change, so we update and get
					// them before rendering
					if (sprite->getType() == SPRITE_TYPES::LOOPING)
					{
						((LoopingSprite *)sprite)->updateTextureCoords();
					}
					float *texcoords = sprite->getTextureCoordinates();
					// P1 texture coords
					mCoords[2] = texcoords[0];
					mCoords[3] = texcoords[1];
					// P2 texture coords
					mCoords[6] = texcoords[2];
					mCoords[7] = texcoords[3];
					// P3 texture coords
					mCoords[10] = texcoords[4];
					mCoords[11] = texcoords[5];
					// P4 texture coords
					mCoords[14] = texcoords[6];
					mCoords[15] = texcoords[7];
				}
			}

			// Load the data of the 'coords' buffer into the currently bound array
			// buffer, VBO
			glBufferData(GL_ARRAY_BUFFER, sizeof(mCoords), mCoords,
						 GL_DYNAMIC_DRAW);
			// Draw the bound buffer (coords)
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			glBindTexture(GL_TEXTURE_2D, 0); // Unbind current sprite

#ifdef __TEST_CORE
			entityError = glGetError();
#endif // __TEST_CORE
		}
	}

	using json = nlohmann::json;
	Entity *Entity::fromJSON(json &root)
	{
		Entity *entity = new Entity("");
		entity->parse(root);
		return entity;
	}

	void Entity::parse(json &root)
	{
		setName(root.at("EntityName").get<std::string>());
		std::vector<json> locVec = root.at("location").get<std::vector<json>>();
		setLocation(glm::vec2(locVec[0].get<float>(), locVec[1].get<float>()));
		std::vector<json> scaleVec = root.at("scale").get<std::vector<json>>();
		setScale(glm::vec2(scaleVec[0].get<float>(), scaleVec[1].get<float>()));
		setInvisible(getData<bool>(root, "invisible"));
		setRotation(root.at("rotation").get<double>());
		setSpriteID(root.at("spriteID").get<int>());
		setInScriptId(getData<int>(root, "inScriptId"));
		setEntityId(getData<int>(root, "entityId"));
	}

	nlohmann::json Entity::serialize()
	{
		nlohmann::json root;
		root["EntityName"] = mEntityName;
		root["location"] = {mLocation.x, mLocation.y};
		root["scale"] = {mScale.x, mScale.y};
		root["rotation"] = mRotation;
		root["spriteID"] = mSpriteID;
		root["inScriptId"] = mInScriptId;
		root["entityId"] = _entityId;
		root["invisible"] = mIsInvisible;
		return root;
	}

	void Entity::setInScriptId(int id)
	{
		mInScriptId = id;
	}
	int Entity::getInScriptId()
	{
		return mInScriptId;
	}
	void Entity::setOutScriptId(int id)
	{
		mOutScriptId = id;
	}
	int Entity::getOutScriptId()
	{
		return mOutScriptId;
	}
} // namespace Core
