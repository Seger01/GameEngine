#include "MixerFacade.h"

int MixerFacade::findAvailableChannel() {
    for (int i = 0; i < mChannelCount; i++) {
        if (!Mix_Playing(i)) {
            mLastUsedChannel = i;
            return i;
        }
    }

    int nextChannel = mLastUsedChannel + 1;
    nextChannel = nextChannel % mChannelCount;
    mLastUsedChannel = nextChannel;
    return nextChannel;
}