#pragma once

#include "configure.h"
#include "struct.h"


namespace matchn {


class World;


/**
* Элемент игры, с которым можно взаимодействовать.
* GE - сокр. от GameElement.
*/
class GE :
    public std::enable_shared_from_this< GE >
{
public:
    explicit GE( std::shared_ptr< World > );



    virtual ~GE();




    // # Название сделаем уникальным, чтобы разрешить необнозначности при
    //   вызове метода элементами.
    std::shared_ptr< World >  allWorld() const;




    std::shared_ptr< World >  allWorld();



    /**
    * @return Элемент существует.
    */
    inline bool live() const {
        return !mDeleted;
    }



    /**
    * Помечает элемент на удаление.
    */
    inline void unlive() {
        mDeleted = true;;
    }




private:
    std::weak_ptr< World >  mWorld;

    /**
    * Признак: элемент игры удалён.
    */
    bool mDeleted;
};



} // matchn
