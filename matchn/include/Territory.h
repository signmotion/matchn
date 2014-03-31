#pragma once

#include "GE.h"


namespace matchn {


class World;
class Background;
class Element;

    
/**
* ���������� - �����, ��� ��������� �����. ������ �������� ������.
*/
class Territory :
    public std::enable_shared_from_this< Territory >,
    public GE
{
public:
    /**
    * ����������� �� ���� ���������� ��������.
    * ������ �������� � ���� N x M ������.
    *
    * # ������ ��������� � ManagerEvent::incarnateElement_t.
    */
    typedef std::vector< std::unique_ptr< Element > >
        incarnateElement_t;




    /**
    * �������� ���������� ��� ����������� �������������.
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
    * ��������� �������.
    */
    void incarnateElement(
        uidElement_t,
        const typelib::coord2Int_t&
    );




    /**
    * ������������� ����������.
    */
    void draw();




    /**
    * @return ���������� ����������� �� ���������� ���������.
    */
    size_t countElement() const;




    /**
    * ��������� ��� ������ ������ ���������� ����������.
    */
    void fillAllEmpty();




    /**
    * ��������� ���� (������ �������������) ������ ������ ��������� ���������.
    * 
    * @return false, ���� �� ���� ������ �����.
    */
    bool fillEmpty();




    /**
    * ��������� � ������� ���� ���� (������ �������������) ������ ������
    * ��������� ���������.
    * 
    * @return false, ���� �� ���� ������ �����.
    */
    bool fillTopEmpty();




    /**
    * ������� ���� (������ �������������) ������� �������.
    * 
    * @return false, ���� ��� �������� �����.
    */
    bool removeUnlive();




    /**
    * @return ��������� UID ��������, ������� ����� ����������� ��
    *         ���� ����������.
    */
    uidElement_t nextRandom() const;




private:
    /**
    * ��������� ���������� � ��� ������ ��� �� ������� (� ���������, �������).
    */
    void load();

    void loadBackground();
    void loadElement();




    void drawBackground();
    void drawIncarnateElement();




private:
    /**
    * ��� ����������.
    */
    std::unique_ptr< Background >  mBackground;


    /**
    * ����������� �� ���� ���������� ��������.
    */
    std::unique_ptr< incarnateElement_t >  mIncarnateElement;
};



} // matchn









namespace std {

std::ostream& operator<<( std::ostream&,  const matchn::Territory& );

} // std
