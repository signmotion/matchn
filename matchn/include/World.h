#pragma once

#include "struct.h"


namespace matchn {


class LogicWorld;
class Element;
class Territory;
class ManagerEvent;


/**
* Класс для работы с игровым миром.
* Создаёт и хранит все миры игры: физический, визуальный, аудио и пр..
*/
class World :
    public std::enable_shared_from_this< World >
{
public:
    /**
    * Мир может возвращать вызвавшему его методу некоторые команды.
    */
    enum COMMAND {
        /**
        * Команда не определена.
        */
        UNDEFINED = 0,

        /**
        * Ожидается меньше собранных вместе элементов (легче).
        */
        DECREASE_CHAIN,

        /**
        * Перезапустить мир с .
        */
        DECREASE_TERRITORY,

        /**
        * Ожидается больше собранных вместе элементов (сложнее).
        */
        INCREASE_CHAIN,

        /**
        * Перезапустить мир с другими характеристиками.
        */
        INCREASE_TERRITORY,

        /**
        * Завершить работу.
        */
        QUIT
    };


    /**
    * @todo extend Некоторые команды могут возвращать параметры. Все они
    *       хранятся в виде JSON-объекта.
    */
    typedef typelib::json::Variant                variant_t;
    typedef std::pair< enum COMMAND, variant_t >  command_t;




protected:
    /**
    * @exception CL_Exception
    */
    World( size_t K, size_t N, size_t M );




public:
    // Для инициализации this->shared_from_this()
    static std::shared_ptr< World > valueOf(
        size_t K = 3,
        size_t N = 5,
        size_t M = 5
    );




    virtual ~World();




    /**
    * @return Миры мира.
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
    * @return Территория мира.
    */
    inline const std::shared_ptr< Territory >  territory() const {
        return mTerritory;
    }

    inline std::shared_ptr< Territory >  territory() {
        return mTerritory;
    }




    /**
    * @return Менеджер событий.
    */
    inline const ManagerEvent*  managerEvent() const {
        return mManagerEvent.get();
    }

    inline ManagerEvent*  managerEvent() {
        return mManagerEvent.get();
    }




    /**
    * Оживляет мир.
    *
    * @return Команду мира.
    */
	command_t go();




    /**
    * Переводит систему в след. состояние.
    *
    * @param deltaTime Время, прошедшее с вызова прошлого pulse(), мс.
    *
    * @see mCurrentTime
    */
    void pulse( int deltaTime );




private:
    /**
    * Слушатели событий визуального мира.
    */
	void onMouseDown( const CL_InputEvent&, const CL_InputState& );
	void onMouseMove( const CL_InputEvent&, const CL_InputState& );
	void onKeyboardDown( const CL_InputEvent&, const CL_InputState& );
	void onWindowClose();




    /**
    * Проверяет, разрешено ли игроку взаимодействовать с миром.
    */
    bool locked() const;




    /**
    * Отрабатывает обмен элементов.
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
    * @return Кадры в секунду (Frame Per Second).
    */
    static size_t World::calcFPS( size_t frameTime );




private:
    /**
    * Логический мир.
    */
    std::unique_ptr< LogicWorld >  mLogicWorld;


    /**
    * Отображаемый мир.
    */
    std::unique_ptr< CL_DisplayWindow >  mVisualWorld;


    /**
    * События от игрока.
    */
    CL_Slot  mSlotMouseDown;
    CL_Slot  mSlotMouseMove;
    CL_Slot  mSlotKeyboardDown;
    CL_Slot  mSlotWindowClose;


    /**
    * Территория.
    */
    std::shared_ptr< Territory >  mTerritory;


    /**
    * Управление игровыми событиями.
    */
    std::unique_ptr< ManagerEvent >  mManagerEvent;


    /**
    * Позиция точки внимания на игровом экране.
    * 
    * # Точка представлена физическими координатами.
    *   См. NDIncarnate::fromVisual().
    *
    * @see onMouseMove()
    */
    typelib::coord2Int_t  mAttentionPoint;


    /**
    * Текущее время, мс.
    * Может использоваться для корректного тайминга.
    */
    int mCurrentTime;
};



} // matchn
