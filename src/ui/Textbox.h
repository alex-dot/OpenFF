#pragma once

#include "../graphics/Window.h"
#include "../graphics/Text.h"
#include "../utilities/Configuration.h"
#include "../utilities/RessourceLoader.h"

using namespace Magnum;

namespace OpenFF {

const Vector2i default_viewport(320,240);

class Textbox{
  public:
    explicit Textbox();
    explicit Textbox(Configuration*, RessourceLoader*);
    explicit Textbox(
            Configuration* config,
            RessourceLoader* ressource_loader,
            Vector2 relative_billboard_ratio,
            Vector2i textbox_size,
            Vector2i offset);

    Textbox& prepare();

    Textbox& draw();
    Textbox& draw(std::string);
    Textbox& write(std::string);

    Textbox& show();
    Textbox& hide();

    // customize font and text
    Textbox& setFont(Font*);
    Textbox& setTextShadowType(OpenFF::ShadowTypes);
    // customize window
    // set uniform color
    Textbox& setColor(Color3);
    Textbox& setColor(Color4);
    // set color gradient, starting top-left going clockwise
    Textbox& setColor(Color3, Color3, Color3, Color3);
    Textbox& setColor(Color3, Color3, Color3, Color3, float);
    Textbox& setBodyTransparency(float);
    Textbox& setBorderTransparency(float);
    // set generic textbox variables
    Textbox& setRelativeBillboardRatio(Vector2);
    Textbox& setViewportSize(Vector2i);
    Textbox& setTextboxSize(Vector2i);
    Textbox& setOffset(Vector2i);
    Textbox& enableInstantRendering();
    Textbox& disableInstantRendering();

  private:
    void prepareWindow();
    void prepareText();

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
