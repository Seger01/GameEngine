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

        AudioManager& audioManager = EngineBravo::getInstance().getAudioManager();
        AudioSource* step = mGameObject->getComponentsWithTag<AudioSource>("step")[0];
        if ((input.GetKey(Key::Key_W) || input.GetKey(Key::Key_A) || input.GetKey(Key::Key_S) ||
             input.GetKey(Key::Key_D)) &&
            !audioManager.getFacade().isPlaying(step->getFileName())) {

            audioManager.play(*(step));
        }
    }
}

std::unique_ptr<Component> AudioBehaviourScript::clone() const { return std::make_unique<AudioBehaviourScript>(*this); }