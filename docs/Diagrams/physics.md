@startuml

class PhysicsEngine {

    + updateReferences(vector<GameObject*>&) : void
    + update() : void
    + setSubStep : void
    + getSubStep() : int

    - GameObjects : vector<GameObject*>
    - step : float
    - subStep : int

}

class BodyProxy {

    BodyProxy(GameObject&)
}

class World {

    + World(b2Vec2 Gravity)

    + executeWorldStep(float step, int subStep) : void
    + reset() : void

    + createBody(BodyProxy& bodyProxy) : b2BoxId
    + updateBody(b2BodyId bodyID, BodyProxy&) : void
    + applyForce(b2BodyId bodyIS, point force): void

    + setPosition(b2BodyId bodyID, point pos) : void
    + setGravity(point gravity) : void

    + getPosition(b2BodyId bodyID) : point
    + getGravity() : point

    - mWorldID : b2worldId
}

class Colliders {

}

class BoxCollider {

    - mWidth : float
    - mHeight : float

    - mBodyId : b2BodyId
    - mBodyDef : b2BodyDef
}

class CircleCollider {

    - mRadius : float

    - mBodyId : b2BodyId
    - mBodyDef : b2BodyDef
}

class RigidBody(b2DynamicBody) {

    + RigidBody(flags, properties)
    + applyForce(point) : void

    ' SETTERS AND GETTERS FOR MEMBERS
    - mIsUpdated : bool
    - mHasGravity : bool true/false
    - mIsMoveableByForce : bool false
    - mCanRotate : bool true/false

    - mDensity : float 0
    - mFriction : float 0
    - mRestitution : float 0

    - mMass : float 0
    - mGravityScale : float 0

    - mBodyId : int

    - mForcesBuffer : vector<point>
}
class RigidBody(B2StaticBody) {

    + RigidBody(flags, properties)
    + applyForce(point) : void

    ' SETTERS AND GETTERS FOR MEMBERS
    - mIsUpdated : bool
    - mHasGravity : bool false
    - mIsMoveable : bool true/false
    - mIsMoveableByForce : bool false
    - mCanRotate : bool true/false

    - mDensity : float 0
    - mFriction : float 0
    - mRestitution : float 0

    - mMass : float 0
    - mGravityScale : float 0

    - mBodyId : int

    - mForcesBuffer : vector<point>
}

class ContactListener {
    
    + setContactEvents(b2BodyId bodyA, b2BodyId, bodyB) : void
}

class b2ContactListener {

}

' CONNECTIONS

PhysicsEngine *-- "1" World
BodyProxy -left-> PhysicsEngine
ContactListener -right-> PhysicsEngine

b2ContactListener -right-> ContactListener

@enduml

