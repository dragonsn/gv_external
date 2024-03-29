// Core

#define GP_WITH_OPENAL GV_WITH_OPENAL
#include "Base.h"
#include "Platform.h"
#include "Game.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Touch.h"
#include "Gesture.h"
#include "Gamepad.h"
#include "FileSystem.h"
#include "Bundle.h"
#include "MathUtil.h"

#if GP_WITH_LOGGER
	#include "Logger.h"
#endif

// Math
#include "Rectangle.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Matrix.h"
#include "Transform.h"
#include "Ray.h"
#include "Plane.h"
#include "Frustum.h"
#include "BoundingSphere.h"
#include "BoundingBox.h"
#include "Curve.h"

// Graphics
#include "Texture.h"
#include "Image.h"
#include "Mesh.h"
#include "MeshPart.h"
#include "Effect.h"
#include "Material.h"
#include "RenderState.h"
#include "VertexFormat.h"
#include "VertexAttributeBinding.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "Scene.h"
#include "Node.h"
#include "Joint.h"
#include "Font.h"
#include "SpriteBatch.h"
#include "ParticleEmitter.h"
#include "FrameBuffer.h"
#include "RenderTarget.h"
#include "DepthStencilTarget.h"
#include "ScreenDisplayer.h"
#include "HeightField.h"
#include "Terrain.h"

// Audio
#if GV_WITH_OPENAL
#include "AudioController.h"
#include "AudioListener.h"
#include "AudioBuffer.h"
#include "AudioSource.h"
#endif

// Animation
#include "AnimationController.h"
#include "AnimationTarget.h"
#include "AnimationValue.h"
#include "Animation.h"
#include "AnimationClip.h"

// Physics
#if GP_WITH_BULLET
#include "PhysicsController.h"
#include "PhysicsConstraint.h"
#include "PhysicsFixedConstraint.h"
#include "PhysicsGenericConstraint.h"
#include "PhysicsHingeConstraint.h"
#include "PhysicsSocketConstraint.h"
#include "PhysicsSpringConstraint.h"
#include "PhysicsCollisionObject.h"
#include "PhysicsCollisionShape.h"
#include "PhysicsRigidBody.h"
#include "PhysicsGhostObject.h"
#include "PhysicsCharacter.h"
#include "PhysicsVehicle.h"
#include "PhysicsVehicleWheel.h"
#endif

// UI
#include "Theme.h"
#include "Control.h"
#include "Container.h"
#include "Form.h"
#include "Label.h"
#include "Button.h"
#include "CheckBox.h"
#include "TextBox.h"
#include "RadioButton.h"
#include "Slider.h"
#include "ImageControl.h"
#include "Joystick.h"
#include "Layout.h"
#include "AbsoluteLayout.h"
#include "VerticalLayout.h"
#include "FlowLayout.h"
