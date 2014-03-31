#pragma once

#include "configure.h"
#include "struct.h"


namespace matchn {


class World;


/**
* ������� ����, � ������� ����� �����������������.
* GE - ����. �� GameElement.
*/
class GE :
    public std::enable_shared_from_this< GE >
{
public:
    explicit GE( std::shared_ptr< World > );



    virtual ~GE();




    // # �������� ������� ����������, ����� ��������� ��������������� ���
    //   ������ ������ ����������.
    std::shared_ptr< World >  allWorld() const;




    std::shared_ptr< World >  allWorld();



    /**
    * @return ������� ����������.
    */
    inline bool live() const {
        return !mDeleted;
    }



    /**
    * �������� ������� �� ��������.
    */
    inline void unlive() {
        mDeleted = true;;
    }




private:
    std::weak_ptr< World >  mWorld;

    /**
    * �������: ������� ���� �����.
    */
    bool mDeleted;
};



} // matchn
