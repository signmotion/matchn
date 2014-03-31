#pragma once

#include "configure.h"
#include "Event.h"


namespace matchn {


class World;
class ManagerSprite;

template< class E >
class Event;


/**
* ������, ������������������ � ������ (� ����) ClanLib.
*
* @see http://clanlib.org
*/
class CLIncarnate {
public:
    /**
    * ������� ������������� �� ��������.
    * ������ ������� ����� ����� ���� ������ (����� ��� ������������).
    */
    typedef std::map<
        UIDEvent,
        std::unique_ptr< CL_Sprite >
    > sprite_t;




protected:
    CLIncarnate(
        std::shared_ptr< World >,
        const typelib::coord2Int_t&  coord,
        int  rotation
    );



    virtual ~CLIncarnate();




public:
    const CL_DisplayWindow* world() const;


    CL_DisplayWindow* world();




    inline const ManagerSprite* managerSprite() const {
        return mManagerSprite.get();
    }


    inline ManagerSprite* managerSprite() {
        return mManagerSprite.get();
    }




    inline sprite_t const* sprite() const {
        return mSprite.get();
    }




    /**
    * @return ������ ��� ��������� �������.
    *         nullptr, ���� ������ ��� ������� �� ������.
    */
    inline CL_Sprite* sprite( enum UIDEvent e ) {
        auto ftr = mSprite->find( e );
        return (ftr == mSprite->end()) ? nullptr : ftr->second.get();
    }



    /**
    * ���������� ������ ��� ��������� �������.
    */
    void drawSprite( enum UIDEvent e );




    /**
    * ��������� �������, ������� �� �� ������� ���� 'extension' �� 'path',
    * �������� ��������� 'configure'.
    *
    * # ������ �������� ������������� �� ������ ������� - ��. ����������
    *   �������� � Event.
    * # ������ � ������� ������ ���� ������ �������.
    */
    void addSprite(
        const std::string&            path,
        const std::string&            extension,
        const typelib::json::Variant* configure = nullptr
    );




    /**
    * ��������� ���������� ������.
    */
    void addSprite(
        enum UIDEvent,
        std::unique_ptr< CL_Sprite >
    );




    /**
    * @return ������ ������ � ���, ������� ����� �������� �� ������.
    *         ������ ����� �������� ��� ���� ������.
    */
    virtual typelib::size2Int_t needVisualSize() const = 0;





    /**
    * @return ���������� ������.
    *
    * # ������� � ����������, ����� ����� �������� ������ �� ��������� ����.
    */
    template< typename T1, typename T2 >
    inline void visualCoord( T1 x, T1 y ) {
        visualCoord( coord2Int_t( x, y ) );
    };


    inline const typelib::coord2Int_t& visualCoord() const {
        return mCoord;
    };


    inline void visualCoord( const typelib::coord2Int_t& c ) {
        mCoord = c;
    };




    /**
    * @return ���� �������� �������� � ��������.
    */
    template< typename T >
    inline void visualRotation( T a ) {
        mRotation = static_cast< int >( a );
    };


    template< typename T >
    inline T visualRotation() const {
        return static_cast< T >( mRotation );
    };


    inline int visualRotation() const {
        return mRotation;
    };




    /**
    * @return ������������ �������� [0.0; 1.0].
    */
    template< typename T >
    inline void visualAlpha( T a ) {
        mAlpha = static_cast< float >( a );
    };


    inline float visualAlpha() const {
        return mAlpha;
    };




    inline bool operator==( const CLIncarnate& b ) const {
        // ���������� �������� �������
        return (*mSprite == *b.sprite());
    }




private:
    static void CLIncarnate::configure(
        CL_Sprite*,
        const typelib::json::Variant&
    );




private:
    std::weak_ptr< World >  mWorld;

    typelib::coord2Int_t  mCoord;
    int                   mRotation;
    float                 mAlpha;

    // @see addSprite(), drawSprite()
    std::unique_ptr< sprite_t >  mSprite;

    /**
    * ������ ���� ��� �������� ��������� ����������
    * �������� (mPreparedWindowSize) � ������� ������
    * ���� (mWindowSize).
    */
    static const CL_Size  mPreparedWindowSize;
    const CL_Size         mWindowSize;
    const CL_Sizef        mWindowScale;

    static std::unique_ptr< ManagerSprite >  mManagerSprite;
};



} // matchn
