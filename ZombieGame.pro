#-------------------------------------------------
#
# Project created by QtCreator 2016-04-02T19:04:21
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += xml
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ZombieGame
TEMPLATE = app

MOC_DIR     = release/build/moc
OBJECTS_DIR = release/build/obj
RCC_DIR     = release/


SOURCES += main.cpp\
    main_menu/mainmenu.cpp \
    main_menu/button.cpp \
    main_menu/inputpage.cpp \
    main_menu/multiplayerpage.cpp \
    main_menu/listitem.cpp \
    main_menu/serverlistitem.cpp \
    network/packets/packet.cpp \
    main_menu/addserverpopup.cpp \
    main_menu/inserttext.cpp \
    network/client.cpp \
    game/entities/bullet.cpp \
    game/entities/entity.cpp \
    game/game.cpp \
    game/multiplayergame.cpp \
    network/entities/playermp.cpp \
    core/config.cpp \
    core/inputmanager.cpp \
    core/window.cpp \
    network/entities/scientistmp.cpp \
    network/entities/zombiemp.cpp \
    game/entities/mobs/player.cpp \
    game/entities/mobs/scientist.cpp \
    game/entities/mobs/zombie.cpp \
    game/entities/mobs/mob.cpp \
    network/packets/packetentitydata.cpp \
    network/packets/packetleveldata.cpp \
    network/packets/packetplayerpos.cpp \
    network/packets/packetzombiechangetarget.cpp \
    network/packets/packetscientistchangedir.cpp \
    network/packets/packetfirebullet.cpp \
    network/packets/packetzombiehit.cpp \
    network/packets/packetbullethit.cpp \
    network/packets/packetplayerchat.cpp \
    network/packets/packetinfo.cpp \
    main_menu/borderlessrect.cpp \
    game/ui/chatbox.cpp \
    game/ui/pausemenu.cpp \
    game/ui/score.cpp \
    game/ui/uimanager.cpp \
    game/ui/uicomponent.cpp \
    level_creator/levelcreator.cpp \
    core/scene.cpp \
    game/misc/animationframework.cpp \
    game/misc/gun.cpp \
    game/misc/level.cpp \
    game/misc/tile.cpp \
    level_creator/misc/mousecontroller.cpp \
    level_creator/levelsizepage.cpp \
    level_creator/ui/bottombar.cpp \
    level_creator/ui/tileitem.cpp \
    level_creator/grid/grid.cpp \
    level_creator/grid/gridtile.cpp \
    level_creator/ui/tilemover.cpp \
    level_creator/grid/leveltile.cpp \
    misc/text.cpp \
    misc/language.cpp \
    misc/notification.cpp \
    misc/userdata.cpp \
    misc/notificationbar.cpp \
    network/packets/packetinfected.cpp \
    network/packets/packetping.cpp \
    network/packets/packetjoin.cpp \
    network/packets/packeterror.cpp \
    network/packets/packetdisconnect.cpp \
    network/packets/packetkick.cpp \
    game/misc/spectator.cpp \
    main_menu/levelselectorpage.cpp \
    main_menu/levellistitem.cpp \
    misc/musicmanager.cpp \
    network/packets/packetentityupdate.cpp

HEADERS  += \
    main_menu/mainmenu.h \
    main_menu/button.h \
    main_menu/inputpage.h \
    main_menu/multiplayerpage.h \
    main_menu/listitem.h \
    main_menu/serverlistitem.h \
    network/packets/packet.h \
    main_menu/addserverpopup.h \
    main_menu/inserttext.h \
    network/client.h \
    game/entities/bullet.h \
    game/entities/entity.h \
    game/game.h \
    game/multiplayergame.h \
    network/entities/playermp.h \
    core/config.h \
    core/inputmanager.h \
    core/window.h \
    network/entities/scientistmp.h \
    network/entities/zombiemp.h \
    game/entities/mobs/player.h \
    game/entities/mobs/scientist.h \
    game/entities/mobs/zombie.h \
    game/entities/mobs/mob.h \
    network/packets/packetentitydata.h \
    network/packets/packetleveldata.h \
    network/packets/packetplayerpos.h \
    network/packets/packetzombiechangetarget.h \
    network/packets/packetscientistchangedir.h \
    network/packets/packetfirebullet.h \
    network/packets/packetzombiehit.h \
    network/packets/packetbullethit.h \
    network/packets/packetplayerchat.h \
    network/packets/packetinfo.h \
    main_menu/borderlessrect.h \
    game/ui/chatbox.h \
    game/ui/pausemenu.h \
    game/ui/score.h \
    game/ui/uimanager.h \
    game/ui/uicomponent.h \
    level_creator/levelcreator.h \
    core/scene.h \
    game/misc/animationframework.h \
    game/misc/gun.h \
    game/misc/level.h \
    game/misc/tile.h \
    level_creator/misc/mousecontroller.h \
    level_creator/levelsizepage.h \
    level_creator/ui/bottombar.h \
    level_creator/ui/tileitem.h \
    level_creator/grid/grid.h \
    level_creator/grid/gridtile.h \
    level_creator/ui/tilemover.h \
    level_creator/grid/leveltile.h \
    misc/text.h \
    misc/language.h \
    misc/notification.h \
    misc/userdata.h \
    misc/notificationbar.h \
    network/packets/packetinfected.h \
    network/packets/packetping.h \
    network/packets/packetjoin.h \
    network/packets/packeterror.h \
    network/packets/packetdisconnect.h \
    network/packets/packetkick.h \
    game/misc/spectator.h \
    main_menu/levelselectorpage.h \
    main_menu/levellistitem.h \
    misc/musicmanager.h \
    network/packets/packetentityupdate.h

FORMS    +=

RESOURCES += \
    res.qrc

RC_FILE = ZombieGame.rc

DISTFILES +=
