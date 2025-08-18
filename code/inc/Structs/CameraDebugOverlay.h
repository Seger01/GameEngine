/**
 * @file CameraDebugOverlay.h
 * @brief CameraDebugOverlay struct
 * @ingroup Camera
 */

#ifndef CAMERADEBUGOVERLAY_H
#define CAMERADEBUGOVERLAY_H

/**
 * @struct CameraDebugOverlay
 * @brief Struct for camera debug overlay settings.
 */
struct CameraDebugOverlay
{
	bool renderCameraViewport = false;
	bool renderColliders = false;
	bool showFPS = false;
};

#endif // CAMERADEBUGOVERLAY_H
