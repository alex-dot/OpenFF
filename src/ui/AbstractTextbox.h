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

    AbstractTextbox& draw();
    AbstractTextbox& draw(std::string);

    AbstractTextbox& show();
    AbstractTextbox& hide();

    // customize font and text
    AbstractTextbox& setFont(Font*);
    AbstractTextbox& setFont(std::string, int);
    AbstractTextbox& setTextShadowType(OpenFF::ShadowTypes);
    // customize window
    // set uniform color
    AbstractTextbox& setColor(Color3);
    AbstractTextbox& setColor(Color4);
    // set color gradient, starting top-left going clockwise
    AbstractTextbox& setColor(Color3, Color3, Color3, Color3);
    AbstractTextbox& setColor(Color3, Color3, Color3, Color3, float);
    AbstractTextbox& setBodyTransparency(float);
    AbstractTextbox& setBorderTransparency(float);
    // set generic textbox variables
    AbstractTextbox& setRelativeBillboardRatio(Vector2);
    AbstractTextbox& setViewportSize(Vector2i);
    AbstractTextbox& setTextboxSize(Vector2i);
    AbstractTextbox& setOffset(Vector2i);
    AbstractTextbox& enableInstantRendering();
    AbstractTextbox& disableInstantRendering();

    virtual AbstractTextbox& write(std::string) = 0;

  protected:
    void prepareWindow();

    virtual void prepareText() = 0;

    Configuration*    _config;
    RessourceLoader*  _ressource_loader;
    Text*             _text;
    Window*           _window;
    Font*             _font;
    Vector2           _relative_billboard_ratio;
    Vector2i          _viewport_size;
    Vector2i          _textbox_size;
    Vector2i          _offset;
    bool              _hidden;
};

}
