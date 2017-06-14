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
root_translation_helper:translate(0.0, 0.0, -1.0)


-- whole model rotation helper
root_rotation_helper = gr.node('root_rotation_helper')
root_translation_helper:add_child(root_rotation_helper)
root_rotation_helper:rotate('y', -20.0)


body_blue = gr.material({36 / 255.0, 80 / 255.0, 128 / 255.0}, {0.1, 0.1, 0.1}, 10)

-- torso
torso = gr.mesh('sphere', 'torso')
root_rotation_helper:add_child(torso)
torso:set_material(body_blue)
torso:scale(0.15, 0.15, 0.15)
torso:scale(0.75, 1.0, 0.6)
torso:rotate('x', 40)


belly_white = gr.material({187 / 255.0, 176 / 255.0, 155 / 255.0}, {0.1, 0.1, 0.1}, 10)

-- belly
belly = gr.mesh('sphere', 'belly')
torso:add_child(belly)
belly:set_material(belly_white)
belly:scale(1 / 0.75, 1 / 1.0, 1 / 0.6)
belly:scale(0.5, 0.7, 0.3)
belly:translate(0, 0, 0.6)


-- neck pivot
neck_pivot = gr.node('neck_pivot')
torso:add_child(neck_pivot)
neck_pivot:scale(1 / 0.75, 1 / 1.0, 1 / 0.6)
neck_pivot:translate(0, 1, 0)

-- neck joint
neck_joint = gr.joint('neck_joint', {-45, 0, 45}, {0, 0, 0})
neck_pivot:add_child(neck_joint)

-- neck
neck = gr.mesh('sphere', 'neck')
neck_joint:add_child(neck)
neck:set_material(body_blue)
neck:scale(0.3, 0.3, 0.3)
neck:translate(0, 0.15, 0)


-- head pivot
head_pivot = gr.node('head_pivot')
neck:add_child(head_pivot)
head_pivot:scale(1 / 0.3, 1 / 0.3, 1 / 0.3)
head_pivot:translate(0, 0.3, 0)

-- head left/right joint
head_lr_rotate_joint = gr.joint('head_lr_rotate_joint', {0, 0, 0}, {-45, 0, 45})
head_pivot:add_child(head_lr_rotate_joint)

-- head up/down rotation joint
head_ud_rotate_joint = gr.joint('head_ud_rotate_joint', {-45, 10, 45}, {0, 0, 0})
head_lr_rotate_joint:add_child(head_ud_rotate_joint)

-- head
head = gr.mesh('sphere', 'head')
head_ud_rotate_joint:add_child(head)
head:set_material(belly_white)
head:scale(0.65, 0.45, 0.8)
head:translate(0, 0.15, 0.5)

-- head top
head_top = gr.mesh('sphere', 'head_top')
head:add_child(head_top)
head_top:set_material(body_blue)
head_top:scale(1 / 0.65, 1 / 0.45, 1 / 0.8)
head_top:scale(0.6, 0.4, 0.75)
head_top:translate(0, 0.2, 0)


iris_pink = gr.material({252 / 255.0, 186 / 255.0, 198 / 255.0}, {0.1, 0.1, 0.1}, 10)


-- left eye ball
left_eye_ball = gr.mesh('sphere', 'left_eye_ball')
head:add_child(left_eye_ball)
left_eye_ball:set_material(white)
left_eye_ball:scale(1 / 0.65, 1 / 0.45, 1 / 0.8)
left_eye_ball:scale(0.2, 0.2, 0.2)
left_eye_ball:translate(0.55, 0.5, 0.5)

-- left eye lid
left_eye_lid = gr.mesh('sphere', 'left_eye_lid')
left_eye_ball:add_child(left_eye_lid)
left_eye_lid:set_material(body_blue)
left_eye_lid:scale(0.9, 0.9, 0.9)
left_eye_lid:translate(-0.05, 0.3, 0)


-- left iris
left_iris = gr.mesh('sphere', 'left_iris')
left_eye_ball:add_child(left_iris)
left_iris:set_material(iris_pink)
left_iris:scale(0.3, 0.45, 0.3)
left_iris:translate(0.2, 0.1, 0.75)


-- right eye ball
right_eye_ball = gr.mesh('sphere', 'right_eye_ball')
head:add_child(right_eye_ball)
right_eye_ball:set_material(white)
right_eye_ball:scale(1 / 0.65, 1 / 0.45, 1 / 0.8)
right_eye_ball:scale(0.2, 0.2, 0.2)
right_eye_ball:translate(-0.55, 0.5, 0.5)

-- right eye lid
right_eye_lid = gr.mesh('sphere', 'right_eye_lid')
right_eye_ball:add_child(right_eye_lid)
right_eye_lid:set_material(body_blue)
right_eye_lid:scale(0.9, 0.9, 0.9)
right_eye_lid:translate(0.05, 0.3, 0)


-- right iris
right_iris = gr.mesh('sphere', 'right_iris')
right_eye_ball:add_child(right_iris)
right_iris:set_material(iris_pink)
right_iris:scale(0.3, 0.45, 0.3)
right_iris:translate(-0.2, 0.1, 0.75)


tongue_pink = gr.material({241 / 255.0, 161 / 255.0, 176 / 255.0}, {0.1, 0.1, 0.1}, 10)

-- tongue
tongue = gr.mesh('sphere', 'tongue')
head:add_child(tongue)
tongue:set_material(tongue_pink)
tongue:scale(1 / 0.65, 1 / 0.45, 1 / 0.8)
tongue:scale(0.8, 0.45, 0.9)
tongue:translate(0, -0.4, 0)

-- tongue2
tongue2 = gr.mesh('sphere', 'tongue2')
tongue:add_child(tongue2)
tongue2:set_material(tongue_pink)
tongue2:scale(1 / 0.8, 1 / 0.45, 1 / 0.9)
tongue2:scale(0.1, 0.1, 0.60)
tongue2:rotate('x', -60)
tongue2:rotate('y', 40)
tongue2:translate(-0.5, 0.3, -1)

-- tongue3
tongue3 = gr.mesh('sphere', 'tongue3')
tongue:add_child(tongue3)
tongue3:set_material(tongue_pink)
tongue3:scale(1 / 0.8, 1 / 0.45, 1 / 0.9)
tongue3:scale(0.25, 0.25, 0.8)
tongue3:rotate('x', 40)
tongue3:rotate('y', 100)
tongue3:translate(-1.3, 0.2, -1.05)


-- left arm pivot
left_shoulder_pivot = gr.node('left_shoulder_pivot')
torso:add_child(left_shoulder_pivot)
left_shoulder_pivot:scale(1 / 0.75, 1 / 1.0, 1 / 0.6)
left_shoulder_pivot:rotate('y', 100)
left_shoulder_pivot:translate(0.5, 0.6, 0.3)



-- left arm joint
left_shoulder_joint = gr.joint('left_shoulder_joint', {-45, 0, 45}, {0, 0, 0})
left_shoulder_pivot:add_child(left_shoulder_joint)


-- left arm
left_upper_arm = gr.mesh('sphere', 'left_upper_arm')
left_shoulder_joint:add_child(left_upper_arm)
left_upper_arm:set_material(body_blue)
left_upper_arm:scale(0.15, 0.15, 0.5)
left_upper_arm:translate(0, 0, 0.5)

-- left elbow
left_elbow = gr.mesh('sphere', 'left_elbow')
left_upper_arm:add_child(left_elbow)
left_elbow:set_material(body_blue)
left_elbow:scale(1.0 / 0.15, 1.0 / 0.15, 1.0 / 0.5)
left_elbow:scale(0.15, 0.15, 0.15)
left_elbow:translate(0, 0, 1)

-- left elbow joint
left_elbow_joint = gr.joint('left_elbow_joint', {0, 0, 45}, {0, 0, 0})
left_elbow:add_child(left_elbow_joint)

-- left elbow white
left_elbow_white = gr.mesh('sphere', 'left_elbow_white')
left_elbow:add_child(left_elbow_white)
left_elbow_white:set_material(white)
left_elbow_white:scale(0.8, 0.8, 0.8)
left_elbow_white:translate(0, 0.25, 0)

-- left lower arm
left_lower_arm = gr.mesh('sphere', 'left_lower_arm')
left_elbow_joint:add_child(left_lower_arm)
left_lower_arm:set_material(body_blue)
left_lower_arm:scale(1 / 0.15, 1 / 0.15, 1 / 0.15)
left_lower_arm:scale(0.15, 0.15, 0.5)
left_lower_arm:translate(0, 0, 3.3)


-- left hand pivot
left_hand_pivot = gr.node('left_hand_pivot')
left_lower_arm:add_child(left_hand_pivot)
left_hand_pivot:translate(0, 0, 0.4)
left_hand_pivot:scale(1 / 0.15, 1 / 0.15, 1 / 0.5)

-- left hand joint
left_hand_joint = gr.joint('left_hand_joint', {-45, 0, 45}, {0, 0, 0})
left_hand_pivot:add_child(left_hand_joint)

-- left hand
left_hand = gr.mesh('sphere', 'left_hand')
left_hand_joint:add_child(left_hand)
left_hand:set_material(body_blue)
left_hand:scale(0.15, 0.1, 0.15)
left_hand:translate(0, 0, 0.15)

-- left hand white
left_hand_white = gr.mesh('sphere', 'left_elbow_white')
left_hand:add_child(left_hand_white)
left_hand_white:set_material(white)
left_hand_white:scale(0.8, 0.8, 0.8)
left_hand_white:translate(0, 0.25, 0)

-- left finger 1
left_finger1 = gr.mesh('sphere', 'left_finger1')
left_hand:add_child(left_finger1)
left_finger1:set_material(body_blue)
left_finger1:scale(1 / 0.15, 1 / 0.1, 1 / 0.1)
left_finger1:scale(0.1, 0.1, 0.2)
left_finger1:rotate('y', -30)
left_finger1:translate(-1, 0, 1)

-- left finger 2
left_finger2 = gr.mesh('sphere', 'left_finger2')
left_hand:add_child(left_finger2)
left_finger2:set_material(body_blue)
left_finger2:scale(1 / 0.15, 1 / 0.1, 1 / 0.1)
left_finger2:scale(0.1, 0.1, 0.2)
left_finger2:translate(0, 0, 1)

-- left finger 3
left_finger3 = gr.mesh('sphere', 'left_finger3')
left_hand:add_child(left_finger3)
left_finger3:set_material(body_blue)
left_finger3:scale(1 / 0.15, 1 / 0.1, 1 / 0.1)
left_finger3:scale(0.1, 0.1, 0.2)
left_finger3:rotate('y', 30)
left_finger3:translate(1, 0, 1)

--------------------------- end left arm

-- right arm pivot
right_shoulder_pivot = gr.node('right_shoulder_pivot')
torso:add_child(right_shoulder_pivot)
right_shoulder_pivot:scale(1 / 0.75, 1 / 1.0, 1 / 0.6)
right_shoulder_pivot:rotate('y', -20)
right_shoulder_pivot:translate(-0.7, 0.6, 0.3)



-- right arm joint
right_shoulder_joint = gr.joint('right_shoulder_joint', {0, 45, 45}, {0, 0, 0})
right_shoulder_pivot:add_child(right_shoulder_joint)


-- right arm
right_upper_arm = gr.mesh('sphere', 'right_upper_arm')
right_shoulder_joint:add_child(right_upper_arm)
right_upper_arm:set_material(body_blue)
right_upper_arm:scale(0.15, 0.15, 0.5)
right_upper_arm:translate(0, 0, 0.5)

-- right elbow
right_elbow = gr.mesh('sphere', 'right_elbow')
right_upper_arm:add_child(right_elbow)
right_elbow:set_material(body_blue)
right_elbow:scale(1.0 / 0.15, 1.0 / 0.15, 1.0 / 0.5)
right_elbow:scale(0.15, 0.15, 0.15)
right_elbow:translate(0, 0, 1)

-- right elbow joint
right_elbow_joint = gr.joint('right_elbow_joint', {-45, -10, 45}, {0, 0, 0})
right_elbow:add_child(right_elbow_joint)

-- right elbow white
right_elbow_white = gr.mesh('sphere', 'right_elbow_white')
right_elbow:add_child(right_elbow_white)
right_elbow_white:set_material(white)
right_elbow_white:scale(0.8, 0.8, 0.8)
right_elbow_white:translate(0, 0.25, 0)

-- right lower arm
right_lower_arm = gr.mesh('sphere', 'right_lower_arm')
right_elbow_joint:add_child(right_lower_arm)
right_lower_arm:set_material(body_blue)
right_lower_arm:scale(1 / 0.15, 1 / 0.15, 1 / 0.15)
right_lower_arm:scale(0.15, 0.15, 0.5)
right_lower_arm:translate(0, 0, 3.3)


-- right hand pivot
right_hand_pivot = gr.node('right_hand_pivot')
right_lower_arm:add_child(right_hand_pivot)
right_hand_pivot:translate(0, 0, 0.4)
right_hand_pivot:scale(1 / 0.15, 1 / 0.15, 1 / 0.5)

-- right hand joint
right_hand_joint = gr.joint('right_hand_joint', {-45, -40, 45}, {0, 0, 0})
right_hand_pivot:add_child(right_hand_joint)

-- right hand
right_hand = gr.mesh('sphere', 'right_hand')
right_hand_joint:add_child(right_hand)
right_hand:set_material(body_blue)
right_hand:scale(0.15, 0.1, 0.15)
right_hand:translate(0, 0, 0.15)

-- right hand white
right_hand_white = gr.mesh('sphere', 'right_elbow_white')
right_hand:add_child(right_hand_white)
right_hand_white:set_material(white)
right_hand_white:scale(0.8, 0.8, 0.8)
right_hand_white:translate(0, 0.25, 0)

-- right finger 1
right_finger1 = gr.mesh('sphere', 'right_finger1')
right_hand:add_child(right_finger1)
right_finger1:set_material(body_blue)
right_finger1:scale(1 / 0.15, 1 / 0.1, 1 / 0.1)
right_finger1:scale(0.1, 0.1, 0.2)
right_finger1:rotate('y', -30)
right_finger1:translate(-1, 0, 1)

-- right finger 2
right_finger2 = gr.mesh('sphere', 'right_finger2')
right_hand:add_child(right_finger2)
right_finger2:set_material(body_blue)
right_finger2:scale(1 / 0.15, 1 / 0.1, 1 / 0.1)
right_finger2:scale(0.1, 0.1, 0.2)
right_finger2:translate(0, 0, 1)

-- right finger 3
right_finger3 = gr.mesh('sphere', 'right_finger3')
right_hand:add_child(right_finger3)
right_finger3:set_material(body_blue)
right_finger3:scale(1 / 0.15, 1 / 0.1, 1 / 0.1)
right_finger3:scale(0.1, 0.1, 0.2)
right_finger3:rotate('y', 30)
right_finger3:translate(1, 0, 1)

------------------------------------------- end right arm


-- left leg
-- left_leg_pivot = gr.node('left_leg_pivot')
-- torso:add_child(left_leg_pivot)
-- right_shoulder_pivot:scale(1 / 0.75, 1 / 1.0, 1 / 0.6)
-- left_shoulder_pivot:rotate('y', -20)
-- right_shoulder_pivot:translate(-0.7, 0.6, 0.3)









---- torso
--torso = gr.mesh('cube', 'torso')
--root_rotation_helper:add_child(torso)
--torso:set_material(white)
--torso:scale(0.5,1.0,0.5);
--torso:scale( 0.25, 0.25, 0.25 )


---- neck up/down rotation helper
--neck_ud_rotate_helper = gr.joint('neck_ud_rotate_helper', {0, 0, 0}, {0, 0 ,0})
--torso:add_child(neck_ud_rotate_helper)
--neck_ud_rotate_helper:scale(1.0/0.5, 1.0, 1.0/0.5)
--neck_ud_rotate_helper:translate(0, 0.5, 0)
--
---- neck up/down rotation joint
--neck_ud_rotate_joint = gr.joint('neck_ud_rotate_joint', {-45, 0, 45}, {0, 0, 0})
--neck_ud_rotate_helper:add_child(neck_ud_rotate_joint)
--
---- neck
--neck = gr.mesh('sphere', 'neck')
--neck_ud_rotate_joint:add_child(neck)
--neck:scale(0.15, 0.3, 0.15)
--neck:translate(0.0, 0.1, 0.0)
--neck:set_material(blue)
--
---- head rotation helper
--head_rotate_helper = gr.joint('head_rotate_helper', {0, 0, 0}, {0, 0, 0})
--neck:add_child(head_rotate_helper)
--head_rotate_helper:scale(1.0/0.15, 1.0/0.3, 1.0/0.15)
--head_rotate_helper:translate(0, 0.15, 0)
--
---- head left/right rotation joint
--head_lr_rotate_joint = gr.joint('head_lr_rotate_joint', {0, 0, 0}, {-90, 0, 90})
--head_rotate_helper:add_child(head_lr_rotate_joint)
--
---- head up/down rotation joint
--head_ud_rotate_joint = gr.joint('head_ud_rotate_joint', {-45, 0, 45}, {0, 0, 0})
--head_lr_rotate_joint:add_child(head_ud_rotate_joint)
--
---- head
--head = gr.mesh('cube', 'head')
--head_ud_rotate_joint:add_child(head)
--head:scale(0.4, 0.4, 0.4)
--head:translate(0.0, 0.15, 0.0)
--head:set_material(red)
--
--
--ears = gr.mesh('sphere', 'ears')
--head:add_child(ears)
--ears:scale(1.2, 0.08, 0.08)
--ears:set_material(red)
--ears:set_material(blue)
--
--leftEye = gr.mesh('cube', 'leftEye')
--head:add_child(leftEye)
--leftEye:scale(0.2, 0.1, 0.1)
--leftEye:translate(-0.2, 0.2, 0.5)
--leftEye:set_material(blue)
--
--rightEye = gr.mesh('cube', 'rightEye')
--head:add_child(rightEye)
--rightEye:scale(0.2, 0.1, 0.1)
--rightEye:translate(0.2, 0.2, 0.5)
--rightEye:set_material(blue)
--
--
--
--
--
--
--
--
--leftShoulder = gr.mesh('sphere', 'leftShoulder')
--torso:add_child(leftShoulder)
--leftShoulder:scale(1/0.5,1.0,1/0.5);
--leftShoulder:scale(0.2, 0.2, 0.2)
--leftShoulder:translate(-0.4, 0.35, 0.0)
--leftShoulder:set_material(blue)
--
--leftArm = gr.mesh('cube', 'leftArm')
--torso:add_child(leftArm)
--leftArm:scale(1/0.5, 1.0, 1/0.5);
--leftArm:scale(0.4, 0.1, 0.1)
--leftArm:rotate('z', 50);
--leftArm:translate(-0.8, 0.0, 0.0)
--leftArm:set_material(red)
--
--rightShoulder = gr.mesh('sphere', 'rightShoulder')
--torso:add_child(rightShoulder)
--rightShoulder:scale(1/0.5,1.0,1/0.5);
--rightShoulder:scale(0.2, 0.2, 0.2)
--rightShoulder:translate(0.4, 0.35, 0.0)
--rightShoulder:set_material(blue)
--
--rightArm = gr.mesh('cube', 'rightArm')
--torso:add_child(rightArm)
--rightArm:scale(1/0.5,1.0,1/0.5);
--rightArm:scale(0.4, 0.1, 0.1)
--rightArm:rotate('z', -50);
--rightArm:translate(0.8, 0.0, 0.0)
--rightArm:set_material(red)
--
--leftHip = gr.mesh('sphere', 'leftHip')
--torso:add_child(leftHip)
--leftHip:scale(1/0.5,1.0,1/0.5);
--leftHip:scale(0.21, 0.21, 0.21)
--leftHip:translate(-0.38, -0.5, 0.0)
--leftHip:set_material(blue)
--
--rightHip = gr.mesh('sphere', 'rightHip')
--torso:add_child(rightHip)
--rightHip:scale(1/0.5,1.0,1/0.5);
--rightHip:scale(0.21, 0.21, 0.21)
--rightHip:translate(0.38, -0.5, 0.0)
--rightHip:set_material(blue)
--
--leftLeg = gr.mesh('cube', 'leftLeg')
--leftHip:add_child(leftLeg)
--leftLeg:scale(0.5,4,0.5)
--leftLeg:translate(0,-2.8,0)
--leftLeg:set_material(red)
--
--rightLeg = gr.mesh('cube', 'rightLeg')
--rightHip:add_child(rightLeg)
--rightLeg:scale(0.5,4,0.5)
--rightLeg:translate(0,-2.8,0)
--rightLeg:set_material(red)


return rootnode
