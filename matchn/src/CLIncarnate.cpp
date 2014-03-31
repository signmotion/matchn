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
    // �������� ������ ���� - ��. mWindowSize; mPreparedWindowSize ����������
    // ��� ���������������� ���������
    mWindowSize( CLIncarnate::world()->get_geometry().get_size() ),
    mWindowScale(
        static_cast< float >( mWindowSize.width  ) /
            static_cast< float >( mPreparedWindowSize.width ),
        static_cast< float >( mWindowSize.height ) /
            static_cast< float >( mPreparedWindowSize.height )
    )
{
    assert( !mWorld.expired()
        && "������� ����������� ���." );
    assert( mWorld.lock()->visual()
        && "���������� ��� ������ ���� ���������������." );
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
        && "������ ��� ��������� ������� �� ������." );

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
    //std::cout << "\n��������� '" << path << "' �� '" << extension << "'\n";
#endif

    // �������� ������ ��� �������
    // # ������ ������� ������������� �� ������ �������.
    std::set< std::string >  event;
    /* - ��������� ��� ��������� �������, �� ������ �����. ��. ����.
    typelib::file::listFolder( &event, path );
    */
    for (auto itr = accordanceEvent.begin(); itr != accordanceEvent.end(); ++itr) {
        if ( !itr->right.empty() ) {
            event.insert( itr->right );
        }
    }
    assert( !event.empty()
        && "��� ������� �� ������� �� ������ �������."
           " ���������� ����� �� ����� ���� ������." );

    for (auto etr = event.cbegin(); etr != event.cend(); ++etr) {
        const std::string& e = *etr;
        if ( boost::starts_with( e, "-" ) ) {
            // �����, ������������ �� '-', ����������
            continue;
        }

#ifdef _DEBUG
        //std::cout << "\n�������������� ������ ��� ������� '" << e << "'\n";
#endif

        // ������ ��� ����������� ������
        CL_SpriteDescription ds;
        std::set< std::string >  frame;
        std::string pathEvent = path + "/" + e;
        typelib::file::listFile( &frame, pathEvent, extension );
        /* - ���� ������ �� ����� 'default'. ��. ����.
        assert( !frame.empty()
            && "��� ������� �� ������� �� ������ �������. "
               " ���������� ����� �� ����� ���� ������." );
        */
        if ( frame.empty() ) {
            pathEvent = path + "/default";
            typelib::file::listFile( &frame, pathEvent, extension );
            assert( !frame.empty()
                && "��� �������� �� ������� �� ������ �������. "
                   " ���������� ����� �� ����� ���� ������." );
        }
        for (auto ftr = frame.cbegin(); ftr != frame.cend(); ++ftr) {
            const std::string& f = *ftr;
            const std::string pathFrame = pathEvent + "/" + f;
            const auto image = mManagerSprite->frame( pathFrame );
            ds.add_frame( *image );
        }

        // ��� ���. ����� ��� ������� 'e'
        std::unique_ptr< CL_Sprite >  sprite(
            new CL_Sprite( CLIncarnate::world()->get_gc(),  ds )
        );

        // ������������ ������
        /* - @todo ...
        {
            // ���� ������������ ������ �� ������ ����, ��������� �����
            // � � ����� �������
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
                        // ��������� �� ���������� � ���������
                    }
                }
            }

        } // ������������ ������
        */


        const enum UIDEvent euid = convertEvent( e );
        sprite->update();

        // ���������� ��������� ������
        addSprite( euid,  std::move( sprite ) );

#ifdef _DEBUG
        //std::cout << "������ ��� ������� '" << e << "' ������.\n";
#endif

    } // for (auto etr = event.cbegin(); ...
}

    
    
    
void CLIncarnate::addSprite(
    enum UIDEvent  e,
    std::unique_ptr< CL_Sprite >  s
) {
    assert( (e != NONE_EVENT)
        && "������������� ������� �� ������ ��������� �����." );
    assert( (mSprite->find( e ) == mSprite->end())
        && "����� ������� ��� ����. ��������� ���������." );

    // ������ ��������� ��� ������� - �� ������
    const int cx = s->get_width()  / 2;
    const int cy = s->get_height() / 2;
    s->set_alignment( origin_center );

    // ����� �������
    // # ���������, ��� ��� ������� ��� ��� ���� ���������� �������
    //   ��� �������� (��. addSprite( path ...) ).
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
        && "�� ��������." );
    return;

    assert( sprite );

    // animation
    if ( cfg.has( "animation" ) ) {
        // ��������� �� ���������
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

    // @todo ������ ���������.
    // @see http://clanlib.org/wiki/MainDocs:Sprite_Resources
}


} // matchn
