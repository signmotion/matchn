#pragma once

#include "configure.h"
#include "struct.h"


namespace matchn {


class Territory;


/**
* ���������� ��� ����.
* ����� �����, ����� �������� �������� "���������� �����": �����������,
* �����������, �����, ...
*/
class LogicWorld {
public:
    /**
    * ���-�� ���������� ������ ��������� � ������ ����.
    *
    * # ������ ��������� �����������, ����� �� ������� gett'���.
    */
    const size_t  K;
    const size_t  N;
    const size_t  M;




public:
    LogicWorld(
        size_t K, size_t N, size_t M
    );




    virtual ~LogicWorld();




    const Territory*  territory() const;


    Territory*  territory();


    void territory( std::shared_ptr< Territory > );




    /**
    * @return ���������� ���������, � �������� �������� ���.
    *
    * # ������������ �� ����������� ����� 'media/element'.
    */
    size_t countAvailableElement() const;




    /**
    * @return ���������� ���������� �� ����������.
    *         ����������� �������� �� ������.
    */
    typelib::coord2Int_t visualByLogicCoord( const typelib::coord2Int_t& ) const;




    /**
    * @return ���������� ���������� �� ����������.
    *         ����������� �������� �� ������.
    */
    typelib::coord2Int_t logicByVisualCoord( const typelib::coord2Int_t& ) const;




    /**
    * @return ���������� ������ ������� � ������ ������� ���� � ��� ��������
    *         �� ������.
    */
    typelib::size2Int_t visualSpriteSize() const;




    /**
    * @return ������� ��� ������������ ��������� � ������ ������� ���� � ���
    *         �������� �� ������.
    */
    typelib::size2_t visualScale() const;




    // @todo ������� � ��������� ����� typelib.
    // @see typelib::DynamicMapContent2D
    inline typelib::size2Int_t size() const {
        return typelib::size2Int_t( N, M );
    }




    inline std::size_t volume() const {
        return N * M;
    }




    inline typelib::coord2Int_t minCoord() const {
        return -maxCoord();
    }


    inline typelib::coord2Int_t maxCoord() const {
        return static_cast< typelib::coord2Int_t >( (size() - 1) / 2 );
    }




    inline bool inside( const typelib::coord2Int_t& c ) const {
        const auto mi = minCoord();
        const auto ma = maxCoord();
        return (c.x >= mi.x) && (c.x <= ma.x)
            && (c.y >= mi.y) && (c.y <= ma.y);
    }


    inline bool inside( int x, int y ) const {
        return inside( typelib::coord2Int_t( x, y ) );
    }




    inline size_t ic( const typelib::coord2Int_t& c ) const {
        return ic( c.x, c.y );
    }


    inline size_t ic( int x, int y ) const {
        const auto max = maxCoord();
        const size_t i = (
            static_cast< std::size_t >(x + max.x)
          + static_cast< std::size_t >(y + max.y) * N
        );
        return i;
    }




    inline typelib::coord2Int_t ci( size_t i ) const {
        const size_t y = i / N;
        const size_t x = i - y * N;
        const typelib::coord2Int_t  c(
            static_cast< int >( x ) - maxCoord().x,
            static_cast< int >( y ) - maxCoord().y
        );

        assert( inside( c )
            && "����������� 2D ���������� ����� �� ��������� �������� �����." );

        return c;
    }




    static inline typelib::coord2Int_t isc( size_t cell ) {
        using namespace typelib;
        const static coord2Int_t a[ 9 ] = {
            coord2Int_t(  0,   0 ),
            coord2Int_t(  0,  +1 ),
            coord2Int_t( +1,   0 ),
            coord2Int_t(  0,  -1 ),
            coord2Int_t( -1,   0 ),
            coord2Int_t( +1,  +1 ),
            coord2Int_t( +1,  -1 ),
            coord2Int_t( -1,  -1 ),
            coord2Int_t( -1,  +1 )
        };

        return a[ cell ];
    }




private:
    /**
    * ������ � ����������.
    */
    std::weak_ptr< Territory >  mTerritory;


    /**
    * ��� ��� ���������� �������.
    */
    mutable size_t countAvailableElement_;
};


} // matchn
