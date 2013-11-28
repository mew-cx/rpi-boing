// tritess.cpp

#include <vector>
#include <iostream>
#include <algorithm>

#include <math.h>
#include <stddef.h>

/////////////////////////////////////////////////////////////////////////////

const double RADIUS1 = 3.7;
const double RADIUS2 = 2.2;

//const double PHI = (1+sqrt(5))/2;

#if 0

$fn=45;
phi=(1+sqrt(5))/2;
r1=2.2;

union()
{
    color([0.2,0.2,0.2,1])
    {
        sphere(r=3.7);
    }

    color([0.74,0.06,0.28,1])
    {
        translate([ 0, 1, phi]) sphere(r1);
        translate([ 0, 1,-phi]) sphere(r1);
        translate([ 0,-1, phi]) sphere(r1);
        translate([ 0,-1,-phi]) sphere(r1);

        translate([ 1, phi, 0]) sphere(r1);
        translate([ 1,-phi, 0]) sphere(r1);
        translate([-1, phi, 0]) sphere(r1);
        translate([-1,-phi, 0]) sphere(r1);

        translate([ phi, 0, 1]) sphere(r1);
        translate([-phi, 0, 1]) sphere(r1);
        translate([ phi, 0,-1]) sphere(r1);
        translate([-phi, 0,-1]) sphere(r1);
    }
}

#endif

/////////////////////////////////////////////////////////////////////////////

class Vec3
{
public:
    Vec3() : _x(0), _y(0), _z(0) {}
    Vec3( double x, double y, double z ) : _x(x), _y(y), _z(z) {}
    ~Vec3() {}

    double x() const { return _x; }
    double y() const { return _y; }
    double z() const { return _z; }

    const Vec3 operator+( const Vec3& rhs ) const;
    const Vec3 operator-( const Vec3& rhs ) const;
    const Vec3 operator*( const double s ) const;

    void normalize();

    friend std::ostream& operator<<( std::ostream& os, const Vec3& v );
    static void print( const Vec3& v );

private:
    double _x;
    double _y;
    double _z;
};


const Vec3 Vec3::operator+( const Vec3& rhs ) const
{
    return Vec3( _x+rhs._x, _y+rhs._y, _z+rhs._z );
}

const Vec3 Vec3::operator-( const Vec3& rhs ) const
{
    return Vec3( _x-rhs._x, _y-rhs._y, _z-rhs._z );
}

const Vec3 Vec3::operator*( const double s ) const
{
    return Vec3( _x*s, _y*s, _z*s );
}

void Vec3::normalize()
{
    const double len( sqrt(_x*_x + _y*_y + _z*_z) );
    _x /= len;
    _y /= len;
    _z /= len;
}

/*friend*/ std::ostream& operator<<( std::ostream& os, const Vec3& v )
{
    os << v._x << " " << v._y << " " << v._z << " ";
}

/*static*/ void Vec3::print( const Vec3& v )
{
    std::cout << v;
}


/////////////////////////////////////////////////////////////////////////////

class Tri
{
public:
    Tri( Vec3 a, Vec3 b, Vec3 c ) : _a(a), _b(b), _c(c) {}
    ~Tri() {}

    const Vec3& a() const { return _a; }
    const Vec3& b() const { return _b; }
    const Vec3& c() const { return _c; }

private:
    Vec3 _a;
    Vec3 _b;
    Vec3 _c;

private:        // disallowed
    Tri();
};


/////////////////////////////////////////////////////////////////////////////

class TriList
{
public:
    TriList() : _origin(Vec3()), _radius(1) {}
    ~TriList() {}

    void setOrigin( const Vec3& origin ) { _origin = origin; }
    void setRadius( double radius ) { _radius = radius; }
    const Vec3 spherize( const Vec3& v ) const;

    void subdivide( const Tri& t, size_t level );

    void printOSG() const;

private:
    Vec3 _origin;
    double _radius;

    typedef std::vector< Vec3 > Vertices;
    //typedef std::vector< unsigned int > Indices;

    Vertices _vertices;
    //Indices _indices;
};


void TriList::subdivide( const Tri& t, size_t level )
{
    const Vec3& a( t.a() );
    const Vec3& b( t.b() );
    const Vec3& c( t.c() );

    if( level > 0 )
    {
        --level;

        Vec3 d( spherize((a + b) * 0.5) );
        Vec3 e( spherize((b + c) * 0.5) );
        Vec3 f( spherize((c + a) * 0.5) );

        subdivide( Tri( a, d, f ), level );
        subdivide( Tri( b, e, d ), level );
        subdivide( Tri( c, f, e ), level );
        subdivide( Tri( e, f, d ), level );
    }
    else
    {
        _vertices.push_back( a );
        _vertices.push_back( b );
        _vertices.push_back( c );
    }
}

const Vec3 TriList::spherize( const Vec3& v ) const
{
    Vec3 z( v - _origin );
    z.normalize();
    return z * _radius + _origin;
}


void TriList::printOSG() const
{
    std::cout << "Geode {" << std::endl;
    std::cout << "  Geometry {" << std::endl;
    std::cout << "    PrimitiveSets 1 { DrawArrays TRIANGLES 0 " << _vertices.size() << " }" << std::endl;
    std::cout << "    VertexArray Vec3Array " << _vertices.size() << std::endl;
    std::cout << "    {" << std::endl;
    std::for_each( _vertices.begin(), _vertices.end(), Vec3::print );
    std::cout << "\n    }" << std::endl;
    std::cout << "  }" << std::endl;
    std::cout << "}" << std::endl;
}


/////////////////////////////////////////////////////////////////////////////

int main( int argc, char* argv[] )
{
    TriList tl;

    {
        tl.setOrigin( Vec3(0,0,0) );
        tl.setRadius( 1 );
        const Vec3 a( tl.spherize( Vec3(1,0,0) ) );
        const Vec3 b( tl.spherize( Vec3(0,1,0) ) );
        const Vec3 c( tl.spherize( Vec3(0,0,1) ) );
        tl.subdivide( Tri(a,b,c), 6 );
    }

    tl.printOSG();
}


// vim: set sw=4 ts=8 et ic ai ru:
