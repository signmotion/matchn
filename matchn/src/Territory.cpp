#include "../include/stdafx.h"
#include "../include/Territory.h"
#include "../include/World.h"
#include "../include/Background.h"
#include "../include/Element.h"
#include "../include/LogicWorld.h"


namespace matchn {


Territory::Territory(
    std::shared_ptr< World >  world,
    const std::string&  name
) :
    GE( world ),
    mIncarnateElement( new incarnateElement_t( world->logic()->volume() ) ),
    name( name )
{
    assert( !this->name.empty()
        && "Не указано название территории." );

    // создаём пустое поле N x M
    // @todo ...

    load();
}




std::shared_ptr< Territory >  Territory::valueOf(
    std::shared_ptr< World >  world,
    const std::string&  name
) {
    return std::shared_ptr< Territory >( new Territory( world, name ) );
}




Territory::~Territory() {
}




Territory::incarnateElement_t const*
Territory::incarnateElement() const {
    return mIncarnateElement.get();
}




Territory::incarnateElement_t*
Territory::incarnateElement() {
    return mIncarnateElement.get();
}




void Territory::load() {
    loadBackground();
}




void Territory::loadBackground() {
    mBackground = Background::valueOf( allWorld(), name );
    assert( mBackground
        && "Фон для территории не получен." );
}




void Territory::incarnateElement(
    uidElement_t                 uid,
    const typelib::coord2Int_t&  coord
) {
    auto element = Element::valueOf( allWorld(), uid, coord );
    
#ifdef _DEBUG
    std::cout << "Воплощение элемента " << *element << "\n";
#endif

    const auto i = allWorld()->logic()->ic( coord );
    mIncarnateElement->at( i ) = std::move( element );

#ifdef _DEBUG
    std::cout << "На территории " << *this << std::endl;
#endif
}




void Territory::draw() {
    drawBackground();
    drawIncarnateElement();
}




void Territory::drawBackground() {
    mBackground->draw();
}




void Territory::drawIncarnateElement() {
    for (auto itr = mIncarnateElement->cbegin(); itr != mIncarnateElement->cend(); ++itr) {
        Element* const element = ( *itr ).get();
        // отрисовываем только живые элементы
        if ( element && element->live() ) {
            // # Координаты э. в визуальном мире установлены при
            //   вызове incarnateElement()
            element->draw();
        }
    }
}




size_t Territory::countElement() const {
    size_t n = 0;
    for (auto itr = mIncarnateElement->cbegin(); itr != mIncarnateElement->cend(); ++itr) {
        Element* const element = ( *itr ).get();
        if ( element ) {
            n++;
        }
    }

    return n;
}




void Territory::fillAllEmpty() {
    while ( fillEmpty() ) {
        // fillEmpty() знает когда прекратить
    }
}




bool Territory::fillEmpty() {

    // найдём первую пустую ячейку и заполним её случайным элементом
    for (auto itr = mIncarnateElement->begin();
         itr != mIncarnateElement->end(); ++itr
    ) {
        Element* const element = ( *itr ).get();
        if ( !element ) {
            const auto uid = nextRandom();
            const auto i = std::distance( mIncarnateElement->begin(), itr );
            const auto cc = allWorld()->logic()->ci( i );
            incarnateElement( uid, cc );
            return true;
        }
    }

    // заполнять нечего
    return false;
}




bool Territory::fillTopEmpty() {

    // найдём в верхнем ряду первую пустую ячейку и заполним её
    // случайным элементом
    const auto lw = allWorld()->logic();
    typelib::coord2Int_t  cc( 0, lw->maxCoord().y );
    for (cc.x = lw->minCoord().x; cc.x <= lw->maxCoord().x; ++cc.x) {
        const auto i = lw->ic( cc );
        auto& element = incarnateElement()->at( i );
        if ( !element ) {
            const auto uid = nextRandom();
            incarnateElement( uid, cc );
            return true;
        }
    }

    // заполнять нечего
    return false;
}




bool Territory::removeUnlive() {

    // найдём первый мёртвый элемент и реально удалим его
    for (auto itr = mIncarnateElement->begin();
         itr != mIncarnateElement->end(); ++itr
    ) {
        Element* const element = ( *itr ).get();
        if ( element && !element->live() ) {
#ifdef _DEBUG
            std::cout << "Удаление элемента " << *element << std::endl;
#endif
            itr->release();
            return true;
        }
    }

    // удалять нечего
    return false;
}




uidElement_t Territory::nextRandom() const {

    static boost::mt19937  gen;
    // # Последним всегда идёт бонусный элемент: его надо заслужить, собирая
    //   вместе первые. Поэтому: -2.
    const size_t ne = allWorld()->logic()->countAvailableElement();
    assert( (ne >= 2)
        && "В папке 'media/element' должно быть больше одного элемента." );
    static boost::uniform_int<>   uni( 'a', 'a' + ne - 2 );
    static boost::variate_generator< boost::mt19937&, boost::uniform_int<> >
        random( gen, uni );

    return static_cast< uidElement_t >( random() );
}




} // matchn








namespace std {

inline std::ostream& operator<<(
    std::ostream& out,
    const matchn::Territory& tr
) {
    out << tr.countElement() << " элементов" << std::endl;
    return out;
}

} // std
