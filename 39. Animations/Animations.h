#pragma once

using namespace std;



enum class ANIMATION_STATE : int {
	ANIMATION_NONE = -1, ANIMATION_FALSE = 0, ANIMATION_TRUE = 1, ANIMATION_ADD = 2, ANIMATION_SUBTRACT = 3,
	ANIMATION_ROTATE_GO_TO = 4, ANIMATION_ROTATE_RETURN_TO = 5
};


class Animations {
public:
	ANIMATION_STATE animation = ANIMATION_STATE::ANIMATION_NONE;

	ANIMATION_STATE Add(float &val, float count, float to) {
		if (val + count <= to) {
			val += count;
			return ANIMATION_STATE::ANIMATION_TRUE;
		}
		else if (val < to) {
			val = to;
			return ANIMATION_STATE::ANIMATION_TRUE;
		}
		return ANIMATION_STATE::ANIMATION_FALSE;
	}
	

	ANIMATION_STATE Subtract(float &val, float count, float to) {
		if (val - count >= to) {
			val -= count;
			return ANIMATION_STATE::ANIMATION_TRUE;
		}
		else if (val > to) {
			val = to;
			return ANIMATION_STATE::ANIMATION_TRUE;
		}
		return ANIMATION_STATE::ANIMATION_FALSE;
	}


	ANIMATION_STATE AddAndReturn(float& val, float count, float go_to, float return_to) {
		if (animation == ANIMATION_STATE::ANIMATION_NONE) animation = ANIMATION_STATE::ANIMATION_ADD;
		if (animation == ANIMATION_STATE::ANIMATION_ADD) {
			if (Add(val, count, go_to) == ANIMATION_STATE::ANIMATION_FALSE) {
				animation = ANIMATION_STATE::ANIMATION_SUBTRACT;
			}
		}
		if (animation == ANIMATION_STATE::ANIMATION_SUBTRACT) {
			if (Subtract(val, count, return_to) == ANIMATION_STATE::ANIMATION_FALSE) {
				animation = ANIMATION_STATE::ANIMATION_NONE;
			}
		}
		return animation;
	}


	ANIMATION_STATE SubtractAndReturn(float& val, float count, float go_to, float return_to) {
		if (animation == ANIMATION_STATE::ANIMATION_NONE) animation = ANIMATION_STATE::ANIMATION_SUBTRACT;
		if (animation == ANIMATION_STATE::ANIMATION_SUBTRACT) {
			if (Subtract(val, count, go_to) == ANIMATION_STATE::ANIMATION_FALSE) {
				animation = ANIMATION_STATE::ANIMATION_ADD;
			}
		}
		if (animation == ANIMATION_STATE::ANIMATION_ADD) {
			if (Add(val, count, return_to) == ANIMATION_STATE::ANIMATION_FALSE) {
				animation = ANIMATION_STATE::ANIMATION_NONE;
			}
		}
		return animation;
	}


	ANIMATION_STATE Rotate(float& angle, float count, float to) {
		if (angle + count <= to) {
			angle += count;
			return ANIMATION_STATE::ANIMATION_TRUE;
		}
		else if (angle < to) {
			angle = to;
			return ANIMATION_STATE::ANIMATION_TRUE;
		}
		return ANIMATION_STATE::ANIMATION_FALSE;
	}


};