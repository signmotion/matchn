#pragma once

#include "GE.h"
#include "CLIncarnate.h"


namespace matchn {


class CLIncarnate;
class World;


/**
* ��� ����������.
*/
class Background :
    public GE,
    public CLIncarnate
{
protected:
    Background(
        std::shared_ptr< World >,
        const std::string&  name
    );




public:
    static std::unique_ptr< Background >  valueOf(
        std::shared_ptr< World >,
        const std::string&  name
    );



    virtual ~Background();



    inline std::string const& name() const {
        return mName;
    }




    /**
    * @virtual CLIncarnate
    */
    virtual inline typelib::size2Int_t needVisualSize() const {
        // ��� ������ ��������� �� ������� ������� (�� ����)
        return typelib::size2Int_t( WINDOW_WIDTH, WINDOW_HEIGHT );
    }




    /**
    * ������������� ���.
    */
    void draw();




private:
    /**
    * ��������� ������ ���� ������� (� ���������, �������).
    */
    void load();


    void drawImage();
    void drawArea();




protected:
    /**
    * �������� ���� ��� ����������� �������������.
    */
    const std::string&  mName;
};


} // matchn






namespace boost {

inline size_t hash_value( const matchn::Background& bg ) {
    return boost::hash_value( bg.name() );
}

} // boost
