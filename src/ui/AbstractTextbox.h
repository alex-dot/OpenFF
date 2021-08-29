#pragma once

#include "../graphics/Window.h"
#include "../graphics/Text.h"
#include "../utilities/Configuration.h"
#include "../utilities/RessourceLoader.h"

using namespace Magnum;

namespace OpenFF {

const Vector2i default_viewport(320,240);

class AbstractTextbox{
  public:
    explicit AbstractTextbox();
    explicit AbstractTextbox(Configuration*, RessourceLoader*);
    explicit AbstractTextbox(
            Configuration* config,
            RessourceLoader* ressource_loader,
            Vector2 relative_billboard_ratio,
            Vector2i textbox_size,
            Vector2i offset);

    AbstractTextbox& prepare();

    virtual AbstractTextbox& draw();
    virtual AbstractTextbox& draw(std::string);

    AbstractTextbox& show();
    AbstractTextbox& hide();

    // customize font and text
    AbstractTextbox& setFont(Font*);
    AbstractTextbox& setFont(std::string, int);
    AbstractTextbox& setTextShadowType(OpenFF::ShadowTypes);
    // customize window
    AbstractTextbox& setBorderImageLocation(std::string);
    // set uniform color
    AbstractTextbox& setColor(Color3);
    AbstractTextbox& setColor(Color4);
    // set color gradient, starting top-left going clockwise
    AbstractTextbox& setColor(Color3, Color3, Color3, Color3);
    AbstractTextbox& setColor(Color3, Color3, Color3, Color3, float);
    AbstractTextbox& setBodyTransparency(float);
    AbstractTextbox& setBorderTransparency(float);
    // set generic textbox variables
    virtual AbstractTextbox& enableInstantRendering();
    virtual AbstractTextbox& disableInstantRendering();
    virtual AbstractTextbox& setRelativeBillboardRatio(Vector2);
    AbstractTextbox& setViewportSize(Vector2i);
    AbstractTextbox& setResizeFactor(float);
    AbstractTextbox& enableRelativeBillboardRendering();
    AbstractTextbox& disableRelativeBillboardRendering();

    AbstractTextbox& setTextboxSize(Vector2i);
    AbstractTextbox& expand(int);
    AbstractTextbox& shrink(int);

    AbstractTextbox& setBorderOffset(Vector2i);
    AbstractTextbox& setOffset(Vector2i);
    Vector2i         getOffset();
    Vector2i         getTextboxSize();
    AbstractTextbox& move(Vector2i);

    virtual unsigned int getLineCount();
    virtual unsigned int getCharacterCountPerLine(unsigned int);
    virtual unsigned int getMaximumCharacterWidth();
    unsigned int getFontLineHeight();

    virtual AbstractTextbox& rewriteCharacter(unsigned int, unsigned int, Vector2i, std::string);
    virtual AbstractTextbox& moveCharacter(unsigned int, unsigned int, Vector2i);
    virtual AbstractTextbox& moveText(Vector2i);

    virtual AbstractTextbox& write(std::string) = 0;
    virtual AbstractTextbox& write() = 0;

  protected:
    void prepareWindow();
    virtual void prepareText() = 0;

    Configuration*    _config;
    RessourceLoader*  _ressource_loader;
    Text*             _text;
    Window*           _window;
    Font*             _font;
    std::string       _border_image_location;
    Vector2           _relative_billboard_ratio;
    Vector2i          _viewport_size;
    Vector2i          _textbox_size;
    Vector2i          _offset;
    Vector2i          _border_offset;
    bool              _hidden;
};

}
