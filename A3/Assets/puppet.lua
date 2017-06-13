-- puppet.lua
-- A simplified puppet without posable joints, but that
-- looks roughly humanoid.

rootnode = gr.node('root')


red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)

-- whole model translation helper
root_translation_helper = gr.node('root_translation_helper')
rootnode:add_child(root_translation_helper)

-- whole model rotation helper
root_rotation_helper = gr.node('root_rotation_helper')
root_translation_helper:add_child(root_rotation_helper)

root_helper = gr.node('root_helper')
root_translation_helper:add_child(root_helper)
root_helper:rotate('y', -20.0)
root_helper:scale( 0.25, 0.25, 0.25 )
root_helper:translate(0.0, 0.0, -1.0)

-- torso
torso = gr.mesh('cube', 'torso')
root_helper:add_child(torso)
torso:set_material(white)
torso:scale(0.5,1.0,0.5);

-- neck up/down rotation helper
neck_ud_rotate_helper = gr.joint('neck_ud_rotate_helper', {0, 0, 0}, {0, 0 ,0})
torso:add_child(neck_ud_rotate_helper)
neck_ud_rotate_helper:scale(1.0/0.5, 1.0, 1.0/0.5)
neck_ud_rotate_helper:translate(0, 0.5, 0)

-- neck up/down rotation joint
neck_ud_rotate_joint = gr.joint('neck_ud_rotate_joint', {-45, 0, 45}, {0, 0, 0})
neck_ud_rotate_helper:add_child(neck_ud_rotate_joint)

-- neck
neck = gr.mesh('sphere', 'neck')
neck_ud_rotate_joint:add_child(neck)
neck:scale(0.15, 0.3, 0.15)
neck:translate(0.0, 0.1, 0.0)
neck:set_material(blue)

-- head rotation helper
head_rotate_helper = gr.joint('head_rotate_helper', {0, 0, 0}, {0, 0, 0})
neck:add_child(head_rotate_helper)
head_rotate_helper:scale(1.0/0.15, 1.0/0.3, 1.0/0.15)
head_rotate_helper:translate(0, 0.15, 0)

-- head left/right rotation joint
head_lr_rotate_joint = gr.joint('head_lr_rotate_joint', {0, 0, 0}, {-90, 0, 90})
head_rotate_helper:add_child(head_lr_rotate_joint)

-- head up/down rotation joint
head_ud_rotate_joint = gr.joint('head_ud_rotate_joint', {-45, 0, 45}, {0, 0, 0})
head_lr_rotate_joint:add_child(head_ud_rotate_joint)

-- head
head = gr.mesh('cube', 'head')
head_ud_rotate_joint:add_child(head)
head:scale(0.4, 0.4, 0.4)
head:translate(0.0, 0.15, 0.0)
head:set_material(red)


ears = gr.mesh('sphere', 'ears')
head:add_child(ears)
ears:scale(1.2, 0.08, 0.08)
ears:set_material(red)
ears:set_material(blue)

leftEye = gr.mesh('cube', 'leftEye')
head:add_child(leftEye)
leftEye:scale(0.2, 0.1, 0.1)
leftEye:translate(-0.2, 0.2, 0.5)
leftEye:set_material(blue)

rightEye = gr.mesh('cube', 'rightEye')
head:add_child(rightEye)
rightEye:scale(0.2, 0.1, 0.1)
rightEye:translate(0.2, 0.2, 0.5)
rightEye:set_material(blue)








leftShoulder = gr.mesh('sphere', 'leftShoulder')
torso:add_child(leftShoulder)
leftShoulder:scale(1/0.5,1.0,1/0.5);
leftShoulder:scale(0.2, 0.2, 0.2)
leftShoulder:translate(-0.4, 0.35, 0.0)
leftShoulder:set_material(blue)

leftArm = gr.mesh('cube', 'leftArm')
torso:add_child(leftArm)
leftArm:scale(1/0.5, 1.0, 1/0.5);
leftArm:scale(0.4, 0.1, 0.1)
leftArm:rotate('z', 50);
leftArm:translate(-0.8, 0.0, 0.0)
leftArm:set_material(red)

rightShoulder = gr.mesh('sphere', 'rightShoulder')
torso:add_child(rightShoulder)
rightShoulder:scale(1/0.5,1.0,1/0.5);
rightShoulder:scale(0.2, 0.2, 0.2)
rightShoulder:translate(0.4, 0.35, 0.0)
rightShoulder:set_material(blue)

rightArm = gr.mesh('cube', 'rightArm')
torso:add_child(rightArm)
rightArm:scale(1/0.5,1.0,1/0.5);
rightArm:scale(0.4, 0.1, 0.1)
rightArm:rotate('z', -50);
rightArm:translate(0.8, 0.0, 0.0)
rightArm:set_material(red)

leftHip = gr.mesh('sphere', 'leftHip')
torso:add_child(leftHip)
leftHip:scale(1/0.5,1.0,1/0.5);
leftHip:scale(0.21, 0.21, 0.21)
leftHip:translate(-0.38, -0.5, 0.0)
leftHip:set_material(blue)

rightHip = gr.mesh('sphere', 'rightHip')
torso:add_child(rightHip)
rightHip:scale(1/0.5,1.0,1/0.5);
rightHip:scale(0.21, 0.21, 0.21)
rightHip:translate(0.38, -0.5, 0.0)
rightHip:set_material(blue)

leftLeg = gr.mesh('cube', 'leftLeg')
leftHip:add_child(leftLeg)
leftLeg:scale(0.5,4,0.5)
leftLeg:translate(0,-2.8,0)
leftLeg:set_material(red)

rightLeg = gr.mesh('cube', 'rightLeg')
rightHip:add_child(rightLeg)
rightLeg:scale(0.5,4,0.5)
rightLeg:translate(0,-2.8,0)
rightLeg:set_material(red)


return rootnode
