#include "AudioBehaviourScript.h"

#include "EngineBravo.h"
#include "Input.h"
#include "SceneManager.h"

void AudioBehaviourScript::onStart() {}

void AudioBehaviourScript::onUpdate() {
    EngineBravo& engine = EngineBravo::getInstance();
    SceneManager& sceneManager = engine.getSceneManager();

    if (sceneManager.getCurrentScene()->getName() == "Level-1") {
        Input& input = Input::getInstance();
        if (input.GetKeyDown(Key::Key_P)) {
            AudioManager& audioManager = EngineBravo::getInstance().getAudioManager();
            audioManager.play(*(mGameObject->getComponentsWithTag<AudioSource>("gun")[0]));
        }
    }
}

std::unique_ptr<Component> AudioBehaviourScript::clone() const { return std::make_unique<AudioBehaviourScript>(*this); }