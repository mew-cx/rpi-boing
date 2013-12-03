// tritess.cpp

#include <vector>
#include <iostream>
//#include <algorithm>

#include <math.h>
#include <stddef.h>

/////////////////////////////////////////////////////////////////////////////

const double PHI = (1+sqrt(5))/2;       // golden ratio

const double RADIUS1 = 3.70;            // radius of berry body
const double RADIUS2 = 2.20;            // radius of berry bump

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

    double length() const;
    void normalize();

    friend std::ostream& operator<<( std::ostream& os, const Vec3& v );

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

double Vec3::length() const
{
    return sqrt( _x*_x + _y*_y + _z*_z );
}

void Vec3::normalize()
{
    const double len( length() );
    _x /= len;
    _y /= len;
    _z /= len;
}

/*friend*/ std::ostream& operator<<( std::ostream& os, const Vec3& v )
{
    return os << v._x << " " << v._y << " " << v._z;
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

class Sphere
{
public:
    Sphere() : _origin(Vec3()), _radius(1) {}
    virtual ~Sphere() {}

    void setColor( const Vec3& color ) { _color = color; }
    const Vec3& color() const { return _color; }

    void setOrigin( const Vec3& origin ) { _origin = origin; }
    const Vec3& origin( const Vec3& origin ) const { return _origin; }

    void setRadius( double radius ) { _radius = radius; }
    double radius() const { return _radius; }

    void build( const Tri& t, size_t level );

    friend std::ostream& operator<<( std::ostream& os, const Sphere& s );

protected:
    const Vec3 spherize( const Vec3& v ) const;
    const Vec3 normal( const Vec3& v ) const;

    void subdivide( const Tri& t, size_t level );

    bool isVisible( const Tri& t ) const;
    virtual bool isVisible( const Vec3& v ) const = 0;

    void add( const Tri& t );
    void add( const Vec3& v );

private:
    Vec3 _color;
    Vec3 _origin;
    double _radius;

    typedef std::vector< Vec3 > Vertices;
    Vertices _vertices;

    //typedef std::vector< unsigned int > Indices;
    //Indices _indices;
};


void Sphere::build( const Tri& t, size_t level )
{
    const Vec3 a( spherize( t.a() ) );
    const Vec3 b( spherize( t.b() ) );
    const Vec3 c( spherize( t.c() ) );

    subdivide( Tri(a,b,c), level );
}

void Sphere::subdivide( const Tri& t, size_t level )
{
    if( level > 0 )
    {
        const Vec3& a( t.a() );
        const Vec3& b( t.b() );
        const Vec3& c( t.c() );
        const Vec3  d( spherize((a + b) * 0.5) );
        const Vec3  e( spherize((b + c) * 0.5) );
        const Vec3  f( spherize((c + a) * 0.5) );

        --level;
        subdivide( Tri( a, d, f ), level );
        subdivide( Tri( b, e, d ), level );
        subdivide( Tri( c, f, e ), level );
        subdivide( Tri( e, f, d ), level );
    }
    else if( isVisible( t ) )
    {
        add( t );
    }
}

bool Sphere::isVisible( const Tri& t ) const
{
    return isVisible( t.a() ) || isVisible( t.b() ) || isVisible( t.c() );
}

void Sphere::add( const Tri& t )
{
    add( t.a() );
    add( t.b() );
    add( t.c() );
}

void Sphere::add( const Vec3& v )
{
    // TODO find if vert already exists
    _vertices.push_back( v );
    // TODO add index to element list
}

const Vec3 Sphere::spherize( const Vec3& v ) const
{
    Vec3 z( v - _origin );
    z.normalize();
    return z * _radius + _origin;
}

const Vec3 Sphere::normal( const Vec3& v ) const
{
    Vec3 z( v - _origin );
    z.normalize();
    return z;
}

/*friend*/ std::ostream& operator<<( std::ostream& os, const Sphere& s )
{
    const size_t numVerts( s._vertices.size() );
    os << "Geode {" << std::endl;
    os << "  Geometry {" << std::endl;
    os << "    PrimitiveSets 1 { DrawArrays TRIANGLES 0 " << numVerts << " }" << std::endl;

    os << "    VertexArray Vec3Array " << numVerts << " {" << std::endl;
    for( Sphere::Vertices::const_iterator i(s._vertices.begin()); i != s._vertices.end(); ++i )
    {
        os << *i << " ";
    }
    os << "}" << std::endl;

    os << "    NormalBinding PER_VERTEX" << std::endl;
    os << "    NormalArray Vec3Array " << numVerts << " {" << std::endl;
    for( Sphere::Vertices::const_iterator i(s._vertices.begin()); i != s._vertices.end(); ++i )
    {
        os << s.normal(*i) << " ";
    }
    os << "}" << std::endl;

    os << "    ColorBinding OVERALL" << std::endl;
    os << "    ColorArray Vec4Array " << 1 << " {";
    os << s._color << " " << 1.0;
    os << "}" << std::endl;

    os << "  }" << std::endl;
    os << "}" << std::endl;
    return os;
}

/////////////////////////////////////////////////////////////////////////////

class Bump : public Sphere
{
public:
    Bump()
    {
        setColor( Vec3( 0.7, 0.2, 0.5 ) );
        setRadius( RADIUS2 );
    }
    ~Bump() {}

protected:
    bool isVisible( const Vec3& v ) const
    {
        return( v.length() >= RADIUS1 );
    }
};

/////////////////////////////////////////////////////////////////////////////

class Body : public Sphere
{
public:
    Body()
    {
        setColor( Vec3( 0.2, 0.2, 0.2 ) );
        setRadius( RADIUS1 );
    }
    ~Body() {}

protected:
    bool isVisible( const Vec3& v ) const
    {
        const Vec3 a( v - Vec3( PHI, 0, 1 ) );
        if( a.length() < RADIUS2 )  return false;

        const Vec3 b( v - Vec3( 1, PHI, 0 ) );
        if( b.length() < RADIUS2 )  return false;

        const Vec3 c( v - Vec3( 0, 1, PHI ) );
        if( c.length() < RADIUS2 )  return false;

        return true;
    }
};

/////////////////////////////////////////////////////////////////////////////

void octant()
{
    {
        Bump b1;
        const Vec3 origin( PHI, 0, 1 );
        b1.setOrigin( origin );
        const Vec3 a( origin + Vec3(1,0,-0.5) );
        const Vec3 b( origin + Vec3(0,1,0) );
        const Vec3 c( origin + Vec3(0,0,1) );
        b1.build( Tri(a,b,c), 4 );
        std::cout << b1;
    }

    {
        Bump b2;
        const Vec3 origin( 1, PHI, 0 );
        b2.setOrigin( origin );
        const Vec3 a( origin + Vec3(1,0,0) );
        const Vec3 b( origin + Vec3(-0.5,1,0) );
        const Vec3 c( origin + Vec3(0,0,1) );
        b2.build( Tri(a,b,c), 4 );
        std::cout << b2;
    }

    {
        Bump b3;
        const Vec3 origin( 0, 1, PHI );
        b3.setOrigin( origin );
        const Vec3 a( origin + Vec3(1,0,0) );
        const Vec3 b( origin + Vec3(0,1,0) );
        const Vec3 c( origin + Vec3(0,-0.5,1) );
        b3.build( Tri(a,b,c), 4 );
        std::cout << b3;
    }

    {
        Body s1;
        const Vec3 origin(0,0,0);
        s1.setOrigin( origin );
        const Vec3 a( origin + Vec3(1,0,0) );
        const Vec3 b( origin + Vec3(0,1,0) );
        const Vec3 c( origin + Vec3(0,0,1) );
        s1.build( Tri(a,b,c), 5 );
        std::cout << s1;
    }
}

/////////////////////////////////////////////////////////////////////////////

int main( int argc, char* argv[] )
{
    octant();
}

// vim: set sw=4 ts=8 et ic ai ru:
