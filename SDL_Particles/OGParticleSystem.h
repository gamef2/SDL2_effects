//由悠然小赐移植
//cocos2dx粒子引擎
//参考原有SDL2粒子引擎的部分代码
//增加了渲染叠加
//qq108201645
#ifndef _OGPARTICLE_SYSTEM_H_
#define _OGPARTICLE_SYSTEM_H_

#include "Vec2.h"
#include <memory>

class LTexture;

enum class BlendFunc
{
	DISABLE = SDL_BLENDMODE_NONE,
	ALPHA_PREMULTIPLIED = SDL_BLENDMODE_BLEND,
	ALPHA_NON_PREMULTIPLIED = SDL_BLENDMODE_BLEND,
	ADDITIVE = SDL_BLENDMODE_ADD

};
struct Color4F {
	float r, g, b, a;
};


//#define OG_SAFE_DELETE(p)           do { delete (p); (p) = nullptr; } while (0)
//
//#define OG_SAFE_FREE(p)             do { if(p) { free(p); (p) = nullptr; } } while(0)
//#define OG_SAFE_RELEASE(p)          do { if(p) { (p)= nullptr; } } while(0)
//
//#define OG_CONTENT_SCALE_FACTOR() 1

inline float clampf(float value, float min_inclusive, float max_inclusive)
{
	if (min_inclusive > max_inclusive) {
		std::swap(min_inclusive, max_inclusive);
	}
	return value < min_inclusive ? min_inclusive : value < max_inclusive ? value : max_inclusive;
}

#define MATH_TOLERANCE              2e-37f
#define OG_DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) * 0.01745329252f) // PI / 180
#define OG_RADIANS_TO_DEGREES(__ANGLE__) ((__ANGLE__) * 57.29577951f) // PI * 180


struct  Vec2;

struct particle_point
{
	float x;
	float y;
};

class  ParticleData
{
public:
	float* posx;
	float* posy;
	float* startPosX;
	float* startPosY;

	float* colorR;
	float* colorG;
	float* colorB;
	float* colorA;

	float* deltaColorR;
	float* deltaColorG;
	float* deltaColorB;
	float* deltaColorA;

	float* size;
	float* deltaSize;
	float* rotation;
	float* deltaRotation;
	float* timeToLive;
	unsigned int* atlasIndex;

	//! Mode A: gravity, direction, radial accel, tangential accel
	struct {
		float* dirX;
		float* dirY;
		float* radialAccel;
		float* tangentialAccel;
	} modeA;

	//! Mode B: radius mode
	struct {
		float* angle;
		float* degreesPerSecond;
		float* radius;
		float* deltaRadius;
	} modeB;

	int maxCount;
	ParticleData();
	bool init(int count);
	void release();
	unsigned int getMaxCount() { return maxCount; }

	void copyParticle(int p1, int p2)
	{
		posx[p1] = posx[p2];
		posy[p1] = posy[p2];
		startPosX[p1] = startPosX[p2];
		startPosY[p1] = startPosY[p2];

		colorR[p1] = colorR[p2];
		colorG[p1] = colorG[p2];
		colorB[p1] = colorB[p2];
		colorA[p1] = colorA[p2];

		deltaColorR[p1] = deltaColorR[p2];
		deltaColorG[p1] = deltaColorG[p2];
		deltaColorB[p1] = deltaColorB[p2];
		deltaColorA[p1] = deltaColorA[p2];

		size[p1] = size[p2];
		deltaSize[p1] = deltaSize[p2];

		rotation[p1] = rotation[p2];
		deltaRotation[p1] = deltaRotation[p2];

		timeToLive[p1] = timeToLive[p2];

		atlasIndex[p1] = atlasIndex[p2];

		modeA.dirX[p1] = modeA.dirX[p2];
		modeA.dirY[p1] = modeA.dirY[p2];
		modeA.radialAccel[p1] = modeA.radialAccel[p2];
		modeA.tangentialAccel[p1] = modeA.tangentialAccel[p2];

		modeB.angle[p1] = modeB.angle[p2];
		modeB.degreesPerSecond[p1] = modeB.degreesPerSecond[p2];
		modeB.radius[p1] = modeB.radius[p2];
		modeB.deltaRadius[p1] = modeB.deltaRadius[p2];

	}
};




class  ParticleSystem
{
public:
	/** Mode
	 * @js cc.ParticleSystem.MODE_GRAVITY;
	 */
	enum class Mode
	{
		GRAVITY,
		RADIUS,
	};

	/** PositionType
	 Possible types of particle positions.
	 * @js cc.ParticleSystem.TYPE_FREE
	 */
	enum class PositionType
	{
		FREE, /** Living particles are attached to the world and are unaffected by emitter repositioning. */

		RELATIVE, /** Living particles are attached to the world but will follow the emitter repositioning.
				   Use case: Attach an emitter to an sprite, and you want that the emitter follows the sprite.*/

		GROUPED, /** Living particles are attached to the emitter and are translated along with it. */

	};

	//* @enum
	enum {
		/** The Particle emitter lives forever. */
		DURATION_INFINITY = -1,

		/** The starting size of the particle is equal to the ending size. */
		START_SIZE_EQUAL_TO_END_SIZE = -1,

		/** The starting radius of the particle is equal to the ending radius. */
		START_RADIUS_EQUAL_TO_END_RADIUS = -1,
	};

	enum PARTICLE {

	};


	ParticleSystem();
	ParticleSystem(int numberOfParticles);

	virtual ~ParticleSystem();

	ParticleSystem(const std::string& plistFile);

	/** Gets all ParticleSystem references
	 */
	 //static std::vector<ParticleSystem*>& getAllParticleSystems();
public:
	void addParticles(int count);

	void stopSystem();
	/** Kill all living particles.
	 */
	void resetSystem();
	/** Whether or not the system is full.
	 *
	 * @return True if the system is full.
	 */
	bool isFull();

	/** Update the verts position data of particle,
	 should be overridden by subclasses.
	 */
	void updateParticleQuads();
	/** Update the VBO verts buffer which does not use batch node,
	 should be overridden by subclasses. */
	void postStep();

	/** Call the update method with no time..
	 */
	void updateWithNoTime();

	/** Whether or not the particle system removed self on finish.
	 *
	 * @return True if the particle system removed self on finish.
	 */
	virtual bool isAutoRemoveOnFinish() const;

	/** Set the particle system auto removed it self on finish.
	 *
	 * @param var True if the particle system removed self on finish.
	 */
	virtual void setAutoRemoveOnFinish(bool var);

	// mode A
	/** Gets the gravity.
	 *
	 * @return The gravity.
	 */
	virtual const Vec2& getGravity();
	/** Sets the gravity.
	 *
	 * @param g The gravity.
	 */
	virtual void setGravity(const Vec2& g);
	/** Gets the speed.
	 *
	 * @return The speed.
	 */
	virtual float getSpeed() const;
	/** Sets the speed.
	 *
	 * @param speed The speed.
	 */
	virtual void setSpeed(float speed);
	/** Gets the speed variance.
	 *
	 * @return The speed variance.
	 */
	virtual float getSpeedVar() const;
	/** Sets the speed variance.
	 *
	 * @param speed The speed variance.
	 */
	virtual void setSpeedVar(float speed);
	/** Gets the tangential acceleration.
	 *
	 * @return The tangential acceleration.
	 */
	virtual float getTangentialAccel() const;
	/** Sets the tangential acceleration.
	 *
	 * @param t The tangential acceleration.
	 */
	virtual void setTangentialAccel(float t);
	/** Gets the tangential acceleration variance.
	 *
	 * @return The tangential acceleration variance.
	 */
	virtual float getTangentialAccelVar() const;
	/** Sets the tangential acceleration variance.
	 *
	 * @param t The tangential acceleration variance.
	 */
	virtual void setTangentialAccelVar(float t);
	/** Gets the radial acceleration.
	 *
	 * @return The radial acceleration.
	 */
	virtual float getRadialAccel() const;
	/** Sets the radial acceleration.
	 *
	 * @param t The radial acceleration.
	 */
	virtual void setRadialAccel(float t);
	/** Gets the radial acceleration variance.
	 *
	 * @return The radial acceleration variance.
	 */
	virtual float getRadialAccelVar() const;
	/** Sets the radial acceleration variance.
	 *
	 * @param t The radial acceleration variance.
	 */
	virtual void setRadialAccelVar(float t);
	/** Whether or not the rotation of each particle to its direction.
	 *
	 * @return True if the rotation is the direction.
	 */
	virtual bool getRotationIsDir() const;
	/** Sets the rotation of each particle to its direction.
	 *
	 * @param t True if the rotation is the direction.
	 */
	virtual void setRotationIsDir(bool t);
	// mode B
	/** Gets the start radius.
	 *
	 * @return The start radius.
	 */
	virtual float getStartRadius() const;
	/** Sets the start radius.
	 *
	 * @param startRadius The start radius.
	 */
	virtual void setStartRadius(float startRadius);
	/** Gets the start radius variance.
	 *
	 * @return The start radius variance.
	 */
	virtual float getStartRadiusVar() const;
	/** Sets the start radius variance.
	 *
	 * @param startRadiusVar The start radius variance.
	 */
	virtual void setStartRadiusVar(float startRadiusVar);
	/** Gets the end radius.
	 *
	 * @return The end radius.
	 */
	virtual float getEndRadius() const;
	/** Sets the end radius.
	 *
	 * @param endRadius The end radius.
	 */
	virtual void setEndRadius(float endRadius);
	/** Gets the end radius variance.
	 *
	 * @return The end radius variance.
	 */
	virtual float getEndRadiusVar() const;
	/** Sets the end radius variance.
	 *
	 * @param endRadiusVar The end radius variance.
	 */
	virtual void setEndRadiusVar(float endRadiusVar);
	/** Gets the number of degrees to rotate a particle around the source pos per second.
	 *
	 * @return The number of degrees to rotate a particle around the source pos per second.
	 */
	virtual float getRotatePerSecond() const;
	/** Sets the number of degrees to rotate a particle around the source pos per second.
	 *
	 * @param degrees The number of degrees to rotate a particle around the source pos per second.
	 */
	virtual void setRotatePerSecond(float degrees);
	/** Gets the rotate per second variance.
	 *
	 * @return The rotate per second variance.
	 */
	virtual float getRotatePerSecondVar() const;
	/** Sets the rotate per second variance.
	 *
	 * @param degrees The rotate per second variance.
	 */
	virtual void setRotatePerSecondVar(float degrees);

	virtual void setScale(float s);
	virtual void setRotation(float newRotation);
	virtual void setScaleX(float newScaleX);
	virtual void setScaleY(float newScaleY);

	/** Whether or not the particle system is active.
	 *
	 * @return True if the particle system is active.
	 */
	virtual bool isActive() const;
	/** Whether or not the particle system is blend additive.
	 *
	 * @return True if the particle system is blend additive.
	 */
	virtual bool isBlendAdditive() const;
	/** Sets the particle system blend additive.
	 *
	 * @param value True if the particle system is blend additive.
	 */
	virtual void setBlendAdditive(bool value);

	/** Gets the batch node.
	 *
	 * @return The batch node.
	 */
	 //virtual ParticleBatchNode* getBatchNode() const;
	 /** Sets the batch node.
	  *
	  * @param batchNode The batch node.
	  */
	  // virtual void setBatchNode(ParticleBatchNode* batchNode);

	   /** Gets the index of system in batch node array.
		*
		* @return The index of system in batch node array.
		*/
	int getAtlasIndex() const { return _atlasIndex; }
	/** Sets the index of system in batch node array.
	 *
	 * @param index The index of system in batch node array.
	 */
	void setAtlasIndex(int index) { _atlasIndex = index; }

	/** Gets the Quantity of particles that are being simulated at the moment.
	 *
	 * @return The Quantity of particles that are being simulated at the moment.
	 */
	unsigned int getParticleCount() const { return _particleCount; }

	/** Gets how many seconds the emitter will run. -1 means 'forever'.
	 *
	 * @return The seconds that the emitter will run. -1 means 'forever'.
	 */
	float getDuration() const { return _duration; }
	/** Sets how many seconds the emitter will run. -1 means 'forever'.
	 *
	 * @param duration The seconds that the emitter will run. -1 means 'forever'.
	 */
	void setDuration(float duration) { _duration = duration; }

	/** Gets the source position of the emitter.
	 *
	 * @return The source position of the emitter.
	 */
	const Vec2& getSourcePosition() const { return _sourcePosition; }
	/** Sets the source position of the emitter.
	 *
	 * @param pos The source position of the emitter.
	 */
	void setSourcePosition(const Vec2& pos) { _sourcePosition = pos; }

	/** Gets the position variance of the emitter.
	 *
	 * @return The position variance of the emitter.
	 */
	const Vec2& getPosVar() const { return _posVar; }
	/** Sets the position variance of the emitter.
	 *
	 * @param pos The position variance of the emitter.
	 */
	void setPosVar(const Vec2& pos) { _posVar = pos; }

	/** Gets the life of each particle.
	 *
	 * @return The life of each particle.
	 */
	float getLife() const { return _life; }
	/** Sets the life of each particle.
	 *
	 * @param life The life of each particle.
	 */
	void setLife(float life) { _life = life; }

	/** Gets the life variance of each particle.
	 *
	 * @return The life variance of each particle.
	 */
	float getLifeVar() const { return _lifeVar; }
	/** Sets the life variance of each particle.
	 *
	 * @param lifeVar The life variance of each particle.
	 */
	void setLifeVar(float lifeVar) { _lifeVar = lifeVar; }

	/** Gets the angle of each particle.
	 *
	 * @return The angle of each particle.
	 */
	float getAngle() const { return _angle; }
	/** Sets the angle of each particle.
	 *
	 * @param angle The angle of each particle.
	 */
	void setAngle(float angle) { _angle = angle; }

	/** Gets the angle variance of each particle.
	 *
	 * @return The angle variance of each particle.
	 */
	float getAngleVar() const { return _angleVar; }
	/** Sets the angle variance of each particle.
	 *
	 * @param angleVar The angle variance of each particle.
	 */
	void setAngleVar(float angleVar) { _angleVar = angleVar; }

	/** Switch between different kind of emitter modes:
	 - kParticleModeGravity: uses gravity, speed, radial and tangential acceleration.
	 - kParticleModeRadius: uses radius movement + rotation.
	 *
	 * @return The mode of the emitter.
	 */
	Mode getEmitterMode() const { return _emitterMode; }
	/** Sets the mode of the emitter.
	 *
	 * @param mode The mode of the emitter.
	 */
	void setEmitterMode(Mode mode) { _emitterMode = mode; }

	/** Gets the start size in pixels of each particle.
	 *
	 * @return The start size in pixels of each particle.
	 */
	float getStartSize() const { return _startSize; }
	/** Sets the start size in pixels of each particle.
	 *
	 * @param startSize The start size in pixels of each particle.
	 */
	void setStartSize(float startSize) { _startSize = startSize; }

	/** Gets the start size variance in pixels of each particle.
	 *
	 * @return The start size variance in pixels of each particle.
	 */
	float getStartSizeVar() const { return _startSizeVar; }
	/** Sets the start size variance in pixels of each particle.
	 *
	 * @param sizeVar The start size variance in pixels of each particle.
	 */
	void setStartSizeVar(float sizeVar) { _startSizeVar = sizeVar; }

	/** Gets the end size in pixels of each particle.
	 *
	 * @return The end size in pixels of each particle.
	 */
	float getEndSize() const { return _endSize; }
	/** Sets the end size in pixels of each particle.
	 *
	 * @param endSize The end size in pixels of each particle.
	 */
	void setEndSize(float endSize) { _endSize = endSize; }

	/** Gets the end size variance in pixels of each particle.
	 *
	 * @return The end size variance in pixels of each particle.
	 */
	float getEndSizeVar() const { return _endSizeVar; }
	/** Sets the end size variance in pixels of each particle.
	 *
	 * @param sizeVar The end size variance in pixels of each particle.
	 */
	void setEndSizeVar(float sizeVar) { _endSizeVar = sizeVar; }

	/** Gets the start color of each particle.
	 *
	 * @return The start color of each particle.
	 */
	const Color4F& getStartColor() const { return _startColor; }
	/** Sets the start color of each particle.
	 *
	 * @param color The start color of each particle.
	 */
	void setStartColor(const Color4F& color) { _startColor = color; }

	/** Gets the start color variance of each particle.
	 *
	 * @return The start color variance of each particle.
	 */
	const Color4F& getStartColorVar() const { return _startColorVar; }
	/** Sets the start color variance of each particle.
	 *
	 * @param color The start color variance of each particle.
	 */
	void setStartColorVar(const Color4F& color) { _startColorVar = color; }

	/** Gets the end color and end color variation of each particle.
	 *
	 * @return The end color and end color variation of each particle.
	 */
	const Color4F& getEndColor() const { return _endColor; }
	/** Sets the end color and end color variation of each particle.
	 *
	 * @param color The end color and end color variation of each particle.
	 */
	void setEndColor(const Color4F& color) { _endColor = color; }

	/** Gets the end color variance of each particle.
	 *
	 * @return The end color variance of each particle.
	 */
	const Color4F& getEndColorVar() const { return _endColorVar; }
	/** Sets the end color variance of each particle.
	 *
	 * @param color The end color variance of each particle.
	 */
	void setEndColorVar(const Color4F& color) { _endColorVar = color; }

	/** Gets the start spin of each particle.
	 *
	 * @return The start spin of each particle.
	 */
	float getStartSpin() const { return _startSpin; }
	/** Sets the start spin of each particle.
	 *
	 * @param spin The start spin of each particle.
	 */
	void setStartSpin(float spin) { _startSpin = spin; }

	/** Gets the start spin variance of each particle.
	 *
	 * @return The start spin variance of each particle.
	 */
	float getStartSpinVar() const { return _startSpinVar; }
	/** Sets the start spin variance of each particle.
	 *
	 * @param pinVar The start spin variance of each particle.
	 */
	void setStartSpinVar(float pinVar) { _startSpinVar = pinVar; }

	/** Gets the end spin of each particle.
	 *
	 * @return The end spin of each particle.
	 */
	float getEndSpin() const { return _endSpin; }
	/** Sets the end spin of each particle.
	 *
	 * @param endSpin The end spin of each particle.
	 */
	void setEndSpin(float endSpin) { _endSpin = endSpin; }

	/** Gets the end spin variance of each particle.
	 *
	 * @return The end spin variance of each particle.
	 */
	float getEndSpinVar() const { return _endSpinVar; }
	/** Sets the end spin variance of each particle.
	 *
	 * @param endSpinVar The end spin variance of each particle.
	 */
	void setEndSpinVar(float endSpinVar) { _endSpinVar = endSpinVar; }

	/** Gets the emission rate of the particles.
	 *
	 * @return The emission rate of the particles.
	 */
	float getEmissionRate() const { return _emissionRate; }
	/** Sets the emission rate of the particles.
	 *
	 * @param rate The emission rate of the particles.
	 */
	void setEmissionRate(float rate) { _emissionRate = rate; }

	/** Gets the maximum particles of the system.
	 *
	 * @return The maximum particles of the system.
	 */
	virtual int getTotalParticles() const;
	/** Sets the maximum particles of the system.
	 *
	 * @param totalParticles The maximum particles of the system.
	 */
	virtual void setTotalParticles(int totalParticles);

	/** does the alpha value modify color */
	void setOpacityModifyRGB(bool opacityModifyRGB) { _opacityModifyRGB = opacityModifyRGB; }
	bool isOpacityModifyRGB() const { return _opacityModifyRGB; }

	/** Gets the particles movement type: Free or Grouped.
	 @since v0.8
	 *
	 * @return The particles movement type.
	 */
	PositionType getPositionType() const { return _positionType; }
	/** Sets the particles movement type: Free or Grouped.
	@since v0.8
	 *
	 * @param type The particles movement type.
	 */
	void setPositionType(PositionType type) { _positionType = type; }

	// Overrides
//     virtual void onEnter() ;
//     virtual void onExit() ;
	virtual void update(float dt);
	virtual std::shared_ptr<LTexture> getTexture() const;
	virtual void setTexture(std::shared_ptr<LTexture> texture);

	/**
	*@code
	*When this function bound into js or lua,the parameter will be changed
	*In js: var setBlendFunc(var src, var dst)
	*In lua: local setBlendFunc(local src, local dst)
	*@endcode
	*/
	//   virtual void setBlendFunc(const BlendFunc &blendFunc) ;
	   /**
	   * @js NA
	   * @lua NA
	   */
	   // virtual const BlendFunc &getBlendFunc() const ;

	const std::string& getResourceFile() const { return _plistFile; }

	/// @{
	/// @name implement Playable Protocol
	virtual void start();
	virtual void stop();
	/// @} end of PlayableProtocol

	void setSourcePositionCompatible(bool sourcePositionCompatible) { _sourcePositionCompatible = sourcePositionCompatible; }
	bool isSourcePositionCompatible() const { return _sourcePositionCompatible; }

	/** initializes a ParticleSystem*/
	bool init();

	//  bool initWithFile(const std::string& plistFile);

	  /** initializes a QuadParticleSystem from a Dictionary.
	   @since v0.99.3
	   */
	   //   bool initWithDictionary(ValueMap& dictionary);

		  /** initializes a particle system from a NSDictionary and the path from where to load the png
		   @since v2.1
		   */
		   //   bool initWithDictionary(ValueMap& dictionary, const std::string& dirname);

			  //! Initializes a system with a fixed number of particles
	virtual bool initWithTotalParticles(int numberOfParticles);

	/** Are the emissions paused
	 @return True if the emissions are paused, else false
	 */
	virtual bool isPaused() const;

	/* Pause the emissions*/
	virtual void pauseEmissions();

	/* UnPause the emissions*/
	virtual void resumeEmissions();

protected:
	// virtual void updateBlendFunc();

private:
	friend class EngineDataManager;

	/** Internal use only, it's used by EngineDataManager class for Android platform */
	static void setTotalParticleCountFactor(float factor);

protected:

	/** whether or not the particles are using blend additive.
	 If enabled, the following blending function will be used.
	 @code
	 source blend function = BlendFactor::SRC_ALPHA;
	 dest blend function = BlendFactor::ONE;
	 @endcode
	 */
	bool _isBlendAdditive;

	/** whether or not the node will be auto-removed when it has no particles left.
	 By default it is false.
	 @since v0.8
	 */
	bool _isAutoRemoveOnFinish;

	std::string _plistFile;
	//! time elapsed since the start of the system (in seconds)
	float _elapsed;

	// Different modes
	//! Mode A:Gravity + Tangential Accel + Radial Accel
	struct {
		/** Gravity value. Only available in 'Gravity' mode. */
		Vec2 gravity;
		/** speed of each particle. Only available in 'Gravity' mode.  */
		float speed;
		/** speed variance of each particle. Only available in 'Gravity' mode. */
		float speedVar;
		/** tangential acceleration of each particle. Only available in 'Gravity' mode. */
		float tangentialAccel;
		/** tangential acceleration variance of each particle. Only available in 'Gravity' mode. */
		float tangentialAccelVar;
		/** radial acceleration of each particle. Only available in 'Gravity' mode. */
		float radialAccel;
		/** radial acceleration variance of each particle. Only available in 'Gravity' mode. */
		float radialAccelVar;
		/** set the rotation of each particle to its direction Only available in 'Gravity' mode. */
		bool rotationIsDir;
	} modeA;

	//! Mode B: circular movement (gravity, radial accel and tangential accel don't are not used in this mode)
	struct {
		/** The starting radius of the particles. Only available in 'Radius' mode. */
		float startRadius;
		/** The starting radius variance of the particles. Only available in 'Radius' mode. */
		float startRadiusVar;
		/** The ending radius of the particles. Only available in 'Radius' mode. */
		float endRadius;
		/** The ending radius variance of the particles. Only available in 'Radius' mode. */
		float endRadiusVar;
		/** Number of degrees to rotate a particle around the source pos per second. Only available in 'Radius' mode. */
		float rotatePerSecond;
		/** Variance in degrees for rotatePerSecond. Only available in 'Radius' mode. */
		float rotatePerSecondVar;
	} modeB;

	//particle data
	ParticleData _particleData;

	//Emitter name
	std::string _configName;

	// color modulate
	//    BOOL colorModulate;

	//! How many particles can be emitted per second
	float _emitCounter;

	// Optimization
	//OG_UPDATE_PARTICLE_IMP    updateParticleImp;
	//SEL                        updateParticleSel;

	/** weak reference to the SpriteBatchNode that renders the Sprite */
	//ParticleBatchNode* _batchNode;

	// index of system in batch node array
	int _atlasIndex;

	//true if scaled or rotated
	bool _transformSystemDirty;
	// Number of allocated particles
	int _allocatedParticles;

	/** Is the emitter active */
	bool _isActive;

	/** Quantity of particles that are being simulated at the moment */
	int _particleCount;
	/** The factor affects the total particle count, its value should be 0.0f ~ 1.0f, default 1.0f*/
	static float __totalParticleCountFactor;

	/** How many seconds the emitter will run. -1 means 'forever' */
	float _duration;
	/** sourcePosition of the emitter */
	Vec2 _sourcePosition;
	/** Position variance of the emitter */
	Vec2 _posVar;
	/** life, and life variation of each particle */
	float _life;
	/** life variance of each particle */
	float _lifeVar;
	/** angle and angle variation of each particle */
	float _angle;
	/** angle variance of each particle */
	float _angleVar;

	/** Switch between different kind of emitter modes:
	 - kParticleModeGravity: uses gravity, speed, radial and tangential acceleration
	 - kParticleModeRadius: uses radius movement + rotation
	 */
	Mode _emitterMode;

	/** start size in pixels of each particle */
	float _startSize;
	/** size variance in pixels of each particle */
	float _startSizeVar;
	/** end size in pixels of each particle */
	float _endSize;
	/** end size variance in pixels of each particle */
	float _endSizeVar;
	/** start color of each particle */
	Color4F _startColor;
	/** start color variance of each particle */
	Color4F _startColorVar;
	/** end color and end color variation of each particle */
	Color4F _endColor;
	/** end color variance of each particle */
	Color4F _endColorVar;
	//* initial angle of each particle
	float _startSpin;
	//* initial angle of each particle
	float _startSpinVar;
	//* initial angle of each particle
	float _endSpin;
	//* initial angle of each particle
	float _endSpinVar;
	/** emission rate of the particles */
	float _emissionRate;
	/** maximum particles of the system */
	int _totalParticles;
	/** conforms to CocosNodeTexture protocol */
  //  LTexture* _texture;
	/** conforms to CocosNodeTexture protocol */
	BlendFunc _blendFunc;
	/** does the alpha value modify color */
	bool _opacityModifyRGB;
	/** does FlippedY variance of each particle */
	int _yCoordFlipped;


	/** particles movement type: Free or Grouped
	 @since v0.8
	 */
	PositionType _positionType;

	/** is the emitter paused */
	bool _paused;

	/** is sourcePosition compatible */
	bool _sourcePositionCompatible;

	bool _visible = true;
	
// 
// 	double _degress = 0.0;
// 	Vec2 _pointer = Vec2::ZERO;
// 	bool _flipX = false, _flipY = false;

	Rect  _dest_rect;

public:
//	void setOriginSize(const Size &size) { _origin = size; }
	void setContentSize(const Size &size) { _dest_rect = size; }
	void setPosition(const Vec2& position)
	{
		_dest_rect.origin = position;
	}
	Vec2 getPosition() { return _dest_rect.origin; }
	// static vector<ParticleSystem*> __allInstances;
	void draw();

	std::shared_ptr<LTexture> _texture;
private:
	ParticleSystem(const ParticleSystem &) = delete;
	ParticleSystem &operator =(const ParticleSystem &) = delete;
};

// end of _2d group
/// @}


const unsigned char __firePngData[] = {
0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52,
0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x08, 0x06, 0x00, 0x00, 0x00, 0x73, 0x7A, 0x7A,
0xF4, 0x00, 0x00, 0x00, 0x04, 0x67, 0x41, 0x4D, 0x41, 0x00, 0x00, 0xAF, 0xC8, 0x37, 0x05, 0x8A,
0xE9, 0x00, 0x00, 0x00, 0x19, 0x74, 0x45, 0x58, 0x74, 0x53, 0x6F, 0x66, 0x74, 0x77, 0x61, 0x72,
0x65, 0x00, 0x41, 0x64, 0x6F, 0x62, 0x65, 0x20, 0x49, 0x6D, 0x61, 0x67, 0x65, 0x52, 0x65, 0x61,
0x64, 0x79, 0x71, 0xC9, 0x65, 0x3C, 0x00, 0x00, 0x02, 0x64, 0x49, 0x44, 0x41, 0x54, 0x78, 0xDA,
0xC4, 0x97, 0x89, 0x6E, 0xEB, 0x20, 0x10, 0x45, 0xBD, 0xE1, 0x2D, 0x4B, 0xFF, 0xFF, 0x37, 0x5F,
0x5F, 0x0C, 0xD8, 0xC4, 0xAE, 0x2D, 0xDD, 0xA9, 0x6E, 0xA7, 0x38, 0xC1, 0x91, 0xAA, 0x44, 0xBA,
0xCA, 0x06, 0xCC, 0x99, 0x85, 0x01, 0xE7, 0xCB, 0xB2, 0x64, 0xEF, 0x7C, 0x55, 0x2F, 0xCC, 0x69,
0x56, 0x15, 0xAB, 0x72, 0x68, 0x81, 0xE6, 0x55, 0xFE, 0xE8, 0x62, 0x79, 0x62, 0x04, 0x36, 0xA3,
0x06, 0xC0, 0x9B, 0xCA, 0x08, 0xC0, 0x7D, 0x55, 0x80, 0xA6, 0x54, 0x98, 0x67, 0x11, 0xA8, 0xA1,
0x86, 0x3E, 0x0B, 0x44, 0x41, 0x00, 0x33, 0x19, 0x1F, 0x21, 0x43, 0x9F, 0x5F, 0x02, 0x68, 0x49,
0x1D, 0x20, 0x1A, 0x82, 0x28, 0x09, 0xE0, 0x4E, 0xC6, 0x3D, 0x64, 0x57, 0x39, 0x80, 0xBA, 0xA3,
0x00, 0x1D, 0xD4, 0x93, 0x3A, 0xC0, 0x34, 0x0F, 0x00, 0x3C, 0x8C, 0x59, 0x4A, 0x99, 0x44, 0xCA,
0xA6, 0x02, 0x88, 0xC7, 0xA7, 0x55, 0x67, 0xE8, 0x44, 0x10, 0x12, 0x05, 0x0D, 0x30, 0x92, 0xE7,
0x52, 0x33, 0x32, 0x26, 0xC3, 0x38, 0xF7, 0x0C, 0xA0, 0x06, 0x40, 0x0F, 0xC3, 0xD7, 0x55, 0x17,
0x05, 0xD1, 0x92, 0x77, 0x02, 0x20, 0x85, 0xB7, 0x19, 0x18, 0x28, 0x4D, 0x05, 0x19, 0x9F, 0xA1,
0xF1, 0x08, 0xC0, 0x05, 0x10, 0x57, 0x7C, 0x4F, 0x01, 0x10, 0xEF, 0xC5, 0xF8, 0xAC, 0x76, 0xC8,
0x2E, 0x80, 0x14, 0x99, 0xE4, 0xFE, 0x44, 0x51, 0xB8, 0x52, 0x14, 0x3A, 0x32, 0x22, 0x00, 0x13,
0x85, 0xBF, 0x52, 0xC6, 0x05, 0x8E, 0xE5, 0x63, 0x00, 0x86, 0xB6, 0x9C, 0x86, 0x38, 0xAB, 0x54,
0x74, 0x18, 0x5B, 0x50, 0x58, 0x6D, 0xC4, 0xF3, 0x89, 0x6A, 0xC3, 0x61, 0x8E, 0xD9, 0x03, 0xA8,
0x08, 0xA0, 0x55, 0xBB, 0x40, 0x40, 0x3E, 0x00, 0xD2, 0x53, 0x47, 0x94, 0x0E, 0x38, 0xD0, 0x7A,
0x73, 0x64, 0x57, 0xF0, 0x16, 0xFE, 0x95, 0x82, 0x86, 0x1A, 0x4C, 0x4D, 0xE9, 0x68, 0xD5, 0xAE,
0xB8, 0x00, 0xE2, 0x8C, 0xDF, 0x4B, 0xE4, 0xD7, 0xC1, 0xB3, 0x4C, 0x75, 0xC2, 0x36, 0xD2, 0x3F,
0x2A, 0x7C, 0xF7, 0x0C, 0x50, 0x60, 0xB1, 0x4A, 0x81, 0x18, 0x88, 0xD3, 0x22, 0x75, 0xD1, 0x63,
0x5C, 0x80, 0xF7, 0x19, 0x15, 0xA2, 0xA5, 0xB9, 0xB5, 0x5A, 0xB7, 0xA4, 0x34, 0x7D, 0x03, 0x48,
0x5F, 0x17, 0x90, 0x52, 0x01, 0x19, 0x95, 0x9E, 0x1E, 0xD1, 0x30, 0x30, 0x9A, 0x21, 0xD7, 0x0D,
0x81, 0xB3, 0xC1, 0x92, 0x0C, 0xE7, 0xD4, 0x1B, 0xBE, 0x49, 0xF2, 0x04, 0x15, 0x2A, 0x52, 0x06,
0x69, 0x31, 0xCA, 0xB3, 0x22, 0x71, 0xBD, 0x1F, 0x00, 0x4B, 0x82, 0x66, 0xB5, 0xA7, 0x37, 0xCF,
0x6F, 0x78, 0x0F, 0xF8, 0x5D, 0xC6, 0xA4, 0xAC, 0xF7, 0x23, 0x05, 0x6C, 0xE4, 0x4E, 0xE2, 0xE3,
0x95, 0xB7, 0xD3, 0x40, 0xF3, 0xA5, 0x06, 0x1C, 0xFE, 0x1F, 0x09, 0x2A, 0xA8, 0xF5, 0xE6, 0x3D,
0x00, 0xDD, 0xAD, 0x02, 0x2D, 0xC4, 0x4D, 0x66, 0xA0, 0x6A, 0x1F, 0xD5, 0x2E, 0xF8, 0x8F, 0xFF,
0x2D, 0xC6, 0x4F, 0x04, 0x1E, 0x14, 0xD0, 0xAC, 0x01, 0x3C, 0xAA, 0x5C, 0x1F, 0xA9, 0x2E, 0x72,
0xBA, 0x49, 0xB5, 0xC7, 0xFA, 0xC0, 0x27, 0xD2, 0x62, 0x69, 0xAE, 0xA7, 0xC8, 0x04, 0xEA, 0x0F,
0xBF, 0x1A, 0x51, 0x50, 0x61, 0x16, 0x8F, 0x1B, 0xD5, 0x5E, 0x03, 0x75, 0x35, 0xDD, 0x09, 0x6F,
0x88, 0xC4, 0x0D, 0x73, 0x07, 0x82, 0x61, 0x88, 0xE8, 0x59, 0x30, 0x45, 0x8E, 0xD4, 0x7A, 0xA7,
0xBD, 0xDA, 0x07, 0x67, 0x81, 0x40, 0x30, 0x88, 0x55, 0xF5, 0x11, 0x05, 0xF0, 0x58, 0x94, 0x9B,
0x48, 0xEC, 0x60, 0xF1, 0x09, 0xC7, 0xF1, 0x66, 0xFC, 0xDF, 0x0E, 0x84, 0x7F, 0x74, 0x1C, 0x8F,
0x58, 0x44, 0x77, 0xAC, 0x59, 0xB5, 0xD7, 0x67, 0x00, 0x12, 0x85, 0x4F, 0x2A, 0x4E, 0x17, 0xBB,
0x1F, 0xC6, 0x00, 0xB8, 0x99, 0xB0, 0xE7, 0x23, 0x9D, 0xF7, 0xCF, 0x6E, 0x44, 0x83, 0x4A, 0x45,
0x32, 0x40, 0x86, 0x81, 0x7C, 0x8D, 0xBA, 0xAB, 0x1C, 0xA7, 0xDE, 0x09, 0x87, 0x48, 0x21, 0x26,
0x5F, 0x4A, 0xAD, 0xBA, 0x6E, 0x4F, 0xCA, 0xFB, 0x23, 0xB7, 0x62, 0xF7, 0xCA, 0xAD, 0x58, 0x22,
0xC1, 0x00, 0x47, 0x9F, 0x0B, 0x7C, 0xCA, 0x73, 0xC1, 0xDB, 0x9F, 0x8C, 0xF2, 0x17, 0x1E, 0x4E,
0xDF, 0xF2, 0x6C, 0xF8, 0x67, 0xAF, 0x22, 0x7B, 0xF3, 0xEB, 0x4B, 0x80, 0x01, 0x00, 0xB8, 0x21,
0x72, 0x89, 0x08, 0x10, 0x07, 0x7D, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42,
0x60, 0x82
};

 


#endif //__CCPARTICLE_SYSTEM_H__


