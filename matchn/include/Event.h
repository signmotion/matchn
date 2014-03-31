#pragma once

#include "configure.h"
#include "struct.h"


namespace matchn {


class Element;


/**
* Перечисление возможных событий в игре.
*
* # События упорядочены по алфавиту.
*
* @see Декларацию и коммент. к классам Event* ниже.
*
* @todo # События проверки вынесены в общий поток из-за небезопасной
*       потоковой синхронизации при работе с веткорами. Проработать
*       безопасную схему.
*/
enum UIDEvent {
    NONE_EVENT = -1,

    /**
    * Игрок уделяет внимание этому игровому элементу (навёл курсор).
    */
    ATTENTION_EVENT,

    /**
    * Элемент перемещается на ячейку ниже.
    */
    COMPLETE_DROP_EVENT,

    /**
    * Элементы меняются местами.
    */
    CHANGE_EVENT,

    /**
    * Элемент готов упасть ниже.
    */
    DROP_EVENT,

    /**
    * Элемент должен исчезнуть (собрано нужное кол-во).
    */
    HIDE_HARVESTED_EVENT,

    /**
    * Элемент удаляется с территории (с игрового поля).
    */
    LEAVE_EVENT,

    /**
    * Игрок отметил этот элемент.
    */
    SELECT_EVENT,

    /**
    * Обновляет логические координаты по текущим визуальным.
    */
    UPDATE_LOGIC_COORD_EVENT,

    /**
    * Проверка "что собрано".
    */
    VERIFY_HARVESTED_EVENT,

    /**
    * Событие "Ожидание других событий".
    */
    WAIT_EVENT
};




/**
* Соответствие числовых UID строковым для события.
*/
typedef boost::bimap< enum UIDEvent, std::string >  accordanceEvent_t;




static const accordanceEvent_t accordanceEvent =
    boost::assign::list_of< accordanceEvent_t::relation >
        ( NONE_EVENT,               "" )
        ( ATTENTION_EVENT,          "attention" )
        ( COMPLETE_DROP_EVENT,      "complete-drop-event" )
        ( DROP_EVENT,               "drop" )
        ( HIDE_HARVESTED_EVENT,     "hide-harvested" )
        ( CHANGE_EVENT,             "change" )
        ( LEAVE_EVENT,              "leave" )
        ( VERIFY_HARVESTED_EVENT,   "verify-harvested" )
        ( SELECT_EVENT,             "select" )
        ( UPDATE_LOGIC_COORD_EVENT, "update-logic-coord" )
        ( WAIT_EVENT,               "wait" )
    ;




/**
* @return UID события по названию.
*/
static inline enum UIDEvent convertEvent( const std::string& name ) {
    const auto ftr = accordanceEvent.right.find( name );
    assert( (ftr != accordanceEvent.right.end())
        && "События с таким названием не существует." );
    return ftr->second;
}




/**
* @return Название события по UID.
*/
static inline std::string const&  convertEvent( UIDEvent uid ) {
    const auto ftr = accordanceEvent.left.find( uid );
    assert( (ftr != accordanceEvent.left.end())
        && "События с таким UID не существует." );
    return ftr->second;
}








/**
* Игровое событие.
*
* # Любое событие имеет время начала (мс), длительность (мс) и событие,
*   следующее по завершению.
*/
template< class E >
class Event {
public:
    typedef Event< E >  event_t;



protected:
    /*
    * @param startTime  Время начала события.
    * @param duration   Длительность события. Если == -1, событие бесконечно.
    * @param next       Следующее событие. Если не указано, принимается WaitEvent.
    *
    * # Отсутствие след. события переводит элемент в событие 'WAIT'.
    * # Длительность события должна быть > 0 или == -1.
    */
    Event( E*, int startTime, int duration );




public:
    virtual ~Event();




    // @todo fine Возвращать пару значений: UID и строковое именование
    //       события. Позволит избавиться от глобальных методов.
    //       Ещё лучше: работать только с событиями-классами, не дублируя
    //       их в enum EVENT.
    virtual enum UIDEvent uid() const = 0;




    /**
    * @alias (uid() == enum Event::NONE)
    */
    inline bool undefined() const {
        return (uid() == NONE_EVENT);
    }




    inline int startTime() const {
        return mStartTime;
    }




    inline int duration() const {
        return mDuration;
    }




    /**
    * @return Событие актуально на данный момент времени.
    *
    * # При вызове без параметра, проверяется только 'mCompleted'.
    */
    inline bool actuality( int currentTime = 0 ) const {
        return !mCompleted && ((currentTime < mEndTime) || infinitely());
    }




    /**
    * @return Событие бесконечно.
    */
    inline bool infinitely() const {
        return (mStartTime > mEndTime);
    }




    /**
    * Отмечает событие элемента как завершённое.
    *
    * @see actuality()
    */
    inline void complete() {
        mCompleted = true;
    }




    /**
    * @return Сколько времени в мс осталось до завершения этого события.
    *         Отрицательное число или 0 - событие завершилось.
    */
    inline int deltatime( int currentTime ) const {
        return (mEndTime - currentTime);
    }




    /**
    * @return Сколько в диапазоне  [ 0.0 (началось); 1.0 (завершено) )
    *         осталось до завершения этого события.
    */
    inline float nepperDeltatime( int currentTime ) const {
        assert( (mDuration != 0)
            && "Нарушено соглашение: Длительность события не может быть == 0." );
        const float u =
            1.0f -
            static_cast< float >( mEndTime - currentTime ) /
            static_cast< float >( mDuration );
        return (u > 1.0f) ? 1.0f : (u < 0.0f) ? 0.0f : u;
    }




    /**
    * Отрабатывает событие.
    */
    virtual void make( int currentTime ) = 0;




protected:
    E*         mElement;

    /**
    * Событие может завершиться по истечении времени.
    */
    const int  mStartTime;
    const int  mDuration;


    /**
    * Событие может завершить само себя, отрабатывая make().
    */
    bool mCompleted;


    /**
    * Когда событие должно завершиться по времени.
    * Считаем в конструкторе для ускорения.
    */
    const int  mEndTime;
};








/**
* Элементу игры уделено внимание.
*/
template< class E >
class AttentionEvent :
    public Event< E >
{
public:
    inline AttentionEvent( E* e, int startTime, int duration ) :
        Event< E >( e, startTime, duration )
    {}




    virtual inline enum UIDEvent uid() const {
        return ATTENTION_EVENT;
    }




    virtual inline void make( int currentTime ) {
#ifdef _DEBUG_EVENT
        //std::cout << "make AttentionEvent " << *mElement << "\n";
#endif

        // # Событие завершится по таймеру.
    }
};








/**
* Элемент перемещается на ячейку ниже.
*/
template< class E >
class CompleteDropEvent :
    public Event< E >
{
public:
    inline CompleteDropEvent( E* e, int startTime, int duration ) :
        Event< E >( e, startTime, duration )
    {}




    virtual inline enum UIDEvent uid() const {
        return COMPLETE_DROP_EVENT;
    }




    virtual inline void make( int currentTime ) {
#ifdef _DEBUG_EVENT
        std::cout << "make CompleteDropEvent " << *mElement << "\n";
#endif

        const auto lw = mElement->allWorld()->logic();
        const auto ia = lw->ic( mElement->coord() );

        const auto shift = lw->isc( 3 );
        const auto coordAbove = mElement->coord() + shift;
        assert ( lw->inside( coordAbove )
            && "Попытка уронить элемент ниже плинтуса." );
        mElement->coord( coordAbove );
        mElement->updateVisualCoordByLogic();

        // меняем положение в ячейках территории
        // @see todo World::swap()
        const auto ib = lw->ic( coordAbove );
        auto incarnate =
            mElement->allWorld()->territory()->incarnateElement();
        std::swap(
            incarnate->at( ia ),
            incarnate->at( ib )
        );


        // событие завершается
        complete();
    }
};







#if 0
// - См. прим. в коде World.cpp.

/**
* Элементы игры меняется местами с другим элементом.
*/
template< class E >
class ChangeEvent :
    public Event< E >
{
public:
    inline ChangeEvent( E* e, int startTime, int duration ) :
        Event< E >( e, startTime, duration )
    {
        // # Каждое новое событие сбрасывает координату-признак.
        mCoord = typelib::coord2Int_t::UNDEFINED();
    }




    virtual inline enum UIDEvent uid() const {
        return CHANGE_EVENT;
    }




    virtual inline void make( int currentTime ) {
#ifdef _DEBUG_EVENT
        std::cout << "make ChangeEvent " << *mElement << "\n";
#endif

        // событие завершается
        // # Отмечаем здесь, т.к. для других элементов отрабатывать
        //   событие не надо.
        complete();

        // элементы меняются местами 1 раз
        if ( !mCoord.undefined() ) {
            return;
        }

        auto a = mElement;
        auto b = a->allWorld()->managerEvent()->other( CHANGE_EVENT, a );
        if ( !b ) {
            return;
        }

        auto sa = a->sprite( CHANGE_EVENT );
        auto sb = b->sprite( CHANGE_EVENT );

        // визуальные координаты
        mCoord = a->visualCoord();
        a->visualCoord( b->visualCoord() );
        b->visualCoord( mCoord );
        sa->update();
        sb->update();

        // логические координаты
        a->updateLogicCoordByVisual();
        b->updateLogicCoordByVisual();
    }




private:
    /**
    * Событие должно выполниться один раз для двух элементов.
    */
    static typelib::coord2Int_t  mCoord;
};


template< class E >
typelib::coord2Int_t ChangeEvent< E >::mCoord =
    typelib::coord2Int_t::UNDEFINED();

#endif







/**
* Элемент готов упасть ниже.
*/
template< class E >
class DropEvent :
    public Event< E >
{
public:
    inline DropEvent( E* e, int startTime, int duration ) :
        Event< E >( e, startTime, duration )
    {}




    virtual inline enum UIDEvent uid() const {
        return DROP_EVENT;
    }




    virtual inline void make( int currentTime ) {
#ifdef _DEBUG_EVENT
        std::cout << "make DropEvent " << *mElement << "\n";
#endif

        const auto nepper = nepperDeltatime( currentTime );
        const auto height =
            static_cast< float >( mElement->needVisualSize().y );
        const auto shift = nepper * height;

        const auto lw = mElement->allWorld()->logic();
        const auto lc = mElement->coord();
        const auto vc = lw->visualByLogicCoord( lc );
        mElement->visualCoord( typelib::coord2Int_t(
            vc.x,  vc.y + shift
        ) );

        // # Событие завершится по таймеру.
    }
};








/**
* Элемент должен исчезнуть (собрано нужное кол-во).
*/
template< class E >
class HideHarvestedEvent :
    public Event< E >
{
public:
    inline HideHarvestedEvent( E* e, int startTime, int duration ) :
        Event< E >( e, startTime, duration )
    {}




    virtual inline enum UIDEvent uid() const {
        return HIDE_HARVESTED_EVENT;
    }




    virtual inline void make( int currentTime ) {
#ifdef _DEBUG_EVENT
        std::cout << "make HideHarvestedEvent " << *mElement << "\n";
#endif

        // гасим элементы
        const auto alpha = 1.0f - nepperDeltatime( currentTime );
        /*
        auto s = mElement->sprite( HIDE_HARVESTED_EVENT );
        s->set_alpha( alpha );
        s->update();
        */
        mElement->visualAlpha( alpha );

        // # Событие завершится по таймеру.
    }
};








/**
* Элемент удаляется с территории (с игрового поля).
*/
template< class E >
class LeaveEvent :
    public Event< E >
{
public:
    inline LeaveEvent( E* e, int startTime, int duration ) :
        Event< E >( e, startTime, duration )
    {}




    virtual inline enum UIDEvent uid() const {
        return LEAVE_EVENT;
    }




    virtual inline void make( int currentTime ) {
#ifdef _DEBUG_EVENT
        std::cout << "make LeaveEvent " << *mElement << "\n";
#endif

        // # Основной процесс разберётся с "мёртвыми" элементами и
        //   создаст новые.
        mElement->unlive();

        // событие завершается
        complete();
    }
};








/**
* Элемент игры выделен.
*/
template< class E >
class SelectEvent :
    public Event< E >
{
public:
    inline SelectEvent( E* e, int startTime, int duration ) :
        Event< E >( e, startTime, duration )
    {}




    virtual inline enum UIDEvent uid() const {
        return SELECT_EVENT;
    }




    virtual inline void make( int currentTime ) {
#ifdef _DEBUG_EVENT
        std::cout << "make SelectEvent " << *mElement << "\n";
#endif

        // # Проще было бы закатать всё в спрайты-образы и только включать их
        //   при наступлении события. Но здесь покажем, что можем работать
        //   с ними и более гибко.
        /*
        auto s = mElement->sprite( SELECT_EVENT );
        s->rotate( CL_Angle( 1.0f, cl_degrees ) );
        s->update();
        */
        const auto angle = mElement->visualRotation();
        mElement->visualRotation( angle + 2 );

        // # Событие завершится по таймеру.
    }
};







/**
* Обновляет логические координаты по текущим визуальным.
*/
template< class E >
class UpdateLogicCoordEvent :
    public Event< E >
{
public:
    inline UpdateLogicCoordEvent( E* e, int startTime, int duration ) :
        Event< E >( e, startTime, duration )
    {}




    virtual inline enum UIDEvent uid() const {
        return UPDATE_LOGIC_COORD_EVENT;
    }




    virtual inline void make( int currentTime ) {
#ifdef _DEBUG_EVENT
        std::cout << "make UpdateLogicCoordEvent " << *mElement << "\n";
#endif

        mElement->updateLogicCoordByVisual();

        // событие завершается
        complete();
    }
};







#if 0
// @todo События проверки вынесены в общий поток из-за небезопасной
//       потоковой синхронизации при работе с веткорами. Проработать
//       безопасную схему.


/**
* Проверка "что собрано".
*/
template< class E >
class VerifyHarvestedEvent :
    public Event< E >
{
public:
    inline VerifyHarvestedEvent( E* e, int startTime, int duration ) :
        Event< E >( e, startTime, duration )
    {}




    virtual inline enum UIDEvent uid() const {
        return VERIFY_HARVESTED_EVENT;
    }




    virtual inline void make( int currentTime );




private:
    /**
    * @return Элементы рядом.
    */
    std::set< E* >  beside() const;


    // вспом. метод для beside()
    static void signAllNeighbour(
        std::set< E* >*  result,
        uidElement_t,
        const E*
    );




    /**
    * Элементы не собраны в нужном кол-ве.
    */
    void notHarvested();




    /**
    * Собрано требуемое кол-во элементов.
    */
    void yesHarvested( std::set< E* >& );
};

#endif








/**
* Элемент игры ожидает событие.
*/
template< class E >
class WaitEvent :
    public Event< E >
{
public:
    inline WaitEvent( E* e, int startTime = 0, int duration = -1 );




    virtual inline enum UIDEvent uid() const {
        return WAIT_EVENT;
    }




    virtual void make( int currentTime );
};



} // matchn








namespace boost {

template< class E >
inline size_t hash_value( const matchn::Event< E >& e ) {
    return static_cast< size_t >( e.uid() );
}

} // boost
