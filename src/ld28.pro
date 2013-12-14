CONFIG -= qt

TARGET = ld28
DESTDIR = ../bin

OBJECTS_DIR = build

LIBS += -lsfml-system -lsfml-graphics -lsfml-window -lBox2D
CONFIG += c++11

SOURCES += \
    main.cpp \
    World.cpp \
    MapLoader.cpp \
    DrawableObject.cpp \
    Map.cpp \
    WorldObject.cpp \
    WorldDebug.cpp \
    TileLayer.cpp \
    RenderProxy.cpp \
    StaticWorldPosition.cpp \
    PhysicsWorldPosition.cpp \
    Player.cpp \
    PositionFactory.cpp \
    ai/Pathfinder.cpp \
    RayCastResult.cpp \
    WorldGenerator.cpp \
    InputHandler.cpp \
    SettingsHandler.cpp \
    Settings.cpp \
    KeyMapping.cpp \
    InputMapping.cpp \
    states/GameState.cpp \
    AnalogMapping.cpp \
    PlayerInputProxy.cpp \
    StringEx.cpp \
    ai/PathNode.cpp \
    SettingsProvider.cpp \
    DefaultSettingsProvider.cpp \
    BinaryStream.cpp \
    states/MenuState.cpp \
    RoomLookupLoader.cpp \
    RoomLookup.cpp \
    RoomObject.cpp \
    Tile.cpp \
    TileObject.cpp \
    ai/Bot.cpp \
    ai/HumanoidBot.cpp

HEADERS += \
    World.h \
    MapLoader.h \
    DrawableObject.h \
    Map.h \
    StringEx.h \
    WorldObject.h \
    WorldDebug.h \
    TileLayer.h \
    RenderProxy.h \
    WorldPosition.h \
    StaticWorldPosition.h \
    PhysicsWorldPosition.h \
    Player.h \
    PositionFactory.h \
    ai/Bot.h \
    ai/Pathfinder.h \
    RayCastResult.h \
    WorldGenerator.h \
    InputHandler.h \
    SettingsHandler.h \
    Settings.h \
    KeyMapping.h \
    StreamingOperators.h \
    InputMapping.h \
    State.h \
    states/GameState.h \
    AnalogMapping.h \
    PlayerInputProxy.h \
    ai/PathNode.h \
    DereferencedEqual.h \
    DereferencedLess.h \
    ISettingsProvider.h \
    SettingsProvider.h \
    DefaultSettingsProvider.h \
    BinaryStream.h \
    states/MenuState.h \
    RoomLookupLoader.h \
    RoomLookup.h \
    RoomObject.h \
    Tile.h \
    TileObject.h \
    states/MenuState.h \
    ai/HumanoidBot.h

win32:DEFINES += M_PI=3.14159265358979323846
win32:LIBS += -LC:\Qt\Qt5.1.0\5.1.0\mingw48_32\lib -LC:\Qt\Qt5.1.0\5.1.0\mingw48_32\bin

# Needed for our OS X build bot
macx:CONFIG -= app_bundle
macx:LIBS += -L/usr/local/lib
macx:INCLUDEPATH += /usr/local/include
