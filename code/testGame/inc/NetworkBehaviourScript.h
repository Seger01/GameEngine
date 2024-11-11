#ifndef NETWORKBEHAVIOURSCRIPT_H
#define NETWORKBEHAVIOURSCRIPT_H

#include "Components/IBehaviourScript.h"
#include "UI/Button.h"

#include <list>
#include <vector>

class NetworkBehaviourScript : public IBehaviourScript {
public:
    NetworkBehaviourScript() {}
    ~NetworkBehaviourScript() {}

    void onStart() override;

    void onUpdate() override;

private:
    Button* mServerButton{nullptr};
    Button* mClientButton{nullptr};
    Button* mHostButton{nullptr};
    Button* mSearchButton{nullptr};

    std::list<std::string> mServerAddresses;
};
#endif // NETWORKBEHAVIOURSCRIPT_H