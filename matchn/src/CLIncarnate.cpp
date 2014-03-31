#include "../include/stdafx.h"
#include "../include/CLIncarnate.h"
#include "../include/World.h"
#include "../include/ManagerSprite.h"


namespace matchn {


const CL_Size CLIncarnate::mPreparedWindowSize( 1024, 768 );

std::unique_ptr< ManagerSprite >  CLIncarnate::mManagerSprite(
    new ManagerSprite()
);



CLIncarnate::CLIncarnate(
    std::shared_ptr< World >  world,
    const typelib::coord2Int_t&  c,
    int  rotation
) :
    mWorld( world ),
    mCoord( c ),
    mRotation( rotation ),
    mAlpha( 1.0f ),
    mSprite( new sprite_t() ),
    // реальный размер окна - см. mWindowSize; mPreparedWindowSize используем
    // для позиционирования элементов
    mWindowSize( CLIncarnate::world()->get_geometry().get_size() ),
    mWindowScale(
        static_cast< float >( mWindowSize.width  ) /
            static_cast< float >( mPreparedWindowSize.width ),
        static_cast< float >( mWindowSize.height ) /
            static_cast< float >( mPreparedWindowSize.height )
    )
{
    assert( !mWorld.expired()
        && "Получен разрушенный мир." );
    assert( mWorld.lock()->visual()
        && "Визуальный мир должен быть инициализирован." );
}




CLIncarnate::~CLIncarnate() {
}




const CL_DisplayWindow* CLIncarnate::world() const {
    return mWorld.lock()->visual();
}


CL_DisplayWindow* CLIncarnate::world() {
    return mWorld.lock()->visual();
}




void CLIncarnate::drawSprite( enum UIDEvent e ) {
    auto s = sprite( e );
    assert( s
        && "Спрайт для заданного события не найден." );

    auto gc = world()->get_gc();
    s->set_angle( CL_Angle( visualRotation< float >(), cl_degrees ) );
    s->set_alpha( visualAlpha() );
    s->draw( gc, visualCoord().x, visualCoord().y );
    s->update();
}




void CLIncarnate::addSprite(
    const std::string&            path,
    const std::string&            extension,
    const typelib::json::Variant* config
) {
#ifdef _DEBUG
    //std::cout << "\nСканируем '" << path << "' на '" << extension << "'\n";
#endif

    // собираем фреймы для спрайта
    // # Фреймы спрайта сгруппированы по папкам событий.
    std::set< std::string >  event;
    /* - Учитываем все известные события, не только папки. См. ниже.
    typelib::file::listFolder( &event, path );
    */
    for (auto itr = accordanceEvent.begin(); itr != accordanceEvent.end(); ++itr) {
        if ( !itr->right.empty() ) {
            event.insert( itr->right );
        }
    }
    assert( !event.empty()
        && "Для спрайта не указано ни одного события."
           " Визуальный образ не может быть собран." );

    for (auto etr = event.cbegin(); etr != event.cend(); ++etr) {
        const std::string& e = *etr;
        if ( boost::starts_with( e, "-" ) ) {
            // папки, начинающиеся на '-', игнорируем
            continue;
        }

#ifdef _DEBUG
        //std::cout << "\nПодготавливаем спрайт для события '" << e << "'\n";
#endif

        // фреймы для визуального образа
        CL_SpriteDescription ds;
        std::set< std::string >  frame;
        std::string pathEvent = path + "/" + e;
        typelib::file::listFile( &frame, pathEvent, extension );
        /* - Берём спрайт из папки 'default'. См. ниже.
        assert( !frame.empty()
            && "Для события не указано ни одного спрайта. "
               " Визуальный образ не может быть собран." );
        */
        if ( frame.empty() ) {
            pathEvent = path + "/default";
            typelib::file::listFile( &frame, pathEvent, extension );
            assert( !frame.empty()
                && "Для элемента не найдено ни одного спрайта. "
                   " Визуальный образ не может быть собран." );
        }
        for (auto ftr = frame.cbegin(); ftr != frame.cend(); ++ftr) {
            const std::string& f = *ftr;
            const std::string pathFrame = pathEvent + "/" + f;
            const auto image = mManagerSprite->frame( pathFrame );
            ds.add_frame( *image );
        }

        // сам виз. образ для события 'e'
        std::unique_ptr< CL_Sprite >  sprite(
            new CL_Sprite( CLIncarnate::world()->get_gc(),  ds )
        );

        // конфигурация образа
        /* - @todo ...
        {
            // если конфигурация образа не задана явно, попробуем найти
            // её в папке спрайта
            if ( config ) {
                configure( sprite.get(), *config );

            } else {
                const std::string pathConfigure =
                    pathEvent + "/configure.json";
                if ( boost::filesystem::exists( pathConfigure ) ) {
                    try {
                        const auto cfg = typelib::json::load( pathConfigure );
                        configure( sprite.get(), cfg );
                    } catch ( ... ) {
                        // останемся со значениями о умолчанию
                    }
                }
            }

        } // конфигурация образа
        */


        const enum UIDEvent euid = convertEvent( e );
        sprite->update();

        // запоминаем собранный спрайт
        addSprite( euid,  std::move( sprite ) );

#ifdef _DEBUG
        //std::cout << "Спрайт для события '" << e << "' собран.\n";
#endif

    } // for (auto etr = event.cbegin(); ...
}

    
    
    
void CLIncarnate::addSprite(
    enum UIDEvent  e,
    std::unique_ptr< CL_Sprite >  s
) {
    assert( (e != NONE_EVENT)
        && "Неопределённые события не должны храниться здесь." );
    assert( (mSprite->find( e ) == mSprite->end())
        && "Такое событие уже есть. Дубликаты запрещены." );

    // начало координат для спрайта - по центру
    const int cx = s->get_width()  / 2;
    const int cy = s->get_height() / 2;
    s->set_alignment( origin_center );

    // задаём масштаб
    // # Учитываем, что для спрайта уже мог быть установлен масштаб
    //   при загрузке (см. addSprite( path ...) ).
    const auto nvs = needVisualSize();
    float fixSX, fixSY;
    s->get_scale( fixSX, fixSY );
    const typelib::size2_t  scale(
        nvs.x / static_cast< float >( s->get_width()  ) * fixSX,
        nvs.y / static_cast< float >( s->get_height() ) * fixSY
    );
    s->set_scale( scale.x, scale.y );

    mSprite->insert( std::make_pair( e,  std::move( s ) ) );
}




void CLIncarnate::configure(
    CL_Sprite*  sprite,
    const typelib::json::Variant&  cfg
) {
    assert( false
        && "Не отлажено." );
    return;

    assert( sprite );

    // animation
    if ( cfg.has( "animation" ) ) {
        // поведение по умолчанию
        bool pingpong = false;
        bool loop     = true;
        size_t delay  = 100;

        const auto& def = cfg[ "animation" ];
        pingpong = def.at( "pingpong", pingpong );
        loop     = def.at( "loop",     loop );
        delay    = def.at( "delay",    delay );

        sprite->set_play_pingpong( pingpong );
        sprite->set_play_loop( loop );
        sprite->set_delay( delay );

    } // animation

    // scale
    if ( cfg.has( "scale" ) ) {
        float sx = 1.0f;
        float sy = 1.0f;

        const auto& def = cfg[ "scale" ];;
        sx = def.at( "x", sx );
        sy = def.at( "y", sy );

        sprite->set_scale( sx, sy );

    } // scale

    // @todo Другие параметры.
    // @see http://clanlib.org/wiki/MainDocs:Sprite_Resources
}


} // matchn
