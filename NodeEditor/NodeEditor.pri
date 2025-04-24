# 此模块源码来自于 github 上 Dmitry Pinaev 大神的开源项目, 链接: https://github.com/paceholder/nodeeditor
QT += widgets core gui opengl
CONFIG += c++11

DEFINES += NODE_EDITOR_STATIC

RESOURCES += \
    $$PWD/resources/resources.qrc

INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/include/QtNodes
INCLUDEPATH += $$PWD/include/QtNodes/internal

HEADERS += \
    $$PWD/include/QtNodes/internal/AbstractConnectionPainter.hpp \
    $$PWD/include/QtNodes/internal/AbstractGraphModel.hpp \
    $$PWD/include/QtNodes/internal/AbstractNodeGeometry.hpp \
    $$PWD/include/QtNodes/internal/AbstractNodePainter.hpp \
    $$PWD/include/QtNodes/internal/BasicGraphicsScene.hpp \
    $$PWD/include/QtNodes/internal/Compiler.hpp \
    $$PWD/include/QtNodes/internal/ConnectionGraphicsObject.hpp \
    $$PWD/include/QtNodes/internal/ConnectionIdHash.hpp \
    $$PWD/include/QtNodes/internal/ConnectionIdUtils.hpp \
    $$PWD/include/QtNodes/internal/ConnectionState.hpp \
    $$PWD/include/QtNodes/internal/ConnectionStyle.hpp \
    $$PWD/include/QtNodes/internal/DataFlowGraphModel.hpp \
    $$PWD/include/QtNodes/internal/DataFlowGraphicsScene.hpp \
    $$PWD/include/QtNodes/internal/DefaultConnectionPainter.hpp \
    $$PWD/include/QtNodes/internal/DefaultHorizontalNodeGeometry.hpp \
    $$PWD/include/QtNodes/internal/DefaultNodePainter.hpp \
    $$PWD/include/QtNodes/internal/DefaultVerticalNodeGeometry.hpp \
    $$PWD/include/QtNodes/internal/Definitions.hpp \
    $$PWD/include/QtNodes/internal/Export.hpp \
    $$PWD/include/QtNodes/internal/GraphicsView.hpp \
    $$PWD/include/QtNodes/internal/GraphicsViewStyle.hpp \
    $$PWD/include/QtNodes/internal/NodeConnectionInteraction.hpp \
    $$PWD/include/QtNodes/internal/NodeData.hpp \
    $$PWD/include/QtNodes/internal/NodeDelegateModel.hpp \
    $$PWD/include/QtNodes/internal/NodeDelegateModelRegistry.hpp \
    $$PWD/include/QtNodes/internal/NodeGraphicsObject.hpp \
    $$PWD/include/QtNodes/internal/NodeState.hpp \
    $$PWD/include/QtNodes/internal/NodeStyle.hpp \
    $$PWD/include/QtNodes/internal/OperatingSystem.hpp \
    $$PWD/include/QtNodes/internal/QStringStdHash.hpp \
    $$PWD/include/QtNodes/internal/QUuidStdHash.hpp \
    $$PWD/include/QtNodes/internal/Serializable.hpp \
    $$PWD/include/QtNodes/internal/Style.hpp \
    $$PWD/include/QtNodes/internal/StyleCollection.hpp \
    $$PWD/include/QtNodes/internal/UndoCommands.hpp \
    $$PWD/include/QtNodes/internal/locateNode.hpp

SOURCES += \
    $$PWD/src/AbstractGraphModel.cpp \
    $$PWD/src/AbstractNodeGeometry.cpp \
    $$PWD/src/BasicGraphicsScene.cpp \
    $$PWD/src/ConnectionGraphicsObject.cpp \
    $$PWD/src/ConnectionState.cpp \
    $$PWD/src/ConnectionStyle.cpp \
    $$PWD/src/DataFlowGraphModel.cpp \
    $$PWD/src/DataFlowGraphicsScene.cpp \
    $$PWD/src/DefaultConnectionPainter.cpp \
    $$PWD/src/DefaultHorizontalNodeGeometry.cpp \
    $$PWD/src/DefaultNodePainter.cpp \
    $$PWD/src/DefaultVerticalNodeGeometry.cpp \
    $$PWD/src/Definitions.cpp \
    $$PWD/src/GraphicsView.cpp \
    $$PWD/src/GraphicsViewStyle.cpp \
    $$PWD/src/NodeConnectionInteraction.cpp \
    $$PWD/src/NodeDelegateModel.cpp \
    $$PWD/src/NodeDelegateModelRegistry.cpp \
    $$PWD/src/NodeGraphicsObject.cpp \
    $$PWD/src/NodeState.cpp \
    $$PWD/src/NodeStyle.cpp \
    $$PWD/src/StyleCollection.cpp \
    $$PWD/src/UndoCommands.cpp \
    $$PWD/src/locateNode.cpp

