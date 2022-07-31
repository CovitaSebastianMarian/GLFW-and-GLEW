#pragma once
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

class Physics {
private:

	pair<float, float> RotateX(float angle, float y, float z) {
		float yy, zz;
		yy = cos(angle) * y - sin(angle) * z;
		zz = sin(angle) * y + cos(angle) * z;
		return { yy, zz };
	}

	pair<float, float> RotateY(float angle, float x, float z) {
		float xx, zz;
		xx = cos(angle) * x + sin(angle) * z;
		zz = -sin(angle) * x + cos(angle) * z;
		return { xx, zz };
	}

	pair<float, float> RotateZ(float angle, float x, float y) {
		float xx, yy;
		xx = cos(angle) * x - sin(angle) * y;
		yy = sin(angle) * x + cos(angle) * y;
		return { xx, yy };
	}


	class Coordinates {
	public:
		float x, y, z;
	};



public:

	enum class FORM_TYPE : int {
		NONE = -1, PARALLELEPIPED = 0, SHPERE = 1
	};

	class Form {
	public:
		FORM_TYPE form_type = FORM_TYPE::NONE;
		unsigned int ID;
		float xpos, ypos, zpos;
		float xsize, ysize, zsize;
		float mass, force, gravity = 9.81;
		float xradius, yradius, zradius;
	};

	vector<Form> V_Forms;

	void InitPhysicsForm(Form& _Form) {

		switch (_Form.form_type) {
		case FORM_TYPE::NONE:
			break;
		case FORM_TYPE::PARALLELEPIPED:

			_Form.force = _Form.mass * _Form.gravity;
			break;
		case FORM_TYPE::SHPERE:
			break;
		default:
			break;
		}

		_Form.ID = V_Forms.size();
		V_Forms.push_back(_Form);
	}

	bool OverX(Form obj, Form vobj) {
		float fMaxX = obj.xpos + obj.xsize;
		float fMinX = obj.xpos - obj.xsize;
		float vfMaxX = vobj.xpos + vobj.xsize;
		float vfMinX = vobj.xpos - vobj.xsize;


		if (fMaxX <= vfMinX || fMinX >= vfMaxX) return true;
		return false;
	}

	bool OverY(Form obj, Form vobj) {
		float fMaxY = obj.ypos + obj.ysize;
		float fMinY = obj.ypos - obj.ysize;
		float vfMaxY = vobj.ypos + vobj.ysize;
		float vfMinY = vobj.ypos - vobj.ysize;

		if (fMaxY <= vfMinY || fMinY >= vfMaxY) return true;
		return false;
	}

	bool OverZ(Form obj, Form vobj) {
		float fMaxZ = obj.zpos + obj.zsize;
		float fMinZ = obj.zpos - obj.zsize;
		float vfMaxZ = vobj.zpos + vobj.zsize;
		float vfMinZ = vobj.zpos - vobj.zsize;

		if (fMaxZ <= vfMinZ || fMinZ >= vfMaxZ) return true;
		return false;
	}



	bool VerifyX(Form form, float x) {
		form.xpos += x;
		for (int i = 0; i < V_Forms.size(); ++i) {
			if (V_Forms[i].ID != form.ID) {
				float distance = abs(form.xpos - V_Forms[i].xpos);
				distance -= form.xsize;
				distance -= V_Forms[i].xsize;
				if (distance < 0) {
					if(!(OverY(form, V_Forms[i]) || OverZ(form, V_Forms[i]))) return false;
				}
			}
		}
		return true;
	}

	bool VerifyY(Form form, float y) {
		form.ypos += y;
		for (int i = 0; i < V_Forms.size(); ++i) {
			if (V_Forms[i].ID != form.ID) {
				float distance = abs(form.ypos - V_Forms[i].ypos);
				distance -= form.ysize;
				distance -= V_Forms[i].ysize;
				if (distance < 0) {
					if (!(OverX(form, V_Forms[i]) || OverZ(form, V_Forms[i]))) return false;
				}
			}
		}
		return true;
	}


	bool VerifyZ(Form form, float z) {
		form.zpos += z;
		for (int i = 0; i < V_Forms.size(); ++i) {
			if (V_Forms[i].ID != form.ID) {
				float distance = abs(form.zpos - V_Forms[i].zpos);
				distance -= form.zsize;
				distance -= V_Forms[i].zsize;
				if (distance < 0) {
					if (!(OverX(form, V_Forms[i]) || OverY(form, V_Forms[i]))) return false;
				}
			}
		}
		return true;
	}










};