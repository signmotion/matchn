#pragma once

#include "struct.h"


namespace matchn {


class LogicWorld;
class Element;
class Territory;
class ManagerEvent;


/**
* ����� ��� ������ � ������� �����.
* ������ � ������ ��� ���� ����: ����������, ����������, ����� � ��..
*/
class World :
    public std::enable_shared_from_this< World >
{
public:
    /**
    * ��� ����� ���������� ���������� ��� ������ ��������� �������.
    */
    enum COMMAND {
        /**
        * ������� �� ����������.
        */
        UNDEFINED = 0,

        /**
        * ��������� ������ ��������� ������ ��������� (�����).
        */
        DECREASE_CHAIN,

        /**
        * ������������� ��� � .
        */
        DECREASE_TERRITORY,

        /**
        * ��������� ������ ��������� ������ ��������� (�������).
        */
        INCREASE_CHAIN,

        /**
        * ������������� ��� � ������� ����������������.
        */
        INCREASE_TERRITORY,

        /**
        * ��������� ������.
        */
        QUIT
    };


    /**
    * @todo extend ��������� ������� ����� ���������� ���������. ��� ���
    *       �������� � ���� JSON-�������.
    */
    typedef typelib::json::Variant                variant_t;
    typedef std::pair< enum COMMAND, variant_t >  command_t;




protected:
    /**
    * @exception CL_Exception
    */
    World( size_t K, size_t N, size_t M );




public:
    // ��� ������������� this->shared_from_this()
    static std::shared_ptr< World > valueOf(
        size_t K = 3,
        size_t N = 5,
        size_t M = 5
    );




    virtual ~World();




    /**
    * @return ���� ����.
    */
    inline const LogicWorld* logic() const {
        return mLogicWorld.get();
    }

    inline LogicWorld* logic() {
        return mLogicWorld.get();
    }

    inline const CL_DisplayWindow* visual() const {
        return mVisualWorld.get();
    }

    inline CL_DisplayWindow* visual() {
        return mVisualWorld.get();
    }




    /**
    * @return ���������� ����.
    */
    inline const std::shared_ptr< Territory >  territory() const {
        return mTerritory;
    }

    inline std::shared_ptr< Territory >  territory() {
        return mTerritory;
    }




    /**
    * @return �������� �������.
    */
    inline const ManagerEvent*  managerEvent() const {
        return mManagerEvent.get();
    }

    inline ManagerEvent*  managerEvent() {
        return mManagerEvent.get();
    }




    /**
    * �������� ���.
    *
    * @return ������� ����.
    */
	command_t go();




    /**
    * ��������� ������� � ����. ���������.
    *
    * @param deltaTime �����, ��������� � ������ �������� pulse(), ��.
    *
    * @see mCurrentTime
    */
    void pulse( int deltaTime );




private:
    /**
    * ��������� ������� ����������� ����.
    */
	void onMouseDown( const CL_InputEvent&, const CL_InputState& );
	void onMouseMove( const CL_InputEvent&, const CL_InputState& );
	void onKeyboardDown( const CL_InputEvent&, const CL_InputState& );
	void onWindowClose();




    /**
    * ���������, ��������� �� ������ ����������������� � �����.
    */
    bool locked() const;




    /**
    * ������������ ����� ���������.
    */
    void changeEvent( Element* a,  Element* b );

    static void swap( Element* a,  Element* b );

    std::set< Element* >  beside( Element* ) const;

    static void signAllNeighbour(
        std::set< Element* >*  result,
        uidElement_t,
        const Element*
    );

    void harvest( std::set< Element* >& );



    /**
    * @return ����� � ������� (Frame Per Second).
    */
    static size_t World::calcFPS( size_t frameTime );




private:
    /**
    * ���������� ���.
    */
    std::unique_ptr< LogicWorld >  mLogicWorld;


    /**
    * ������������ ���.
    */
    std::unique_ptr< CL_DisplayWindow >  mVisualWorld;


    /**
    * ������� �� ������.
    */
    CL_Slot  mSlotMouseDown;
    CL_Slot  mSlotMouseMove;
    CL_Slot  mSlotKeyboardDown;
    CL_Slot  mSlotWindowClose;


    /**
    * ����������.
    */
    std::shared_ptr< Territory >  mTerritory;


    /**
    * ���������� �������� ���������.
    */
    std::unique_ptr< ManagerEvent >  mManagerEvent;


    /**
    * ������� ����� �������� �� ������� ������.
    * 
    * # ����� ������������ ����������� ������������.
    *   ��. NDIncarnate::fromVisual().
    *
    * @see onMouseMove()
    */
    typelib::coord2Int_t  mAttentionPoint;


    /**
    * ������� �����, ��.
    * ����� �������������� ��� ����������� ��������.
    */
    int mCurrentTime;
};



} // matchn
