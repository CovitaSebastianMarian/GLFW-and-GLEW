#pragma once



class Point {
public:
    float x = 0.0F, y = 0.0F, z = 0.0F;
    Point() noexcept = default;
    Point(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    Point(glm::vec3 p) : x(p.x), y(p.y), z(p.z) {}
    bool operator <= (const Point& p) const {
        return (x == p.x ? y == p.y ? z <= p.z : y <= p.y : x <= p.x);
    }
};
class Segment {
public:
    Point p1, p2;
    Segment() noexcept = default;
    Segment(Point _p1, Point _p2) : p1(_p1), p2(_p2) {}
};


constexpr float sign_2d(const Point& p1, const Point& p2, const Point& p3) {
    return ((p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y));
}
constexpr float sign_3d(const Point& p1, const Point& p2, const Point& p3) {
    float acx = p1.x - p3.x;
    float bcy = p2.y - p3.y;
    float acy = p1.y - p3.y;
    float bcz = p2.z - p3.z;
    float bcx = p2.x - p3.x;
    float acz = p1.z - p3.z;
    return (acx * bcy + acy * bcz + bcx * acz - bcy * acz - acx * bcz - bcx * acy);
}

constexpr bool segment_intersection_2d(const Segment& s1, const Segment& s2) {
    float a = sign_2d(s2.p1, s1.p1, s1.p2);
    float b = sign_2d(s2.p2, s1.p1, s1.p2);
    if (a == 0 && b == 0) {
        if (s2.p1 <= s1.p1 && s1.p1 <= s2.p2) return true;
        if (s1.p1 <= s2.p1 && s2.p1 <= s1.p2) return true;
        return false;
    }
    if (a * b > 0) return false;

    float c = sign_2d(s1.p1, s2.p1, s2.p2);
    float d = sign_2d(s1.p2, s2.p1, s2.p2);

    if (c * d > 0) return false;
    return true;
}
constexpr bool segment_intersection_3d(const Segment& s1, const Segment& s2) {
    float a = sign_3d(s2.p1, s1.p1, s1.p2);
    float b = sign_3d(s2.p2, s1.p1, s1.p2);
    if (a == 0 && b == 0) {
        if (s2.p1 <= s1.p1 && s1.p1 <= s2.p2) return true;
        if (s1.p1 <= s2.p1 && s2.p1 <= s1.p2) return true;
        return false;
    }
    if (a * b > 0) return false;

    float c = sign_3d(s1.p1, s2.p1, s2.p2);
    float d = sign_3d(s1.p2, s2.p1, s2.p2);

    if (c * d > 0) return false;
    return true;
}














void InitGame(double w, double h) {










	



}



void Game(double w, double h, float x, float y) {


	







}





