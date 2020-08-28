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

    void prepare();

    void draw();
    void draw(std::string);
    void write(std::string);

    // customize font and text
    void setFont(Font*);
    void setTextShadowType(OpenFF::ShadowTypes);
    // customize window
    // set uniform color
    void setColor(Color3);
    void setColor(Color4);
    // set color gradient, starting top-left going clockwise
    void setColor(Color3, Color3, Color3, Color3);
    void setColor(Color3, Color3, Color3, Color3, float);
    void setBodyTransparency(float);
    void setBorderTransparency(float);
    // set generic textbox variables
    void setRelativeBillboardRatio(Vector2);
    void setViewportSize(Vector2i);
    void setTextboxSize(Vector2i);
    void setOffset(Vector2i);

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
};

}
