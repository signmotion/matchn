#pragma once

#include "GE.h"


namespace matchn {


class World;
class Background;
class Element;

    
/**
* Территория - место, где находится игрок. Аналог игрового уровня.
*/
class Territory :
    public std::enable_shared_from_this< Territory >,
    public GE
{
public:
    /**
    * Воплощённые на этой территории элементы.
    * Хранит элементы в поле N x M клеток.
    *
    * # Должен совпадать с ManagerEvent::incarnateElement_t.
    */
    typedef std::vector< std::unique_ptr< Element > >
        incarnateElement_t;




    /**
    * Название территории для однозначной идентификации.
    */
    const std::string  name;




protected:
    /**
    * @exception CL_Exception
    */
    Territory(
        std::shared_ptr< World >,
        const std::string&  name
    );




public:
    static std::shared_ptr< Territory >  valueOf(
        std::shared_ptr< World >,
        const std::string&  name
    );




    virtual ~Territory();




    incarnateElement_t const* incarnateElement() const;


    incarnateElement_t* incarnateElement();




    /**
    * Воплощает элемент.
    */
    void incarnateElement(
        uidElement_t,
        const typelib::coord2Int_t&
    );




    /**
    * Визуализирует территорию.
    */
    void draw();




    /**
    * @return Количество воплощённых на территории элементов.
    */
    size_t countElement() const;




    /**
    * Заполняем все пустые ячейки случайными элементами.
    */
    void fillAllEmpty();




    /**
    * Заполняем одну (первую встретившуюся) пустую ячейку случайным элементом.
    * 
    * @return false, если не было пустых ячеек.
    */
    bool fillEmpty();




    /**
    * Заполняем в верхнем ряду одну (первую встретившуюся) пустую ячейку
    * случайным элементом.
    * 
    * @return false, если не было пустых ячеек.
    */
    bool fillTopEmpty();




    /**
    * Удаляет один (первый встретившийся) неживой элемент.
    * 
    * @return false, если все элементы живые.
    */
    bool removeUnlive();




    /**
    * @return Случайный UID элемента, который может воплотиться на
    *         этой территории.
    */
    uidElement_t nextRandom() const;




private:
    /**
    * Загружает территорию и все нужные для неё ресурсы (в частности, графику).
    */
    void load();

    void loadBackground();
    void loadElement();




    void drawBackground();
    void drawIncarnateElement();




private:
    /**
    * Фон территории.
    */
    std::unique_ptr< Background >  mBackground;


    /**
    * Воплощённые на этой территории элементы.
    */
    std::unique_ptr< incarnateElement_t >  mIncarnateElement;
};



} // matchn









namespace std {

std::ostream& operator<<( std::ostream&,  const matchn::Territory& );

} // std
