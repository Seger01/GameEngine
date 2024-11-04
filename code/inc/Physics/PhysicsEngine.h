#include "GameObject.h"

#include <vector>
class PhysicsEngine {

public:
    void updateReferences(std::vector<GameObject*>&);
    void update();

    void setSubStep(int);
    float getSubStep() const;
    void setStep(float);
    float getStep() const;

    void executeCollisionScripts(std::vector<int>);

private:
    std::vector<GameObject> mGameObjects;

    float step;
    int subStep;
};
